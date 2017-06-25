#ifndef __RULES_H__ 
#define __RULES_H__
/**
*@file rules.h
*\authors{ Bouris Dimitris 1894 (bourisdim@gmail.com) ,
 Nikos Ginargyros 2038 (nikospaxos@hotmail.com) }
*@date  March ,2014
*@brief Contains prototypes for grammar rules of Mini Pascal.
**/ 

#include<stdio.h>
#include<stdlib.h>
#include"definitions.h"
#include "functionsLib.h"
#include "symbolTableStructures.h"

void program();
void programBlock(char * blockId,int isMainBlock, struct labelList *startList,
						 struct scope* currentScope);
void block(struct scope* currentScope);
void declarations(struct scope* currentScope);
void constDecl(struct scope* currentScope);
void assignList(struct scope* currentScope);
void assignConst(struct scope* currentScope);
void varDecl(struct scope* currentScope);
void varList(struct scope* currentScope);
void subPrograms(struct scope* currentScope);
void procOrFunc(struct scope* currentScope);
void procOrFuncBody(char *blockName,int blockType,struct scope* myScope);
void formalPars(struct scope* myScope,struct parametersList *parametersList);
void formalParList(struct scope* myScope,struct parametersList *parametersList);
void sequence(struct scope* currentScope);
void blockOrStat(struct scope *currentScope);
void statement(struct scope* currentScope);
void assignmentStat(char * ASSIGNMENTplace,char * Eplace,
						struct scope* currentScope);
void ifStat(struct scope *currentScope);
void elsePart(struct scope *currentScope);
void whileStat(struct scope *currentScope);
void forStat(struct scope *currentScope);
void stepPart(char * STEPplace,struct scope *currentScope);
void callStat(struct scope* currentScope);
void printStat(char* PRINTplace,struct scope *currentScope);
void inputStat(char* INPUTplace );
void returnStat(struct scope* currentScope);
void actualPars(char *idName,char *ACTPARSplace,struct scope* currentScope);
void actualParItem(char *idName,char * ACTPARITEMplace
			,struct parametersList* paramList,struct scope *currentScope);
void actualParList(char *idName,char * ACTPARLISTplace
			,struct parametersList* paramList,struct scope *currentScope);
void condition(struct labelList ** condTrueList,struct labelList ** condFalseList,struct scope *currentScope);
void boolFactor(struct labelList **boolFactorTrueList,
		struct labelList **boolFactorFalseList,struct scope *currentScope);
void boolTerm(struct labelList **boolTermTrueList,
		struct labelList **boolTermFalseList,struct scope *currentScope);
void factor(char* Fplace,struct scope *currentScope);
void relationalOper(char *RELOPplace);
void expression(char* Eplace,struct scope* currentScope);
void term(char* Tplace,struct scope *currentScope);
int optActPars(char *idName,char *OPTACTPARSplace,struct scope *currentScope);
void addOper(char* ADDplace);
void mulOper(char* MULplace);
void optionalSign(char* OPTSIGNplace);
void formalParItem(struct scope* currentScope,struct parametersList *parametersList);
void selectConStat(char* Eplace1,struct labelList *jumpOutList, 
						struct scope *currentScope);
void selectStat(struct scope *currentScope);

#endif
