#ifndef __FUNCTIONSLIB_H__
#define __FUNCTIONSLIB_H__
/**
*@file functionsLib.h
*\authors{ Bouris Dimitris 1894 (bourisdim@gmail.com) ,
 Nikos Ginargyros 2038 (nikospaxos@hotmail.com) }
*@date  March ,2014
*@brief Contains prototypes for basic functions.
**/ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "definitions.h"
#include "listStructures.h"
#include "symbolTableStructures.h"
#include "finalCodeFunctions.h"
#include <math.h>

void initializeParameters(char* programFileName);
void initializeLexAutomaton();
int syntaxAnalyzer();
int lex();

int nextQuad();
void newTemp(char* tempName,struct scope * scope);
void genQuad(char* op,char* x,char* y, char* z);
struct labelList * emptyList();
struct labelList * makeList(int x);
void merge(struct labelList *list1,struct labelList *list2);
void backPatch(struct labelList **aLabelList,int z);
void customExit();


#endif
