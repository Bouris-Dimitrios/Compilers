#ifndef __FINALCODE__H 
#define __FINALCODE__H

#include "definitions.h"
#include "symbolTableStructures.h"
#include "functionsLib.h"

void openFinalCodeFile();
void gnlvCode(char * variableName,struct scope * currentScope);
void loadvr(char * variableName,int regNum,struct scope * currentScope);
void storerv(int regNum,char* variableName,struct scope* currentScope);
int isNumber(char * variableName,int regNum);
void produceFinalCodeForSubProgram(struct scope* currentScope,char* blockId);
int isConst(char* constName,int regNum,struct scope* currentScope);
int isRelop(char* op);
int isOperator(char* operator);
void produceRelopFinalCode(struct scope* currentScope);
void produceOperatorFinalCode(struct scope* currentScope);
void produceReturnValueFinalCode(struct scope* currentScope);
void produceParameterPassingCvFinalCode(struct scope* currentScope, int dValue);
void produceParameterPassingReferenceFinalCode(struct scope* currentScope, 
								int dValue);
void produceParameterPassingReturnFinalCode(struct scope* currentScope, int frameLength);
void produceCallFunctionFinalCode(struct scope* currentScope,char* blockId);

void produceAssignmentFinalCode(struct scope* currentScope);
void produceOutFinalCode(struct scope* currentScope);
void produceInFinalCode(struct scope* currentScope);
void produceJumpFinalCode();
void produceHaltFinalCode();
void produceEndBlockFinalCode(struct scope* currentScope,char* blockId);
void printLabelForFinalCodeBlock();
void produceFinalCodeForSubProgram(struct scope* currentScope,char* blockId );
#endif
