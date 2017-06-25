#ifndef __INTERMEDIATECODEFUNCS_C__
#define __INTERMEDIATECODEFUNCS_C__

#include "functionsLib.h"

int nextQuad()
{
	quadNumber++;
	return quadNumber;
}

void genQuad(char* op,char* x,char* y, char* z)
{
	addToList(programList,nextQuad(),op,x,y,z);
}

void newTemp(char* tempName,struct scope * scope)
{
	struct entity *anEntity; 
	sprintf(tempName,"T_%d",countTempVariables);
	countTempVariables++;	
	anEntity = createNewEntity(tempName,1,TEMPVARIABLE);
	anEntity->data.tmpVar.offset = scope -> currentOffset;
	scope -> currentOffset += ENTITY_MEM_BYTES;
	addEntityToScope(anEntity,scope);
}

struct labelList * emptyList()
{
	struct labelList * newLabelList;
	allocateLabelList(&newLabelList);
	return newLabelList;
}


struct labelList* makeList(int x)
{
	struct labelList * newLabelList;
	allocateLabelList(&newLabelList);
	addNodeToLabelList(programList,newLabelList,x);
	return newLabelList;
}

void merge(struct labelList *list1,struct labelList *list2)
{
	mergeLists(list1,list2);
}

void backPatch(struct labelList **aLabelList,int z)
{
	struct labelListNode *node;

	char convertZtoChar[TOKEN_SIZE];
	sprintf(convertZtoChar,"%d",z);
	if( (*aLabelList)== NULL)
	{
		printf("The list doesnt exist.\n");
		return	;
	}
	if( (*aLabelList)->head==NULL)
	{
		printf("List is empty.\n");
		return; 
	}
	
	node = (*aLabelList)-> head;
	while(node != NULL)
	{
		strncpy(node->data->z,convertZtoChar,MAX_TOKEN_CHARACTERS);
		node = (struct labelListNode*) node -> next;
	}

	freeLabelList(aLabelList);
}


#endif
