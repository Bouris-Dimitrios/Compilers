#ifndef __FINALCODE__C
#define __FINALCODE__C

#include "finalCodeFunctions.h"

void openFinalCodeFile(){
	strcat(finalCodeFileName,".mp");	

	finalCode =fopen(finalCodeFileName,"w+");
	if( finalCode == NULL){
		printf("Error opening %s\n",finalCodeFileName);
		exit(0);
	}
}

void gnlvCode(char * variableName,struct scope * currentScope){
	struct scope * aScope ;
	int varOffset;
	if(currentScope -> prevScope != NULL)
		aScope = currentScope -> prevScope;	
	else{
		printf("ERROR GNLVCODE\n");
		customExit();
	}

	fprintf(finalCode,"\tmovi R[254],4\n");
	fprintf(finalCode,"\taddi R[255],R[0],R[254]\n");
	fprintf(finalCode,"\tmovi R[255],M[R[255]]\n");

	varOffset = findDataInScope(variableName,aScope);

	while(varOffset == 0){ //TODO check mexri poses mporei na epitrepetai
		fprintf(finalCode,"\tmovi R[254],4\n");
		fprintf(finalCode,"\taddi R[255],R[255],R[254]\n");
		fprintf(finalCode,"\tmovi R[255],M[R[255]]\n");
	
		if(aScope -> prevScope != NULL)
			aScope = aScope -> prevScope;	
		else{
			printf("%s :Syntax Error:%d: Variable: \"%s\" "
					"			undeclared!\n",
			file_name,currentLineInFile,variableName);
			customExit();
		}	
		varOffset = findDataInScope(variableName,aScope);
	}
	fprintf(finalCode,"\tmovi R[254],%d\n",varOffset);
	fprintf(finalCode,"\taddi R[255],R[255],R[254]\n");

}

int isNumber(char * variableName,int regNum){
	char tmpVarName[TOKEN_SIZE];
	if((variableName[0]-0 >=48) && (variableName[0]-0 <= 57)){
		fprintf(finalCode,"\tmovi R[%d],%s\n",regNum,variableName);
		return 1;
	}
	strncpy(tmpVarName,variableName,MAX_TOKEN_CHARACTERS);
	if(tmpVarName[0] == '-'){
		tmpVarName[0] = ' ';
		fprintf(finalCode,"\tmovi R[255],%s\n",tmpVarName);
		fprintf(finalCode,"\tmovi R[254],0\n");		
		fprintf(finalCode,"\tsubi R[%d],R[254],R[255]\n",regNum);
		return 1;
	}
	if(tmpVarName[0] == '+'){
		tmpVarName[0] = ' ';
		fprintf(finalCode,"\tmovi R[%d],%s\n",regNum,tmpVarName);
		return 1;
	}
	return 0;
}

int isConst(char* constName,int regNum,struct scope* currentScope){
	struct entity* entity;
	int constValue ;

	constValue = findConstantInScope(constName,currentScope);
	if(constValue == -1)
		constValue = findConstantInScope(constName,
						currentScope -> prevScope);
	if(constValue == -1)
		constValue = findGlobalConstant(constName,currentScope);

	if( constValue != -1 ){
		fprintf(finalCode,"\tmovi R[%d],%d\n",regNum,constValue);			
		return 1;
	}

	return 0;
}

void loadvr(char * variableName,int regNum,struct scope * currentScope){
	struct scope * aScope;
	struct entity* entity;
	int offset=0;

	if(isNumber(variableName,regNum))
		return;	
	if(isConst(variableName,regNum,currentScope))
		return;
	offset = findLocalOrTemporaryOrParameter(variableName,currentScope);
	if(offset != 0){
		fprintf(finalCode,"\tmovi R[255],%d\n",offset);
		fprintf(finalCode,"\taddi R[255],R[255],R[0]\n");
		fprintf(finalCode,"\tmovi R[%d],M[R[255]]\n",regNum);
		return;
	}

	offset = findByReferenceParameter(variableName,currentScope);
	if( offset != 0 ){
		fprintf(finalCode,"\tmovi R[255],%d\n",offset);
		fprintf(finalCode,"\taddi R[255],R[255],R[0]\n");
		fprintf(finalCode,"\tmovi R[255],M[R[255]]\n");
		fprintf(finalCode,"\tmovi R[%d],M[R[255]]\n",regNum);
		return;
		}

	offset = findLocalOrTemporaryOrParameter(variableName,currentScope->prevScope);
	if(offset != 0){
		gnlvCode(variableName,currentScope);
		fprintf(finalCode,"\tmovi R[%d],M[R[255]]\n",regNum);
		return;
	}

	offset = findByReferenceParameter(variableName,currentScope->prevScope);
	if(offset != 0){
		gnlvCode(variableName,currentScope);
		fprintf(finalCode,"\tmovi R[255],M[R[255]]\n");
		fprintf(finalCode,"\tmovi R[%d],M[R[255]]\n",regNum);
		return;	
	}
	offset = findGlobalVariable(variableName,currentScope);
	if (offset != 0){
		fprintf(finalCode,"\tmovi R[254],600\n");
		fprintf(finalCode,"\tmovi R[255],%d\n",offset);
		fprintf(finalCode,"\taddi R[255],R[255],R[254]\n");	
		fprintf(finalCode,"\tmovi R[%d],M[R[255]]\n",regNum);
		return;
	}	
	else{
		printf("%s :Syntax Error:%d: Variable: \"%s\" undeclared!\n",
		file_name,currentLineInFile,variableName);
		customExit();
	}
}


void storerv(int regNum,char* variableName,struct scope* currentScope){
	int offset = 0;
	offset = findLocalOrTemporaryOrParameter(variableName,currentScope);
	if(offset != 0){
		fprintf(finalCode,"\tmovi R[254],%d\n",offset);
		fprintf(finalCode,"\taddi R[255],R[254],R[0]\n");
		fprintf(finalCode,"\tmovi M[R[255]],R[%d]\n",regNum);
		return;
	}	

	offset = findByReferenceParameter(variableName,currentScope);
	if(offset != 0 ){
		fprintf(finalCode,"\tmovi R[255],%d\n",offset);
		fprintf(finalCode,"\taddi R[255],R[255],R[0]\n");
		fprintf(finalCode,"\tmovi R[255],M[R[255]]\n");
		fprintf(finalCode,"\tmovi M[R[255]],R[%d]\n",regNum);
		return;
	}

	offset = findLocalOrTemporaryOrParameter(variableName,
						currentScope->prevScope);
	if(offset != 0){
		gnlvCode(variableName,currentScope);
		fprintf(finalCode,"\tmovi M[R[255]],R[%d]\n",regNum);
		return;
	}

	offset = findByReferenceParameter(variableName,currentScope->prevScope);
	if (offset !=0 ){
		gnlvCode(variableName,currentScope);
		fprintf(finalCode,"\tmovi R[255],M[R[255]]\n");
		fprintf(finalCode,"\tmovi M[R[255]],R[%d]\n",regNum);
		return;		
	}

	offset = findGlobalVariable(variableName,currentScope);
	if(offset != 0){
		fprintf(finalCode,"\tmovi R[254],600\n");
		fprintf(finalCode,"\tmovi R[255],%d\n",offset);
		fprintf(finalCode,"\taddi R[255],R[255],R[254]\n");
		fprintf(finalCode,"\tmovi M[R[255]],R[%d]\n",regNum);
		return;
	}
	else{
		printf("%s :Syntax Error:%d: Variable: \"%s\" undeclared!\n",
		file_name,currentLineInFile,variableName);
		customExit();
	}
}

int isRelop(char* op){
	if(strcmp(op,"=") == 0)
		return equal;
	if(strcmp(op,"<>") == 0)
		return diffTk;
	if(strcmp(op,">") == 0)
		return moreThan;
	if(strcmp(op,">=") == 0)
		return moreeqTk;
	if(strcmp(op,"<") == 0)
		return lessThan;
	if(strcmp(op,"<=") == 0)
		return lesseqTk;
	return 0;
}

int isOperator(char* operator){
	if(strcmp(operator,"+") == 0)
		return plus;
	if(strcmp(operator,"-") == 0)
		return minus;
	if(strcmp(operator,"*") == 0)
		return star;
	if(strcmp(operator,"/") == 0)
		return slash;
	return 0;
}

void produceRelopFinalCode(struct scope* currentScope){
	int opCode = isRelop(compileCounter->op);
	loadvr(compileCounter->x,1,currentScope);
	loadvr(compileCounter->y,2,currentScope);
	fprintf(finalCode,"\tcmpi R[1],R[2]\n");
	if( opCode == equal)
		fprintf(finalCode,"\tje L%s\n",compileCounter->z);
	else if(opCode == moreThan)
		fprintf(finalCode,"\tjb L%s\n",compileCounter->z);
	else if(opCode == lessThan)
		fprintf(finalCode,"\tja L%s\n",compileCounter->z);
	else if(opCode == moreeqTk)
		fprintf(finalCode,"\tjbe L%s\n",compileCounter->z);
	else if(opCode == lesseqTk)
		fprintf(finalCode,"\tjae L%s\n",compileCounter->z);
	else 
		fprintf(finalCode,"\tjne L%s\n",compileCounter->z);
}

void produceOperatorFinalCode(struct scope* currentScope){
	int opCode = isOperator(compileCounter->op);
	loadvr(compileCounter->x,1,currentScope);
	loadvr(compileCounter->y,2,currentScope);
	if(opCode == plus)
		fprintf(finalCode,"\taddi R[3],R[1],R[2]\n");
	else if(opCode == minus)
		fprintf(finalCode,"\tsubi R[3],R[1],R[2]\n");
	else if(opCode == star)
		fprintf(finalCode,"\tmuli R[3],R[1],R[2]\n");
	else if(opCode == slash)
		fprintf(finalCode,"\tdivi R[3],R[1],R[2]\n");
	storerv(3,compileCounter->z,currentScope);
}


void produceReturnValueFinalCode(struct scope* currentScope){
	loadvr(compileCounter->x,1,currentScope);
	fprintf(finalCode,"\tmovi R[254],8\n");
	fprintf(finalCode,"\taddi R[255],R[0],R[254]\n");
	fprintf(finalCode,"\tmovi R[255],M[R[255]]\n");
	fprintf(finalCode,"\tmovi M[R[255]],R[1]\n");
}

void produceParameterPassingCvFinalCode(struct scope* currentScope, int dValue){
	loadvr(compileCounter->x,255,currentScope);
	fprintf(finalCode,"\tmovi R[254],%d\n",dValue);
	fprintf(finalCode,"\taddi R[254],R[254],R[0]\n");
	fprintf(finalCode,"\tmovi M[R[254]],R[255]\n");
}

void produceParameterPassingReferenceFinalCode(struct scope* currentScope, 
								int dValue){ 
	int offset;
	offset = findLocalOrTemporaryOrParameter(compileCounter->x,currentScope);
	if(offset != 0){
		fprintf(finalCode,"\tmovi R[255],R[0]\n");
		fprintf(finalCode,"\tmovi R[254],%d\n",offset);
		fprintf(finalCode,"\taddi R[255],R[254],R[255]\n");
		fprintf(finalCode,"\tmovi R[254],%d\n",dValue);
		fprintf(finalCode,"\taddi R[254],R[254],R[0]\n");
		fprintf(finalCode,"\tmovi M[R[254]],R[255]\n");		
		return;			
	}

	offset = findByReferenceParameter(compileCounter->x,currentScope);
	if( offset != 0 ){
		fprintf(finalCode,"\tmovi R[255],R[0]\n");
		fprintf(finalCode,"\tmovi R[254],%d\n",offset);
		fprintf(finalCode,"\taddi R[255],R[254],R[255]\n");
		fprintf(finalCode,"\tmovi R[1],M[R[255]]\n");
		fprintf(finalCode,"\tmovi R[254],%d\n",dValue);
		fprintf(finalCode,"\taddi R[254],R[254],R[0]\n");
		fprintf(finalCode,"\tmovi M[R[254]],R[1]\n");	
		return;				
	}

	offset = findLocalOrTemporaryOrParameter(compileCounter->x,currentScope->prevScope);
	if(offset != 0){
		gnlvCode(compileCounter->x,currentScope);
		fprintf(finalCode,"\tmovi R[254],%d\n",dValue);
		fprintf(finalCode,"\taddi R[254],R[254],R[0]\n");
		fprintf(finalCode,"\tmovi M[R[254]],R[255]\n");	
		return;	

	}
	
	offset = findByReferenceParameter(compileCounter->x,currentScope->prevScope);
	if(offset != 0){
		gnlvCode(compileCounter->x,currentScope);
		fprintf(finalCode,"\tmovi R[1],M[R[255]]\n");
		fprintf(finalCode,"\tmovi R[254],%d\n",dValue);
		fprintf(finalCode,"\taddi R[254],R[254],R[0]\n");
		fprintf(finalCode,"\tmovi M[R[254]],R[1]\n");	
		return;
	}
	else{
		printf("%s :Syntax Error:%d: Variable: \"%s\" undeclared!\n",
		file_name,currentLineInFile,compileCounter->x);
		customExit();
	}
}

void produceParameterPassingReturnFinalCode(struct scope* currentScope, int frameLength){
	int offset = findLocalOrTemporaryOrParameter(compileCounter->x,currentScope);

	fprintf(finalCode,"\tmovi R[255],R[0]\n");
	fprintf(finalCode,"\tmovi R[254],%d\n",offset);
	fprintf(finalCode,"\taddi R[255],R[254],R[255]\n");
	fprintf(finalCode,"\tmovi R[254],%d\n",frameLength + 8);
	fprintf(finalCode,"\taddi R[254],R[254],R[0]\n");
	fprintf(finalCode,"\tmovi M[R[254]],R[255]\n");	
}

void produceCallFunctionFinalCode(struct scope* currentScope,char* blockId){
	int frameLength;
	int foundMain =0;
	struct entity* entity,*callFunEntity,*checkForMain;

	callFunEntity = findEntity(compileCounter->x,currentScope ) ; 
	if(callFunEntity != NULL)
		;
	else if(currentScope->prevScope != NULL)
		callFunEntity = findEntity(compileCounter->x,
						  currentScope -> prevScope) ; 
	if(currentScope->prevScope != NULL)
		entity = findEntity(blockId,currentScope->prevScope) ; 
	else
		entity = findEntity(blockId,currentScope) ; 

	frameLength = entity->data.fun.frameLength ;
	checkForMain = findEntityInSymbolTable(blockId,currentScope) ;
	if ( checkForMain != NULL ){
		if(checkForMain -> type == 256)
			foundMain=1;	
	}

	if(findEntityAndReturnNestingLevel(blockId,currentScope) 
	== findEntityAndReturnNestingLevel(compileCounter->x,currentScope) && 
								foundMain ==0){ 
		fprintf(finalCode,"\tmovi R[254],4\n");
		fprintf(finalCode,"\taddi R[254],R[254],R[0]\n");		
		fprintf(finalCode,"\tmovi R[255],M[R[254]]\n");
		fprintf(finalCode,"\tmovi R[254],%d\n",frameLength + 4);
		fprintf(finalCode,"\taddi R[254],R[254],R[0]\n");
		fprintf(finalCode,"\tmovi M[R[254]],R[255]\n");	
	}
	else
		fprintf(finalCode,"\tmovi M[%d + R[0]],R[0]\n",frameLength + 4);

	fprintf(finalCode,"\tmovi R[255],%d\n",frameLength);
	fprintf(finalCode,"\taddi R[0],R[255],R[0]\n");
	fprintf(finalCode,"\tmovi R[255],$\n");
	fprintf(finalCode,"\tmovi R[254],15\n");
	fprintf(finalCode,"\taddi R[255],R[255],R[254]\n");
	fprintf(finalCode,"\tmovi M[R[0]],R[255]\n");

	fprintf(finalCode,"\tjmp L%s\n",callFunEntity->data.fun.startQuad);	
	fprintf(finalCode,"\tmovi R[255],%d\n",frameLength);
	fprintf(finalCode,"\tsubi R[0],R[0],R[255]\n");
}

void produceAssignmentFinalCode(struct scope* currentScope){
	loadvr(compileCounter->x,1,currentScope);
	storerv(1,compileCounter->z,currentScope);
}

void produceOutFinalCode(struct scope* currentScope){
	loadvr(compileCounter->x,1,currentScope);
	fprintf(finalCode,"\touti R[1]\n");
}


void produceInFinalCode(struct scope* currentScope){
	fprintf(finalCode,"\tini R[1]\n");
	storerv(1,compileCounter->x,currentScope);
}

void produceJumpFinalCode(){
	fprintf(finalCode,"\tjmp L%s\n",compileCounter->z);
}

void produceHaltFinalCode(){
	fprintf(finalCode,"\thalt\n");
}

void produceEndBlockFinalCode(struct scope* currentScope,char* blockId){
	struct entity* entity;
	if(currentScope->prevScope != NULL){
		entity = findEntity(blockId,currentScope->prevScope) ;
		fprintf(finalCode,"L%s: ",compileCounter->quadNumber);
	}
	else
		return;
	fprintf(finalCode,"\tjmp M[R[0]]\n",compileCounter->quadNumber);
}

void printLabelForFinalCodeBlock(){
	if( strcmp(compileCounter->op,"begin_block") != 0 
			&& strcmp(compileCounter->op,"end_block") != 0 )
		fprintf(finalCode,"L%s: ",compileCounter->quadNumber);
}

void produceFinalCodeForSubProgram(struct scope* currentScope,char* blockId ){
	int endBlockFound = 0,properOffsetInNewFunction = 0,parNum = 0;
	struct entity* entity;
	while( endBlockFound != 1){
		compileCounter = compileCounter->next;

		printLabelForFinalCodeBlock();
		if( strcmp(compileCounter->op,"jump") == 0)
			produceJumpFinalCode();
		else if( isRelop(compileCounter->op) != 0 )
			produceRelopFinalCode(currentScope);
		else if( strcmp(compileCounter->op,":=") ==0 )
			produceAssignmentFinalCode(currentScope);
		else if( isOperator(compileCounter->op) != 0)
			produceOperatorFinalCode(currentScope);
		else if (strcmp(compileCounter->op,"out") == 0 )
			produceOutFinalCode(currentScope);
		else if (strcmp(compileCounter->op,"inp") == 0 )
			produceInFinalCode(currentScope);
		else if( strcmp(compileCounter->op,"retv") == 0 )
			produceReturnValueFinalCode(currentScope);	
		else if( strcmp(compileCounter->op,"par") == 0 ){

			if(currentScope->prevScope != NULL)
				entity = findEntity(blockId,
						   currentScope->prevScope) ;
			else
				entity = findEntity(blockId,currentScope) ; 

			properOffsetInNewFunction = entity->data.fun.frameLength
						+ 12 + (4 * parNum);

			if(strcmp(compileCounter->y,"CV") == 0){
				produceParameterPassingCvFinalCode(currentScope,
						properOffsetInNewFunction);
				parNum++;
			}
			else if(strcmp(compileCounter->y,"REF") == 0){
				produceParameterPassingReferenceFinalCode(
					currentScope,properOffsetInNewFunction);
				parNum++;
			}
			else if(strcmp(compileCounter->y,"RET") == 0)
				produceParameterPassingReturnFinalCode(
				currentScope,entity->data.fun.frameLength);

		}
		else if( strcmp(compileCounter->op,"call") == 0){ 
			produceCallFunctionFinalCode(currentScope,blockId);
			parNum = 0 ;
		}
		else if (strcmp(compileCounter->op,"halt") == 0)
			produceHaltFinalCode();
		else if( strcmp(compileCounter->op,"end_block") == 0){
			endBlockFound = 1;
			produceEndBlockFinalCode(currentScope,blockId);
		}
		fprintf(finalCode,"\n");
	}
}


#endif
