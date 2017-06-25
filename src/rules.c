#ifndef __RULES_C__ 
#define __RULES_C__
/**
*@file rules.c
*\authors{ Bouris Dimitris 1894 (bourisdim@gmail.com) ,
 Nikos Ginargyros 2038 (nikospaxos@hotmail.com) }
*@date  March ,2014
*@brief  Contains the grammar rules of Mini Pascal language.
*All these rules are used in order to recognize the syntax
*structure of the input file.They are the "heart" of syntax analyzer.
*/
#include "rules.h"

void program()
{
	char blockId[TOKEN_SIZE];
	struct labelList * startList = NULL;
	struct scope* mainScope = NULL;
	struct entity* newEntity;

	allocateScope(&mainScope);

	genQuad("jump","_","_","_");

	compileCounter = programList->head;
	startList = makeList(quadNumber);
	if (token_id == programTk)
	{
		token_id = lex();
		if (token_id == identifierTk)
		{
			strncpy(blockId,token,MAX_TOKEN_CHARACTERS);
			strncpy(finalCodeFileName,token,MAX_TOKEN_CHARACTERS);
			openFinalCodeFile();
			fprintf(finalCode,"L1:\tjmp L0\n");
			newEntity = createNewEntity(blockId,256,FUNCTION);
			addEntityToScope(newEntity,mainScope);
			token_id = lex();
			programBlock(blockId,PROGRAMBLOCK,startList,mainScope); 
			produceFinalCodeForSubProgram(mainScope,blockId);
			freeScope(&mainScope);
		}
		else
		{
			printf("%s :Syntax Error:%d:Program name expected."
			"Found: \" %s \".\n",file_name,currentLineInFile,token);
			exit(EXIT_FAILURE);
		}
	}
	else 
	{
		printf("%s :Syntax Error:%d:The keyword 'program' was expected."
		"Found: \" %s \"\n",file_name,currentLineInFile,token);
		exit(EXIT_FAILURE);
	}
}

void programBlock(char * blockId,int isMainBlock, struct labelList *startList,
						 struct scope* currentScope)
{
	struct entity *anEntity;
	char nextQuad[TOKEN_SIZE];
	declarations(currentScope);
	subPrograms(currentScope);
	
	genQuad("begin_block",blockId,"_","_");
	if( isMainBlock == PROGRAMBLOCK ){
		backPatch(&startList,quadNumber + 1);
		anEntity = findEntity(blockId,currentScope);
		sprintf(nextQuad,"%d",quadNumber + 1);
		strncpy(anEntity->data.fun.startQuad,nextQuad,
						MAX_TOKEN_CHARACTERS);
	}
	else
	{
		anEntity = findEntity(blockId,currentScope-> prevScope);
		sprintf(nextQuad,"%d",quadNumber + 1);
		strncpy(anEntity->data.fun.startQuad,nextQuad,
						MAX_TOKEN_CHARACTERS);
	}
	block(currentScope);

	if( isMainBlock == PROGRAMBLOCK){
		anEntity = findEntity(blockId,currentScope);
		anEntity ->data.fun.frameLength = currentScope -> currentOffset;	
		fprintf(finalCode,"L0:\tmovi R[0],600\n\tjmp L%s\n\n",anEntity->data.fun.startQuad);	
		genQuad("halt","_","_","_");
		compileCounter = compileCounter->next;
	}
	genQuad("end_block",blockId,"_","_");
}

void block(struct scope* currentScope)
{
	if(token_id == beginTk)
	{
		token_id = lex();	
		sequence(currentScope);
		if( token_id == endTk )
			token_id = lex();
		else
		{
			printf("%s :Syntax Error:%d:The keyword 'end' was "
			"expected.Found: \" %s \".\n",file_name
			,currentLineInFile,token);
			printSpaces();
			printf("Suggestion:Probably you forgot a  semicolon ';'"
			".\n");
			customExit();
		}
	}
	else
	{
		printf("%s :Syntax Error:%d:The keyword 'begin' was expected."
		"Found: \" %s \".\n",file_name,currentLineInFile,token);
		customExit();
	}
}

void sequence(struct scope* currentScope)
{
	statement(currentScope);
	while(token_id == semiColon)
	{
		token_id = lex();
		statement(currentScope);
	}
	
}

void statement(struct scope* currentScope)
{ 	
	if( token_id == ifTk )
	{
		token_id = lex();
		ifStat(currentScope);
	}
	else if ( token_id == whileTk)
	{
		token_id = lex();
		whileStat(currentScope);
	}
	else if ( token_id == forTk)
	{
		token_id = lex();
		forStat(currentScope);
	} 
	else if ( token_id == selectTk)
	{
		token_id = lex();
		selectStat(currentScope);
	}
	else if ( token_id == callTk)
	{
		token_id = lex();
		callStat(currentScope);
	}
	else if ( token_id == printTk)
	{
		char PRINTplace[TOKEN_SIZE];
		token_id = lex();
		printStat(PRINTplace,currentScope);
	}
	else if ( token_id == inputTk)
	{
		char INPUTplace[TOKEN_SIZE];
		token_id = lex();
		inputStat(INPUTplace);
	}
	else if ( token_id == returnTk)
	{
		token_id = lex();
		returnStat(currentScope);
	}
	else if(token_id == identifierTk)
	{
		char ASSIGNMENTplace[TOKEN_SIZE];
		char idPlace[TOKEN_SIZE];
		assignmentStat(ASSIGNMENTplace,idPlace,currentScope);
	}
	else
	{
		if(token_id == elseTk) 
		{
			printf("%s :Syntax Error:%d:Unexpected \"else\".\n"
			,file_name,currentLineInFile);
			printSpaces();
			printf("Found \"else\" without previous if "
			"statement.\n");
			customExit();
		}
		else
		{
			printf("%s :Syntax Error:%d:Statement was expected.\n"
			,file_name,currentLineInFile);
			printSpaces();
			printf("Unexpected : \" %s \". This isn't a statement."
			"\n",token);
			customExit();
		}
	}	
}

void returnStat(struct scope* currentScope)
{
	char Eplace[TOKEN_SIZE];
	if(token_id == leftPar)
	{
		token_id = lex();
		expression(Eplace,currentScope);
		genQuad("retv",Eplace,"_","_");
		if(token_id == rightPar)	
			token_id = lex();
		else	
		{
			printf("%s :Syntax Error:%d:The character ')' "
			"was expected.\n",file_name,currentLineInFile);
			printSpaces();
			printf("Proper syntax for \"return\" "
			"statement is:  return (<expression>).\n");
			customExit();
		}		
	}
	else
	{
		printf("%s :Syntax Error:%d:The character '(' "
		"was expected.\n",file_name,currentLineInFile);
		printSpaces();
		printf("Proper syntax for \"return\" "
		"statement is:  return (<expression>).\n");
		customExit();
	}
}

void selectStat(struct scope *currentScope)
{
	char Eplace[TOKEN_SIZE];
	struct labelList *jumpOutList = NULL;
	allocateLabelList(&jumpOutList);
	if(token_id == ifTk)
	{
		token_id = lex();
		if(token_id == leftPar)
		{
			token_id = lex();
			expression(Eplace,currentScope);
			if(token_id == rightPar)
			{
				token_id = lex();
				selectConStat(Eplace,jumpOutList,currentScope);
				while(token_id == isTk)
					selectConStat(Eplace,jumpOutList,
								currentScope);

				backPatch(&jumpOutList,quadNumber + 1);
				
				if(token_id == endSelectTk)
					token_id = lex();
				else
				{
					printf("%s :Syntax Error:%d:The keyword"
					" 'endselect' was expected.\n"
					,file_name,currentLineInFile);
					printSpaces();
					printf("Proper syntax for \"select if\"" 
					" statement is:\n");
					printSpaces();
					printf("select if(<expression>) is "
					"equal to : <expression> <block> "
					"endselect.\n");
					customExit();
				}
			}
			else
			{
				printf("%s :Syntax Error:%d:The character ')' "
				"was expected.\n",file_name,currentLineInFile);
				printSpaces();
				printf("Proper syntax for \"select if \""
				" statement is:\n");
				printSpaces();
				printf("select if(<expression>) is equal to :"
				" <expression> <block> endselect.\n");
				customExit();
			}
		}		
		else
		{
			printf("%s :Syntax Error:%d:The character '(' was "
			"expected.\n",file_name,currentLineInFile);
			printSpaces();
			printf("Proper syntax for \"select if \" statement is:"
			"\n");
			printSpaces();
			printf("select if(<expression>) is equal to"
			" : <expression> <block> endselect.\n");
			customExit();
		}
	}
	else 
	{
		printf("%s :Syntax Error:%d:The keyword 'if' was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("Proper syntax for \"select if \" statement is:\n");
		printSpaces();
		printf("select if (<expression>) is equal to : <expression> "
		"<block> endselect.\n");
		customExit();
	}
}

void selectConStat(char* Eplace1,struct labelList *jumpOutList, 
						struct scope *currentScope)
{
	char Eplace2[TOKEN_SIZE];
	struct labelList* trueList = NULL;
	struct labelList* falseList = NULL;
	struct labelList* localJumpList = NULL;
	if( token_id == isTk)
	{
		token_id = lex();
		if(token_id == equalKeyTk)
		{
			token_id = lex();
			if(token_id == toTk)
			{
				token_id = lex();
				if(token_id == colon)
				{
					token_id = lex();
					expression(Eplace2,currentScope);

					genQuad("=",Eplace1,Eplace2,"_");
					trueList = makeList(quadNumber);
					genQuad("jump","_","_","_");
					falseList = makeList(quadNumber);
					backPatch(&trueList,quadNumber + 1);

					block(currentScope);
		
					genQuad("jump","_","_","_");
					localJumpList = makeList(quadNumber);
					backPatch(&falseList,quadNumber + 1);
					merge(jumpOutList,localJumpList);
				}
				else
				{
					printf("%s :Syntax Error:%d:The "
					"character ':' was expected.\n"	
					,file_name,currentLineInFile);
					printSpaces();
					printf("Proper syntax for \"select if "
					"\" statement is:\n");
					printSpaces();
					printf("select if (<expression>) is "
					"equal to : <expression> <block> "
					"endselect.\n");
					customExit();
				}
			}
			else
			{
				printf("%s :Syntax Error:%d:The keyword 'to'"
				" was expected.\n",file_name,currentLineInFile);
				printSpaces();
				printf(" Proper syntax for \"select if \" "
				"statement is:\n");
				printSpaces();
				printf("select if (<expression>) is equal to : "
				"<expression> <block> endselect.\n");
				customExit();
			}
		}
		else
		{
			printf("%s :Syntax Error:%d:The keyword 'equal' was"
			" expected.\n",file_name,currentLineInFile);
			printSpaces();
			printf("Proper syntax for \"select if \"statement is:"
			"\n");
			printSpaces();
			printf("select if(<expression>) is equal to "
			": <expression> <block> endselect.\n");
			customExit();
		}
	}
	else
	{
		printf("%s :Syntax Error:%d:The keyword 'is' was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("Proper syntax for \"select if \" statement is:\n");
		printSpaces();
		printf("select if(<expression>) is equal to : <expression>"
		" <block> endselect.\n");
		customExit();
	}


}

void ifStat(struct scope *currentScope)
{
	struct labelList *condTrueList = NULL, *condFalseList = NULL;
	struct labelList *ifList=NULL;
	condition(&condTrueList,&condFalseList,currentScope);
	if( token_id == thenTk)
	{
		backPatch(&condTrueList,quadNumber+1);
		token_id = lex();
		blockOrStat(currentScope);

		genQuad("jump","_","_","_");
		ifList = makeList(quadNumber);
		backPatch(&condFalseList,quadNumber+1);

		elsePart(currentScope);
		backPatch(&ifList,quadNumber+1);
	}
	else
	{
		printf("%s :Syntax Error:%d:The keyword 'then' was expected."
		"Found: \" %s \".\n",file_name,currentLineInFile,token);
		customExit();
	}
}

void condition(struct labelList **condTrueList,struct labelList **condFalseList,
						struct scope *currentScope)
{
	struct labelList *boolTerm1TrueList = NULL
		, *boolTerm1FalseList = NULL
	 	, *boolTerm2TrueList = NULL
 		, *boolTerm2FalseList = NULL;
	allocateLabelList(&boolTerm1TrueList );
	allocateLabelList(&boolTerm1FalseList );
	allocateLabelList(&boolTerm2TrueList );
	allocateLabelList(&boolTerm2FalseList );

	boolTerm(&boolTerm1TrueList,&boolTerm1FalseList,currentScope);

	*condTrueList = boolTerm1TrueList;
	*condFalseList = boolTerm1FalseList;

	while(token_id==orTk)
	{
		backPatch(condFalseList, (quadNumber+1) );

		token_id=lex();
		boolTerm(&boolTerm2TrueList,&boolTerm2FalseList,currentScope);
		merge(*condTrueList, boolTerm2TrueList);
		*condFalseList = boolTerm2FalseList;
	}
}

void elsePart(struct scope *currentScope)
{
	if( token_id == elseTk)
	{
		token_id = lex();
		blockOrStat(currentScope);
	}
}

void blockOrStat(struct scope *currentScope)
{
	if(token_id == beginTk)
		block(currentScope);
	else
		statement(currentScope);
}

void boolTerm(struct labelList **boolTermTrueList,
		struct labelList **boolTermFalseList,struct scope *currentScope)
{

	struct labelList *boolFactor1TrueList = NULL
		, *boolFactor1FalseList = NULL
		, *boolFactor2TrueList = NULL
		, *boolFactor2FalseList = NULL;
	boolFactor(&boolFactor1TrueList,&boolFactor1FalseList,currentScope);
	*boolTermTrueList = boolFactor1TrueList;
	*boolTermFalseList = boolFactor1FalseList;
	while(token_id == andTk)
	{
		
		backPatch(boolTermTrueList,quadNumber+1);

		token_id=lex();
		boolFactor(&boolFactor2TrueList,&boolFactor2FalseList,
								currentScope);
		merge(*boolTermFalseList,boolFactor2FalseList);
		*boolTermTrueList = boolFactor2TrueList ;

	}
}

void boolFactor(struct labelList **boolFactorTrueList,
		struct labelList **boolFactorFalseList,
				struct scope *currentScope)
{
	char E1place[TOKEN_SIZE];
	char E2place[TOKEN_SIZE];
	char RELOPplace[TOKEN_SIZE];
	struct labelList *condTrueList = NULL, *condFalseList = NULL;
	if(token_id == notTk)
	{
		token_id = lex();
		if(token_id == leftBrk)
		{
			token_id = lex();
			condition(&condTrueList,&condFalseList,currentScope);
			if(token_id == rightBrk)
				token_id = lex();
			else
			{
				printf("%s :Syntax Error:%d:The character ']' "
				"was expected.Found: \" %s \"\n"
				,file_name,currentLineInFile,token);
				printSpaces();
				printf("The proper syntax for conditions is:"
				" [ <condition> ].\n");
				customExit();
			}
			*boolFactorTrueList = condFalseList;
			*boolFactorFalseList = condTrueList;
		}
		else
		{
			printf("%s :Syntax Error:%d:The character '[' was "
			"expected.Found: \" %s \"\n"
			,file_name,currentLineInFile,token);
			printSpaces();
			printf("The proper syntax for conditions is: "
			"[ <condition> ].\n");
			customExit();		
		}
	}
	else if(token_id == leftBrk)
	{
		token_id = lex();
		condition(&condTrueList,&condFalseList,currentScope);
		if(token_id == rightBrk)
			token_id = lex();
		else
		{
			printf("%s :Syntax Error:%d:The character ']' was "
			"expected.Found: \" %s \"\n"
			,file_name,currentLineInFile,token);
			printSpaces();
			printf("The proper syntax for conditions is: "
			"[ <condition> ].\n");
			customExit();
		}
		*boolFactorTrueList = condTrueList;
		*boolFactorFalseList = condFalseList;
	}
	else
	{
		expression(E1place,currentScope);
		relationalOper(RELOPplace);
		expression(E2place,currentScope);

		genQuad(RELOPplace,E1place,E2place,"_");
		*boolFactorTrueList = makeList(quadNumber);
		genQuad("jump","_","_","_");
		*boolFactorFalseList = makeList(quadNumber);		
	}
}

void expression(char* Eplace,struct scope* currentScope)
{
	char T1place[TOKEN_SIZE],T2place[TOKEN_SIZE];
	char w[TOKEN_SIZE];
	char ADDplace[TOKEN_SIZE];
	char OPTSIGNplace[TOKEN_SIZE];
	strncpy(OPTSIGNplace,"empty",MAX_TOKEN_CHARACTERS);
	
	optionalSign(OPTSIGNplace);

	term(T1place,currentScope);
	if(strcmp(OPTSIGNplace,"empty")!=0)
	{
		strcat(OPTSIGNplace,T1place);
		strncpy(T1place,OPTSIGNplace,MAX_TOKEN_CHARACTERS);
	}
	while( token_id==plus || token_id == minus )
	{
		addOper(ADDplace);
		term(T2place,currentScope);
		newTemp(w,currentScope);
		genQuad(ADDplace,T1place,T2place,w);
		strncpy(T1place,w,MAX_TOKEN_CHARACTERS);
	}
	strncpy(Eplace,T1place,MAX_TOKEN_CHARACTERS);
}

void term(char* Tplace,struct scope *currentScope)
{
	char F1place[TOKEN_SIZE],F2place[TOKEN_SIZE];
	char MULplace[TOKEN_SIZE],w[TOKEN_SIZE];
	factor(F1place,currentScope);

	while(token_id == star || token_id == slash)
	{
		mulOper(MULplace);
		factor(F2place,currentScope);
		newTemp(w,currentScope);
		genQuad(MULplace,F1place,F2place,w);
		strncpy(F1place,w,MAX_TOKEN_CHARACTERS);
	}
	strncpy(Tplace,F1place,MAX_TOKEN_CHARACTERS);
}

void factor(char* Fplace,struct scope *currentScope)
{
	char Eplace[TOKEN_SIZE];
	char OPTACTPARSplace[TOKEN_SIZE];
	if(token_id == numConst)
	{
		strncpy(Fplace,token,MAX_TOKEN_CHARACTERS);
		token_id = lex();
	}
	else if(token_id == leftPar)
	{	
		token_id = lex();
		expression(Eplace,currentScope);
		strncpy(Fplace,Eplace,MAX_TOKEN_CHARACTERS);
		if(token_id == rightPar)
			token_id = lex();
		else
		{
			printf("%s :Syntax Error:%d:The character ')' was "
			"expected.\n",file_name,currentLineInFile);
			printSpaces();
			printf("The proper syntax is: ( <expression> ).\n");
			customExit();
		}
	}
	else
	{
		if(token_id == identifierTk)
		{
			strncpy(Fplace,token,MAX_TOKEN_CHARACTERS);
			token_id = lex();
			if(optActPars(Fplace,OPTACTPARSplace,currentScope) == 1)
				strncpy(Fplace,OPTACTPARSplace
				,MAX_TOKEN_CHARACTERS);
		}
		else
		{
			printf("%s :Syntax Error:%d:Identifier was expected."
			"Found: \" %s \".\n",file_name,currentLineInFile,token);
			customExit();
		}
	}

}

void whileStat(struct scope *currentScope)
{
	int Bquad=0;
	char repQuad[TOKEN_SIZE];
	struct labelList *condTrueList = NULL, *condFalseList = NULL;
	int condQuad = quadNumber + 1 ;
	condition(&condTrueList,&condFalseList,currentScope);
	if(token_id == doTk)
	{
		Bquad = quadNumber + 1 ;
		sprintf(repQuad,"%d",Bquad);
		backPatch(&condTrueList,Bquad);
	
		token_id = lex();
		blockOrStat(currentScope);
		sprintf(repQuad,"%d",condQuad);
		genQuad("jump","_","_",repQuad);
		backPatch(&condFalseList,quadNumber + 1);

	}
	else
	{
		printf("%s :Syntax Error:%d:The keyword 'do' was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("Proper syntax for \"while\"is: while <condition> do "
		"<block or statement>.\n");
		customExit();
	}
}


void forStat(struct scope *currentScope)
{
	char condQuadS[TOKEN_SIZE],stepQuadSt[TOKEN_SIZE] ;
	char Eplace[TOKEN_SIZE];
	char ASSIGNMENTplace[TOKEN_SIZE];
	char valuePlace[TOKEN_SIZE];
	char STEPplace[TOKEN_SIZE];
	struct labelList * condTrue = NULL;
	struct labelList * condFalse = NULL;
	struct labelList * asc = NULL;
	struct labelList * desc = NULL;
	struct labelList * eq = NULL;
	struct labelList * t = NULL;
	int condQuad = 0;
	int stepQuad = 0;
	
	assignmentStat(ASSIGNMENTplace,valuePlace,currentScope);
	
	if(token_id == toTk){
		token_id = lex();
		expression(Eplace,currentScope);
		
		condQuad = quadNumber + 1;
		genQuad("<", valuePlace ,Eplace, "_");
		asc = makeList(quadNumber);
		genQuad(">", valuePlace ,Eplace, "_");
		desc = makeList(quadNumber);		
		genQuad("=",valuePlace ,Eplace, "_");
		eq = makeList(quadNumber);	
		backPatch(&asc,quadNumber+1);
		genQuad("<=",ASSIGNMENTplace,Eplace,"_");
		condTrue = makeList(quadNumber);
		genQuad("jump","_","_","_");
		condFalse = makeList(quadNumber);
		backPatch(&desc,quadNumber+1);
		genQuad(">=",ASSIGNMENTplace,Eplace,"_");
		t = makeList(quadNumber);
		merge(condTrue,t);	
		genQuad("jump","_","_","_");
		t = makeList(quadNumber);
		merge(condFalse,t);
		backPatch(&eq,quadNumber+1);
		genQuad("=", ASSIGNMENTplace ,Eplace, "_");
		t = makeList(quadNumber);
		merge(condTrue,t);		
		genQuad("jump","_","_","_");
		t = makeList(quadNumber);
		merge(condFalse,t);	
		stepQuad = quadNumber + 1;

		stepPart(STEPplace,currentScope);

		sprintf(condQuadS,"%d",condQuad);
		genQuad("+",ASSIGNMENTplace,STEPplace,ASSIGNMENTplace);
		genQuad("jump","_","_",condQuadS);
		backPatch(&condTrue,quadNumber+1);

		blockOrStat(currentScope);
		sprintf(stepQuadSt,"%d",stepQuad);
		genQuad("jump","_","_",stepQuadSt);
		backPatch(&condFalse,quadNumber+1);
	}
	else{
		printf("%s :Syntax Error:%d:The keyword 'to' was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("Proper syntax for \"for\"is:\n");
		printSpaces();
		printf("for <assignment> to <expression> [step] "
		"<block or statement> .\n");
		customExit();
	}
}

void stepPart(char * STEPplace,struct scope *currentScope)
{
	char Eplace[TOKEN_SIZE];
	strncpy(STEPplace,"1",MAX_TOKEN_CHARACTERS);	
	if(token_id == stepTk)
	{
		token_id = lex();
		expression(Eplace,currentScope);
		strncpy(STEPplace,Eplace,MAX_TOKEN_CHARACTERS);
	}
}

void callStat(struct scope* currentScope)
{
	char idName[TOKEN_SIZE];
	char ACTPARSplace[TOKEN_SIZE];
	if(token_id == identifierTk)
	{
		strncpy(idName,token,MAX_TOKEN_CHARACTERS);
		token_id = lex();
		actualPars(idName,ACTPARSplace,currentScope);
	}
	else
	{
		printf("%s :Syntax Error:%d:Procedure Identifier was expected."
		"Found : \" %s \".\n",file_name,currentLineInFile,token);
		printSpaces();
		printf("Proper syntax for \"call\" is: call ID(params,...).\n");
		customExit();
	}	
}

void printStat(char* PRINTplace,struct scope *currentScope)
{
	char Eplace[TOKEN_SIZE];
	if(token_id == leftPar)
	{
		token_id = lex();
		expression(Eplace,currentScope);
		if(token_id == rightPar)
		{
			genQuad("out",Eplace,"_","_");
			token_id = lex();
		}
		else
		{
			printf("%s :Syntax Error:%d:The character ')' was "
			"expected.\n",file_name,currentLineInFile,token);
			printSpaces();
			printf("Proper syntax for \"print\" command is: "
			"print (<expression>).\n");
			customExit();
		}
	}
	else
	{
		printf("%s :Syntax Error:%d:The character '(' was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("Proper syntax for \"print\" command is: print "
		"(<expression>).\n");
		customExit();
	}
}

void inputStat(char* INPUTplace )
{
	if(token_id == leftPar)
	{
		token_id = lex();
		if(token_id == identifierTk)
		{
			strncpy(INPUTplace,token,MAX_TOKEN_CHARACTERS);
			token_id = lex();
		}
		else
		{
			printf("%s :Syntax Error:%d:Identifier was expected.\n"
			,file_name,currentLineInFile);
			printSpaces();
			printf("The proper syntax for \"input\" command is: "
			"input (ID).\n");
			customExit();
		}			
		if(token_id == rightPar)
		{
			genQuad("inp",INPUTplace,"_","_");
			token_id = lex();
		}
		else
		{
			printf("%s :Syntax Error:%d:The character ')' was "
			"expected.\n",file_name,currentLineInFile);
			printSpaces();
			printf("Proper syntax for \"input\" command is: input"
			" (ID).\n");
			customExit();
		}
	}
	else
	{
		printf("%s :Syntax Error:%d:The character '(' was expected."
		"\n",file_name,currentLineInFile);
		printSpaces();
		printf("Proper syntax for \"input\" command is: input (ID).\n");
		customExit();
	}
}

void assignmentStat(char * ASSIGNMENTplace,char * Eplace,
						struct scope* currentScope)
{
	strncpy(ASSIGNMENTplace,token,MAX_TOKEN_CHARACTERS);
	token_id = lex();
	if(token_id == assignTk)
	{
		token_id = lex();
		expression(Eplace,currentScope);
		genQuad(":=",Eplace,"_",ASSIGNMENTplace);
	}
	else
	{
		printf("%s :Syntax Error:%d:The character ':=' was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("Proper syntax for assignment : Identifier := "
		"<Expression>.Found : \" %s \".\n",token);
		customExit();
	}	
}

void optionalSign(char* OPTSIGNplace)
{
	char ADDplace[TOKEN_SIZE];
	if(token_id == plus || token_id == minus){
		addOper(ADDplace);
		strncpy(OPTSIGNplace,ADDplace,MAX_TOKEN_CHARACTERS);
	}
}

void addOper(char* ADDplace)
{
	if( token_id == plus )
	{
		strncpy(ADDplace,token,MAX_TOKEN_CHARACTERS);
		token_id = lex();
	}
	else if(token_id == minus)
	{
		strncpy(ADDplace,token,MAX_TOKEN_CHARACTERS);
		token_id = lex();
	}
	else
	{
		printf("%s :Syntax Error:%d:Addition operator was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("Addition operators : '+' or '-'.\n");
		customExit();
	}
}

void mulOper(char* MULplace)
{
	if( token_id == star )
	{
		strncpy(MULplace,token,MAX_TOKEN_CHARACTERS);
		token_id = lex();
	}
	else if(token_id == slash)
	{
		strncpy(MULplace,token,MAX_TOKEN_CHARACTERS);
		token_id = lex();
	}
	else
	{
		printf("%s :Syntax Error:%d:Multiplier operator was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("Multiplier operators : '*' or '/'.Found: \" %s \".\n"
		,token);
		customExit();
	}
	
}

void relationalOper(char *RELOPplace)
{
	strncpy(RELOPplace,token,MAX_TOKEN_CHARACTERS);
	if( token_id == comparTk)
		token_id = lex();
	else if( token_id == lessThan)
		token_id = lex();
	else if( token_id == moreThan)
		token_id = lex();
	else if( token_id == lesseqTk)
		token_id = lex();
	else if( token_id == moreeqTk)
		token_id = lex();
	else if( token_id == diffTk)
		token_id = lex();
	else
	{
		printf("%s :Syntax Error:%d:Relational operator was expected.\n"
				,file_name,currentLineInFile);
		printSpaces();
		printf("Relational operators : '=' or '<' or '>' or '>=' or "
		"'<=' or '<>'. Found: \" %s \".\n",token);
		customExit();
	}
}

int optActPars(char *idName,char *OPTACTPARSplace,struct scope *currentScope)
{
	char ACTPARSplace[TOKEN_SIZE];
	if(token_id == leftPar)
	{
		actualPars(idName,ACTPARSplace,currentScope);
		strncpy(OPTACTPARSplace,ACTPARSplace,MAX_TOKEN_CHARACTERS);
		return 1;
	}
	return 0;
}


void checkParametersListWithPrototype(struct parametersList* paramList,
				struct scope* currentScope,char* blockId){
	struct entity* entity = findEntity(blockId,currentScope);
	if(entity == NULL)
		entity = findEntity(blockId,currentScope -> prevScope);
	struct parametersList* prototypeArgList;
	if(entity != NULL){
		prototypeArgList = entity -> data.fun.arguments;
		struct parametersListNode* nodePrototype = 
						prototypeArgList->head;
		struct parametersListNode* nodeCall = paramList->head;
		if( paramList->size > prototypeArgList->size ){
			printf("%s :Syntax Error:%d: In call of \"%s\" function/"
			"procedure has too many arguments.\n",
			file_name,currentLineInFile,blockId);
			customExit();
		}
		else if( paramList->size < prototypeArgList->size ){
			printf("%s :Syntax Error:%d: In call of \"%s\" function/"
			"procedure has too few arguments.\n",
			file_name,currentLineInFile,blockId);
			customExit();
		}
		
		while(nodePrototype != NULL && nodeCall != NULL){
			if( nodePrototype -> type == BYREFERENCE ){
				if(nodeCall -> type == 1){
					printf("%s :Syntax Error:%d: In call of"
					" \"%s\" function/procedure\n"
					,file_name,currentLineInFile,blockId);
					printSpaces();
					printf("parameter \"%s\" is passed by"
					" value but in prototype is declared as"
					" byreference.\n",nodeCall->name);
					customExit();					

				}
			}
			else if( nodePrototype -> type == BYVALUE ){
				if(nodeCall -> type == 2){
					printf("%s :Syntax Error:%d: In call of"
					" \"%s\" function/procedure\n"
					,file_name,currentLineInFile,blockId);
					printSpaces();
					printf("parameter \"%s\" is passed by"
					"reference but in prototype is declared"
					" as by value.\n",nodeCall->name);
					customExit();					
				}
			}
			nodePrototype = nodePrototype -> next;
			nodeCall = nodeCall -> next;
		}
	}
	else{
		printf("%s :Syntax Error:%d: \"%s\" function/procedure"
		" is undeclared.\n",file_name,currentLineInFile,blockId);
		customExit();
	}
}

void actualPars(char *idName,char *ACTPARSplace,struct scope* currentScope)
{
	struct parametersList* paramList;
	allocateParametersList(&paramList);
	char w[TOKEN_SIZE];
	char ACTPARLISTplace[TOKEN_SIZE];
	if( token_id == leftPar){
		token_id = lex();
		if(token_id != rightPar)
			actualParList(idName,ACTPARLISTplace,paramList,
							currentScope);
		if(token_id == rightPar)
		{
			checkParametersListWithPrototype(paramList,currentScope,
									idName);		
			if(paramList->size > 0)
			{
				struct parametersListNode* node;
		
				node = paramList->head;
				while( node != NULL)
				{
					if( node->type == 1)
						genQuad("par",node->name
						,"CV","_");
					else if( node->type == 2)
						genQuad("par",node->name
						,"REF","_");			
					node = (struct parametersListNode *)
						node->next;
				}
				freeParametersList(&paramList);
			}
		
			newTemp(w,currentScope);
			genQuad("par",w,"RET","_");
			genQuad("call",idName,"_","_");
			strncpy(ACTPARSplace,w,MAX_TOKEN_CHARACTERS);			
			token_id = lex();
		}
		else
		{
			printf("%s :Syntax Error:%d:The character ')' was "
			"expected.Found: \" %s \".\n"
			,file_name,currentLineInFile,token);
			customExit();
		}
	}
	else
	{
		printf("%s :Syntax Error:%d:The character '(' was expected."
		"Found: \" %s \".\n",file_name,currentLineInFile,token);
		customExit();
	}
}

void actualParList(char *idName,char * ACTPARLISTplace,
			struct parametersList* paramList,
				struct scope *currentScope)
{

	char ACTPARITEMplace[TOKEN_SIZE];
	actualParItem(idName,ACTPARITEMplace,paramList,currentScope);
	
	while( token_id == comma)
	{
		token_id = lex();
		actualParItem(idName,ACTPARITEMplace,paramList,currentScope);
	}

	strncpy(ACTPARLISTplace,ACTPARITEMplace,MAX_TOKEN_CHARACTERS);
}

void actualParItem(char *idName,char * ACTPARITEMplace,
			struct parametersList* paramList,
				struct scope *currentScope)
{
	char Eplace[TOKEN_SIZE];
	if( token_id == varTk)
	{
		token_id = lex();
		if(token_id == identifierTk)
		{
			addToParametersList(paramList,token,REF);
			strncpy(ACTPARITEMplace,token,MAX_TOKEN_CHARACTERS);
			token_id = lex();
		}
		else
		{
			printf("%s :Syntax Error:%d:Identifier was expected."
			"Found: \" %s \".\n",file_name,currentLineInFile,token);
			customExit();
		}
	}
	else
	{
		expression(Eplace,currentScope);
		addToParametersList(paramList,Eplace,CV);
		strncpy(ACTPARITEMplace,Eplace,MAX_TOKEN_CHARACTERS);	
	}		
}

void declarations(struct scope* currentScope)
{
	while(token_id == constTk)
	{
		token_id = lex();
		constDecl(currentScope);
	}
	while(token_id == varTk)
	{
		token_id = lex();
		varDecl(currentScope);
		if(token_id == constTk)
		{
			printf("%s :Syntax Error:%d:Numerical Constants should"
			" be declared before variables.\n"
			,file_name,currentLineInFile);
			customExit();
		}
	}
}

void constDecl(struct scope* currentScope)
{
	assignList(currentScope);
	if(token_id == semiColon)
		token_id = lex();
	else
	{
		printf("%s :Syntax Error:%d:The character ';' was expected."
		"Found: \" %s \".\n",file_name,currentLineInFile,token);
		customExit();
	}
}

void assignList(struct scope* currentScope)
{
	assignConst(currentScope);
	while(token_id == comma){
		token_id = lex();
		assignConst(currentScope);
	}

}

void assignConst(struct scope* currentScope)
{
	struct entity* constEntity;
	if(token_id == identifierTk)
	{
		constEntity = createNewEntity(token,1,CONSTANT);
		addEntityToScope(constEntity,currentScope);
		token_id = lex();
		if(token_id == assignTk)
		{
			token_id = lex();
			if(token_id == numConst)
			{
				strncpy(constEntity->data.cons.value,token,
							MAX_TOKEN_CHARACTERS);
				token_id = lex();
			}
			else
			{
				printf("%s :Syntax Error:%d:Numerical constant "
				"was expected. Found: \" %s \".\n"
				,file_name,currentLineInFile,token);
				customExit();
			}
		}
		else
		{
			printf("%s :Syntax Error:%d:The character ':=' was "
			"expected.Found: \" %s \".\n"
			,file_name,currentLineInFile,token);
			customExit();
		}
	}
	else 
	{
		printf("%s :Syntax Error:%d:Constant name  was expected.\n"
		,file_name,currentLineInFile);
		customExit();
	}
}

void varDecl(struct scope* currentScope)
{
	varList(currentScope);
	if(token_id == semiColon)
		token_id = lex();
	else
	{
		printf("%s :Syntax Error:%d:The character ';' was expected."
		"Found: \" %s \".\n",file_name,currentLineInFile,token);
		customExit();
	}
}

void varList(struct scope* currentScope)
{
	struct entity* varEntity;
	if(token_id == identifierTk)
	{
		varEntity = createNewEntity(token,1,VARIABLE);
		varEntity->data.var.offset = currentScope -> currentOffset;
		currentScope -> currentOffset += ENTITY_MEM_BYTES;
		addEntityToScope(varEntity,currentScope);
		token_id = lex();
		while(token_id == comma)
		{
			token_id = lex();
			if(token_id == identifierTk)
			{
				varEntity = createNewEntity(token,1,VARIABLE);
				varEntity->data.var.offset = 
						currentScope -> currentOffset;
				currentScope -> currentOffset 
							+= ENTITY_MEM_BYTES;
				addEntityToScope(varEntity,currentScope);
				token_id = lex();
			}
			else
			{
				printf("%s :Syntax Error:%d:Variable name was "
				"expected.\n",file_name,currentLineInFile);
				customExit();
			}
		}	
	}
	else
	{
		printf("%s :Syntax Error:%d:Variable name was expected.\n"
		,file_name,currentLineInFile);
		customExit();
	}
}

void subPrograms(struct scope* currentScope)
{
	struct scope* myScope;

	while(token_id == procedureTk || token_id == functionTk)
	{

		allocateScope(&myScope);
		connectScopes(currentScope,myScope);
		procOrFunc(myScope);
		//printScope(myScope);
		freeScope(&myScope);
	}
}

void procOrFunc(struct scope* currentScope)
{
	char blockId[TOKEN_SIZE];
	struct entity *procOrFuncEntity;
	if(token_id == procedureTk)
	{
		token_id = lex();
		if(token_id == identifierTk)
		{
			procOrFuncEntity = createNewEntity(token,PROCEDURE
							,FUNCTION);
			addEntityToScope(procOrFuncEntity, currentScope 
							-> prevScope);
			strncpy(blockId,token,MAX_TOKEN_CHARACTERS);

			token_id = lex();
			procOrFuncBody(blockId,PROCEDUREBLOCK,currentScope);
			procOrFuncEntity ->data.fun.frameLength = currentScope->
					 currentOffset;

			if(checkIfReturnStatementExists(blockId) == 1){
				printf("%s :Syntax Error:Procedure %s: "
					"has \"return\" statement.\n"
					,file_name,blockId);
				customExit();
			}			
		}
		else
		{
			printf("%s :Syntax Error:%d:Procedure name expected.\n"
			,file_name,currentLineInFile);
			customExit();
		}		
		produceFinalCodeForSubProgram(currentScope,blockId);	
	}
	else if(token_id==functionTk)
	{
		token_id = lex();
		if(token_id == identifierTk)
		{
			procOrFuncEntity = createNewEntity(token,FUNCTION
							,FUNCTION);
			addEntityToScope(procOrFuncEntity, currentScope 
							-> prevScope);
			strncpy(blockId,token,MAX_TOKEN_CHARACTERS);
			token_id = lex();
			procOrFuncBody(blockId,FUNCTIONBLOCK,currentScope);
			procOrFuncEntity ->data.fun.frameLength = currentScope->
					 currentOffset;			
			if(checkIfReturnStatementExists(blockId) == 0){
				printf("%s :Syntax Error:In function %s: "
					"Missing \"return\" statement.\n"
					,file_name,blockId);
				customExit();
			}
		}
		else
		{
			printf("%s :Syntax Error:%d:Function name expected.\n"
			,file_name,currentLineInFile);
			customExit();
		}	
		produceFinalCodeForSubProgram(currentScope,blockId);	
	}
}

void procOrFuncBody(char *blockName,int blockType,struct scope* myScope)
{
	struct parametersList *parametersList;
	struct entity * anEntity;

	allocateParametersList(&parametersList);
	formalPars(myScope,parametersList);
	anEntity = findEntity(blockName,myScope -> prevScope);
	anEntity->data.fun.arguments = parametersList;
	programBlock(blockName,blockType,NULL,myScope);
}

void formalPars(struct scope* myScope,struct parametersList *parametersList)
{ 
	if(token_id == leftPar)
	{
		token_id=lex();
		formalParList(myScope,parametersList);
		if(token_id == rightPar)
			token_id=lex();
		else
		{
			printf("%s :Syntax Error:%d:The character ')' was "
			"expected.\n",file_name,currentLineInFile);
			printSpaces();
			printf("The proper syntax for function parameters"
			" is: (param1,param2,..).\n");
			customExit();
		}
	}
	else
	{
		printf("%s :Syntax Error:%d:The character '(' was expected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("The proper syntax for function parameters is: "
		"(param1,param2,..).\n");
		customExit();
	}	
}

void formalParList(struct scope* myScope,struct parametersList *parametersList)
{
	formalParItem(myScope,parametersList);
	while(token_id == comma)
	{
		token_id = lex();
		formalParItem(myScope,parametersList);
	}
	
}

void formalParItem(struct scope* currentScope,
					struct parametersList *parametersList)
{
	struct entity* parEntity;
	if(token_id == varTk)
	{
		token_id = lex();
		if(token_id == identifierTk)
		{

			parEntity = createNewEntity(token,1,PARAMETER);
			parEntity->data.par.mode = BYREFERENCE;
			parEntity->data.par.offset = currentScope ->
								 currentOffset;
			currentScope -> currentOffset += ENTITY_MEM_BYTES;	
			addEntityToScope(parEntity,currentScope);
			addToParametersList(parametersList,token,BYREFERENCE);
			token_id = lex();
		}
		else
		{
			printf("%s :Syntax Error:%d:Identifier was expected.\n"
			,file_name,currentLineInFile);
			printSpaces();
			printf("Proper syntax for passing parameter with the "
			"keyword \"var\" is: var ID.\n");
			customExit();
		}
	}
	else if(token_id == identifierTk)
	{

		parEntity = createNewEntity(token,1,PARAMETER);
		parEntity->data.par.mode = BYVALUE;
		parEntity->data.par.offset = currentScope -> currentOffset;
		currentScope -> currentOffset += ENTITY_MEM_BYTES;
		addEntityToScope(parEntity,currentScope);	
		addToParametersList(parametersList,token,BYVALUE);
		token_id = lex();
	}
}

#endif
