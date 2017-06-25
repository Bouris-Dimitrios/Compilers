#ifndef __SYNTAXANALYZER_C__
#define __SYNTAXANALYZER_C__
/**
*@file syntaxAnalyzer.c
*\authors{ Bouris Dimitris 1894 (bourisdim@gmail.com) ,
 Nikos Ginargyros 2038 (nikospaxos@hotmail.com) }
*@date  March ,2014
*@brief  This file contains the syntax analyze function. **/
#include "functionsLib.h"
#include "rules.h"

/** @brief Responsible  to recognize if the syntax of input file
*is acceptable by miniPascal standards.\n
*Lexical analyzer is called to get the first token of input program 
*and calls the rule proram().\n
*Also closes the input file .
**/


int syntaxAnalyzer()
{
	token_id = lex();
	program();	

	freeList(&programList);	
	free(token);
	fclose(sourceFile);
	fclose(finalCode);
	return 1;
}

#endif
