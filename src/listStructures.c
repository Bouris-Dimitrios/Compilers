#ifndef __LISTSTRUCTURES_C__
#define __LISTSTRUCTURES_C__


#include "listStructures.h"


void allocateList(struct list** outList)
{
	struct list* aList = malloc(sizeof(struct list));
	if( aList == NULL )
	{
		printf("listStructures.h : Fatal Error : Cannot allocate"
		" memory.\nProgram will exit.\n");	
		exit(-6);
	}
	aList -> head = NULL;
	aList -> tail = NULL;
	aList -> size = 0;
	*outList = aList;
}

void allocateLabelList(struct labelList** outList)
{
	struct labelList* aList = malloc(sizeof(struct labelList));
	if( aList == NULL )
	{
		printf("listStructures.h : Fatal Error : Cannot allocate"
		" memory.\nProgram will exit.\n");	
		exit(-6);
	}
	aList -> head = NULL;
	aList -> tail = NULL;
	aList -> size = 0;
	*outList = aList;
}

void allocateParametersList(struct parametersList** outList)
{
	struct parametersList* aList = malloc(sizeof(struct parametersList));
	if( aList == NULL )
	{
		printf("listStructures.h : Fatal Error : Cannot allocate"
		" memory.\nProgram will exit.\n");	
		exit(-6);
	}
	aList -> head = NULL;
	aList -> tail = NULL;
	aList -> size = 0;
	*outList = aList;
}


int isEmpty(struct list* aList)
{
	if(aList == NULL)
	{
		printf("The list doesnt exist.\n");
		return	;
	}		
	if(aList->head == NULL)
		return 1;
	return 0;
}

int checkIfReturnStatementExists(char * blockId){
	struct listNode* aNode,*functionNode;
	aNode = programList-> head;
	while(aNode != NULL){
		if(strcmp(aNode -> op, "begin_block") == 0 && 
				strcmp(aNode->x,blockId)==0){
			break;
		}
						

		aNode = aNode -> next;	
	}

	while(aNode != NULL){
		if(strcmp(aNode -> op, "end_block") == 0 && 
			strcmp(aNode->x,blockId)==0)
			return 0;
		if(strcmp(aNode -> op, "retv") == 0)
			return 1;
		aNode = aNode -> next;
	}
	return 0;	
}

void mergeLists(struct labelList* list1,struct labelList* list2)
{
	if(list1 == NULL || list2 == NULL)
	{
		printf("A list doesnt exist.\n");
		return	;
	}
	if(list2->head == NULL)
		return;
	else if(list1 -> head == NULL)
	{
		list1 -> head = list2 -> head;
		list1 -> tail = list2 -> tail;
		list1 -> size = list2 -> size;
		return;
	}
	struct labelListNode ** mergePoint;
	mergePoint = (struct labelListNode**) &(list1 -> tail )-> next;
	(*mergePoint ) = (struct labelListNode*) list2 -> head;
	list1 -> tail = list2 -> tail;
	list1 -> size += list2 -> size;
}

void addToList(struct list* aList,int quadId,char* op,char* x,char* y,
		 char* z)
{

	struct listNode* newNode = malloc(sizeof(struct listNode));
	if(aList == NULL)
	{
		printf("The list doesnt exist.\n");
		return	;
	}
	if( newNode == NULL )
	{
		printf("listStructures.h : Fatal Error : Cannot allocate"
		" memory.\nProgram will exit.\n");	
		exit(-7);
	}

	sprintf(newNode->quadNumber,"%d",quadId);
	strncpy(newNode->op,op,MAX_TOKEN_CHARACTERS);
	strncpy(newNode->x,x,MAX_TOKEN_CHARACTERS);
	strncpy(newNode->y,y,MAX_TOKEN_CHARACTERS);
	strncpy(newNode->z,z,MAX_TOKEN_CHARACTERS);	
	newNode -> next = NULL;

	if( aList->head == NULL )
	{
		aList -> head = newNode;
		aList -> tail = newNode;
	}
	else
	{
		aList->tail->next = newNode;
		aList->tail = newNode;
	}
	aList ->size++;
}


void addToParametersList(struct parametersList* aList,char* varName,int type)
{
	struct parametersListNode* newNode;
	newNode = malloc(sizeof(struct parametersListNode));
	if(aList == NULL)
	{
		printf("The list doesnt exist.\n");
		return	;
	}
	if( newNode == NULL )
	{
		printf("listStructures.h : Fatal Error : Cannot allocate"
		" memory.\nProgram will exit.\n");	
		exit(-7);
	}

	strncpy(newNode->name,varName,MAX_TOKEN_CHARACTERS);
	newNode->type = type;

	newNode -> next = NULL;

	if( aList->head == NULL )
	{
		aList -> head = newNode;
		aList -> tail = newNode;
	}
	else
	{
		aList->tail->next = newNode;
		aList->tail = newNode;
	}

	aList ->size++;	
}

void addNodeToLabelList(struct list * programList ,struct labelList* aList,int x)
{
	struct labelListNode *nodeToAdd=malloc(sizeof(struct labelListNode));
	struct listNode* node;
	char point[TOKEN_SIZE];
	sprintf(point,"%d",x);
	nodeToAdd -> data = NULL;

	if(aList == NULL || programList == NULL)
		return	;
	if(nodeToAdd == NULL)
	{
		printf("listStructures.h : Fatal Error : Cannot allocate"
		" memory.\nProgram will exit.\n");	
		exit(-6);
	}
	node = (struct listNode *) programList->head;
	while( node != NULL)
	{
		if(strcmp(node -> quadNumber,point) == 0)
		{
			nodeToAdd -> data = node;
			break;			
		} 
		node = node->next;
	}

	if(nodeToAdd -> data == NULL)
		return;
	nodeToAdd -> next = NULL;

	if( aList->head == NULL )
	{
		aList -> head = nodeToAdd;
		aList -> tail = nodeToAdd;
	}
	else
	{
		struct labelListNode** addPoint; 
		addPoint = (struct labelListNode**) &(aList->tail)->next;
		*addPoint = nodeToAdd;
		aList->tail = nodeToAdd;
	}

	aList ->size++;	
}

void printList(struct list* aList)
{
	struct listNode* node;
	if(aList == NULL)
	{
		printf("The list doesnt exist.\n");
		return	;
	}
	if(aList->head==NULL)
	{
		printf("List is empty.\n");
		return;
	}

	node = aList->head;
	while( node != NULL)
	{
		printf("Quad: %s | %s , %s , %s , %s\n",
		node->quadNumber,node->op,node->x,node->y,node->z);
		node = node->next;
	}
}

void printParametersList(struct parametersList* aList)
{
	struct parametersListNode* node;
	if(aList == NULL)
	{
		printf(" -There are no Parameters.\n");
		return	;
	}
	if(aList->head==NULL)
	{
		printf(" -There are no Parameters.\n");
		return;
	}

	node = aList->head;
	while( node != NULL)
	{
		printf("\tname: %s,type: ",node->name);
		if(node -> type == 1)
			printf("Reference.\n");
		else
			printf("Value.\n");
			
		node = node->next;
	}
}

void printLabelList(struct labelList* aList)
{
	struct labelListNode* node;

	if(aList == NULL || aList->head==NULL)
	{
		printf("Empty List. \n");
		return	;	
	}
	node = aList->head;
	while( node != NULL)
	{
		printf("Quad: id: %s | op : %s | x: %s | y: %s | z: %s .\n"
		,node->data->quadNumber,node->data->op,node->data->x,
		node->data->y,node->data->z);
		node = (struct labelListNode *) node->next;
	}
}

void freeParametersList(struct parametersList **aList)
{
	struct parametersListNode *node ,*temp;
	if(*aList == NULL)
	{
		printf("The list doesnt exist.\n");
		return	;
	}
	if((*aList)-> head == NULL)
	{
		free(*aList);
		return;
	}

	node = (*aList)->head;
	while(node != NULL)
	{
		temp = node;
		node = node->next;
		free (temp);	
	}

	free (*aList);
	*aList = NULL;	


}

void freeList(struct list ** aList)
{

	struct listNode *node ,*temp;
	if(*aList == NULL)
	{
		printf("The list doesnt exist.\n");
		return	;
	}
	if((*aList)-> head == NULL)
	{
		free(*aList); 
		return;
	}

	node = (*aList)->head;
	while(node != NULL)
	{
		temp = node;
		node = node->next;
		free (temp);	
	}

	free (*aList);
	*aList = NULL;	
}

void freeLabelList(struct labelList **aList)
{
	struct labelListNode *node ,*temp;
	if(*aList == NULL)
	{
		printf("The list doesnt exist.\n");
		return	;
	}
	if((*aList)-> head == NULL)
	{
		free(*aList);
		return;
	}

	node = (*aList)->head;
	while(node != NULL)
	{
		temp = node;
		node = (struct labelListNode*) node->next;
		free (temp);	
	}

	free (*aList);
	*aList = NULL;	
}
#endif
