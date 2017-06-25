#ifndef __SYMBOLTABLESTRUCTURES_H__
#define __SYMBOLTABLESTRUCTURES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listStructures.h"



struct scope
{	
	int nestingLevel;
	int currentOffset;
	struct scope *nextScope;
	struct scope *prevScope;
	struct entity *head;
};

struct variable
{
	int offset;
};

struct function
{
	char startQuad[TOKEN_SIZE];
	struct parametersList* arguments;
	int frameLength;
};

struct constant
{
	char value[TOKEN_SIZE];
};

struct parameter
{
	int mode;
	int offset;
};

struct tempVariable
{
	int offset;
};


struct entity
{
	char name[TOKEN_SIZE];
	int type;
	int typeOfEntity;
	union entityData
	{
		struct variable var;
		struct function fun;
		struct constant cons;
		struct parameter par;
		struct tempVariable tmpVar;

	}data;
	struct entity* next;
};

void connectScopes(struct scope* currentScope, struct scope* nextScope);
void addEntityToScope(struct entity *anEntity, struct scope *aScope );
void printScope(struct scope *aScope);
void allocateScope(struct scope** outScope);
void freeScope(struct scope **aScope);
struct entity* createNewEntity(char* name,int type,int typeOfEntity);
struct entity* findEntity(char * blockId,struct scope * scope);
int findVariableInScope(char * variableName,struct scope * aScope);
int findGlobalVariable(char * variableName,struct scope * aScope);
int findLocalOrTemporaryOrParameter(char * variableName,struct scope *aScope);
int findByReferenceParameter(char *variableName, struct scope *aScope);
int findConstantInScope(char * constant,struct scope * aScope);
int findEntityAndReturnNestingLevel(char * variableName,struct scope * aScope);
struct entity* findEntityInSymbolTable(char * blockId,struct scope * currentScope);

#endif
