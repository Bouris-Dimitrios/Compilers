#ifndef __SYMBOLTABLESTRUCTURES_C__
#define __SYMBOLTABLESTRUCTURES_C__

#include "symbolTableStructures.h"

int findDataInScope(char * variableName,struct scope * aScope){
	
	if( aScope != NULL)
	{
		struct entity * node;
		if( aScope -> head != NULL)
		{
			node = aScope->head;	
			while(node != NULL)	
			{
				if( strcmp(node ->name,variableName) == 0 ){
					if( node -> typeOfEntity == VARIABLE)
						return node->data.var.offset;
					if( node -> typeOfEntity == PARAMETER)
						return node->data.par.offset;
				}
				node = node->next;	
			}
		}
	}
	return 0;
}

int findLocalOrTemporaryOrParameter(char * variableName,struct scope *aScope){

	if( aScope != NULL)
	{
		struct entity * node;
		if( aScope -> head != NULL)
		{
			node = aScope->head;	
			while(node != NULL)	
			{
				if(strcmp(node -> name ,variableName) == 0 ){

					if(node->typeOfEntity == VARIABLE)
						return node->data.var.offset;
					else if(node->typeOfEntity == 
								   PARAMETER ){
						if(node->data.par.mode == 
									BYVALUE)
							return node->data.
								     par.offset;
					}
					else if(node -> typeOfEntity == TEMPVARIABLE)
						return node->data.tmpVar.offset;
				}
				node = node->next;	
			}
		}
	}
	return 0;
}

int findByReferenceParameter(char *variableName, struct scope *aScope){

	if( aScope != NULL)
	{
		struct entity * node;
		if( aScope -> head != NULL)
		{
			node = aScope->head;	
			while(node != NULL)	
			{
				if(strcmp(node ->name,variableName) == 0 ){
					if(node->data.par.mode == BYREFERENCE)
						return node->data.par.offset;
				}
				node = node->next;	
			}
		}
	}
	return 0;
}


int findEntityAndReturnNestingLevel(char * entityName,struct scope * aScope){
	
	if(aScope != NULL)
	{
		struct scope* node;
		node = aScope;
		while(node != NULL)
		{
			if(findEntity(entityName,node)!= NULL)
				return node->nestingLevel;
			node = node -> prevScope;
		}
	}
	return -1;
}

int findVariableInScope(char * variableName,struct scope * aScope){
	
	if( aScope != NULL)
	{
		struct entity * node;
		if( aScope -> head != NULL)
		{
			node = aScope->head;	
			while(node != NULL)	
			{
				if(node->typeOfEntity == VARIABLE && 
					strcmp(node ->name,variableName) == 0 )
					return node->data.var.offset;
				node = node->next;	
			}
		}
	}
	return 0;
}

int findGlobalVariable(char * variableName,struct scope * aScope){
	
	if(aScope != NULL)
	{
		struct scope* node;
		node = aScope;
		while(node != NULL)
		{
			if(node -> prevScope == NULL)
				return findVariableInScope(variableName,node);
				
			node = node -> prevScope;
		}
	}
	return 0;
}


int findConstantInScope(char * constant,struct scope * aScope){
	
	if( aScope != NULL && constant != NULL)
	{
		struct entity * node;
		if( aScope -> head != NULL)
		{
			node = aScope->head;
			
			while(node != NULL)	
			{	
				if(node->typeOfEntity == CONSTANT) {
					if(strcmp(node ->name,constant) == 0 )
						return atoi(node->data.cons.value);
				}
				node = node->next;	
			}
		}
	}
	return -1;
}

int findGlobalConstant(char * constant,struct scope * aScope){
	
	if(aScope != NULL)
	{
		struct scope* node;
		node = aScope;
		while(node != NULL)
		{
			if(node -> prevScope == NULL)
				return findConstantInScope(constant,node);
			node = node -> prevScope;
		}
	}
	return -1;
}

void printScopes(struct scope* mainScope)
{
	if(mainScope != NULL)
	{
		struct scope* node;
		node = mainScope;
		while(node != NULL)
		{
			printScope(node);
			node = node -> nextScope;
		}
	}
}

void connectScopes(struct scope* lowLevelScope, struct scope* highLevelScope)
{
	if(lowLevelScope != NULL && highLevelScope != NULL)
	{
		lowLevelScope -> nextScope = highLevelScope;
		highLevelScope -> prevScope = lowLevelScope; 
		highLevelScope -> nestingLevel =
					 lowLevelScope -> nestingLevel + 1 ;
	}
}

void addEntityToScope(struct entity *anEntity, struct scope *aScope )
{
	struct entity * node ;
	if( aScope != NULL)
	{
		if( aScope -> head != NULL)
		{
			node = aScope->head;	
			while(node != NULL){
				if(node -> next == NULL){
					node->next = anEntity;
					return;
				}
				node = node->next;

			}	
		}
		else
			aScope->head = anEntity;
	}

}

void printScope(struct scope *aScope)
{
	if( aScope != NULL)
	{
		printf("---- SCOPE Level : %d.\n",aScope->nestingLevel);
		struct entity * node;
		if( aScope -> head != NULL)
		{
			node = aScope->head;	
			while(node != NULL)	
			{
				if(node->typeOfEntity == CONSTANT)
					printf("Const: %s = %s\n", node -> name,
						node->data.cons.value);
				if(node->typeOfEntity == VARIABLE)
					printf("Var: %s offset: %d\n",
					 node -> name,node->data.var.offset);
				if(node->typeOfEntity == PARAMETER){
					printf("Par: %s offset: %d mode: ",
					 node -> name,node->data.par.offset);
					if(node -> data.par.mode == BYREFERENCE)
						printf("Reference\n");
					else
						printf("Value\n");
				}
				if(node->typeOfEntity == FUNCTION){
					if(node->type == PROCEDURE)
						printf("Procedure:");
					else
						printf("Function:");
					printf(" %s Frame Length: %d Start quad"
					 ":%s \nParameters:\n", 
					node -> name,node ->data.fun.frameLength
					,node->data.fun.startQuad);
					printParametersList(
						node->data.fun.arguments);
				}
				if(node->typeOfEntity == TEMPVARIABLE)
					printf("TEMPNAME: %s offset: %d \n", 
					node -> name,node ->data.tmpVar.offset);
				node = node->next;	
			}
			printf("----End of scope level : %d \n",
				aScope -> nestingLevel);
		}
	}
	else
		printf("Scope doesn't exist\n");
	printf("\n\n");
}

void allocateScope(struct scope** outScope)
{
	struct scope* aScope = malloc(sizeof(struct scope));
	if( aScope == NULL )
	{
		printf("Symbol table structures : Fatal Error : Cannot allocate"
		" memory.\nProgram will exit.\n");	
		exit(-6);
	}
	aScope -> nextScope = NULL;
	aScope -> prevScope = NULL;
	aScope -> head = NULL;
	aScope -> nestingLevel = 0;
	aScope -> currentOffset = 12;
	*outScope = aScope;

}

void freeScope(struct scope **aScope)
{
	struct entity *node ,*temp;
	if(*aScope == NULL)
	{
		printf("The scope doesnt exist.\n");
		return	;
	}
	if((*aScope)-> head == NULL)
	{
		free(*aScope);
		return;
	}

	node = (*aScope)->head;
	while(node != NULL)
	{
		temp = (node);
		node = node->next;
		free(temp); 	
	}
	free(*aScope);
	*aScope = NULL;	
}


struct entity* createNewEntity(char* name,int type,int typeOfEntity)
{

	struct entity* newEnt;
	newEnt = malloc(sizeof(struct entity));
	strncpy(newEnt->name,name,MAX_TOKEN_CHARACTERS);
	newEnt->type = type;
	newEnt->typeOfEntity = typeOfEntity;
	newEnt->next = NULL;
	return newEnt;
}

struct entity* findEntity(char * blockId,struct scope * scope)
{
	
	struct entity * anEntity;
	if(scope != NULL){
		anEntity = scope -> head;
	
		while(anEntity != NULL)
		{
			if(strcmp(anEntity->name , blockId) == 0)
				return anEntity;
			anEntity = anEntity -> next;
		}
	}
	return NULL;
}

struct entity* findEntityInSymbolTable(char * blockId,struct scope * 
								currentScope)
{
	struct entity * anEntity;
	struct scope* scope = currentScope;
		
	while( scope != NULL ){
		if(scope != NULL){
			anEntity = scope -> head;
	
			while(anEntity != NULL)
			{
				if(strcmp(anEntity->name , blockId) == 0)
					return anEntity;
				anEntity = anEntity -> next;
			}
		}
		scope = scope->prevScope;
	}
	return NULL;
}

#endif
