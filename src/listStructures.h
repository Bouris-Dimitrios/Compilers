#ifndef __LISTSTRUCTURES_H__
#define __LISTSTRUCTURES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"

struct listNode
{
	char quadNumber[MAX_QUADS];
	char op[TOKEN_SIZE];
	char x[TOKEN_SIZE];
	char y[TOKEN_SIZE];
	char z[TOKEN_SIZE];
	struct listNode * next; 

};

struct list
{
	struct listNode * head;
	struct listNode * tail;
	int size;

};


struct labelListNode
{
	struct listNode* data;
	struct LabelListNode *next;
};

struct labelList
{
	struct labelListNode * head;
	struct labelListNode * tail;
	int size;
};


struct parametersListNode
{
	char name[TOKEN_SIZE];
	int type;
	struct parametersListNode *next;
};

struct parametersList
{
	struct parametersListNode * head;
	struct parametersListNode * tail;
	int size;
};

void allocateParametersList(struct parametersList** outList);
void addToParametersList(struct parametersList* aList,char* varName,int type);
void freeParametersList(struct parametersList **aList);
int checkIfReturnStatementExists(char * blockId);
void allocateList(struct list** outList);
void allocateLabelList(struct labelList** outList);
void addToList(struct list* aList,int quadId,char* op,char* x,char* y,
		 char* z);
void addNodeToLabelList(struct list * programList ,struct labelList* aList,int x);
int isEmpty(struct list* aList);
void mergeLists(struct labelList* list1,struct labelList* list2);
void printList(struct list* aList);
void printLabelList(struct labelList* aList);
void freeList(struct list **alist  );
void freeLabelList(struct labelList **aList);

#endif
