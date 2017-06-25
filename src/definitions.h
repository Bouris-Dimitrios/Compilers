
#ifndef __DEFINITIONS_H___
#define __DEFINITIONS_H___

/**
*@file definitions.h
*\authors{ Bouris Dimitris 1894 (bourisdim@gmail.com) ,
 Nikos Ginargyros 2038 (nikospaxos@hotmail.com) }
*@date  March ,2014

*@brief  Defines and declaration of global variables.\n
*This file contains defines for:\n
*the states needed from *the lex automato,
*symbols,keywords,tokens.\n
*All the global variables are 
*declared here.
*/



#define EOFCODE (-3)


/**The starting state of automato.**/
#define state0 0

/** <Input was a letter **/
#define state1 1 

/** Input was a digit **/
#define state2 2

/** Input was a '+' **/
#define state3 3 

/** Input was a '-' **/
#define state4 4

/** Input was a '/' **/
#define state5 5

/** We are in a comment block and all characters are ignored. **/
#define state6 6

/** We are in comment state and '*' came as token. **/
#define state7 7

/** Input was a '=' **/
#define state8 8

/** Input was a '<' **/
#define state9 9

/** Input was a '>' **/
#define state10 10

/** Input was a ':' **/
#define state11 11

/** Input was a '*' **/
#define state12 12

/** In case the absolute value of a numerical constant  > (2^31)-1 **/
#define numberOverFlow 13


#define error -1
/** okState Everything is ok.Use ungetc()to return next character to file.**/

#define okState    -2

/*okNoUnget Everything is ok.Dont use ungetc(). Keep the character.*/
#define okNoUnget -3
#define warning -4


/** {A-Z} , {a-z} **/
#define letter     0
/** {0-9} **/
#define digit      1

/**symbols**/
#define plus       2 
#define minus      3
#define star       4
#define slash      5
#define equal      6
#define lessThan   7
#define moreThan   8
#define colon      9
#define comma     10
#define semiColon 11
#define leftPar   12
#define rightPar  13
#define leftBrk   14
#define rightBrk  15
#define whiteSpace 16

#define eoff      17
#define other     18


#define rows 13 
#define columns   19 

/**keywords*/
#define programTk   19
#define functionTk  20
#define procedureTk 21
#define returnTk    22
#define callTk      23
#define beginTk     24
#define endTk       25
#define ifTk        26
#define thenTk      27
#define elseTk	    28
#define doTk        29
#define whileTk     30
#define forTk       31 
#define toTk  	    32
#define stepTk 	    33
#define varTk       34
#define constTk     35
#define inputTk     36
#define printTk     37
#define andTk       38
#define orTk        39
#define notTk       40
#define selectTk    41
#define endSelectTk 42
#define isTk        43
#define equalKeyTk  53    

#define identifierTk 44
#define numConst    45

#define moreeqTk    46   
#define lesseqTk    47
#define comparTk    48
#define assignTk    49
#define diffTk      50
#define pluseqTk      51
#define minuseqTk     52

#define BUFFER_SIZE 35
#define TOKEN_SIZE 31
#define INPUT_FILENAME_SIZE 512
#define MAX_TOKEN_CHARACTERS 30
#define MAX_QUADS 30

#define FALSE 0
#define TRUE 1


#define PROGRAMBLOCK 0
#define FUNCTIONBLOCK 1
#define PROCEDUREBLOCK 2


#define CV  1
#define REF 2

#define VARIABLE 1
#define FUNCTION 2
#define CONSTANT 3
#define PARAMETER 4
#define TEMPVARIABLE 5

#define BYREFERENCE 1
#define BYVALUE 2

#define PROCEDURE 2

#define ENTITY_MEM_BYTES 4
#include <stdlib.h>
#include "listStructures.h"

int token_id; //!<Contains the id of the token read from lexical analyzer;
char *token; //!<A string that contains a token read from lectical analyzer.

int currentLineInFile; //!<A counter for the lines of the file to be compiled. 
		//!<It is used keeping  the line of an error.


char file_name[INPUT_FILENAME_SIZE]; //!<The name of the file that is going to be compiled.
char finalCodeFileName[INPUT_FILENAME_SIZE];
int countTempVariables;
int fileNameLength;
int trans_diag[rows][columns]; //!<An array of integeres that implements the
			       //!<automaton mechanismm. Every row is a state ,
			       //!<every column an input token(symbols,keywords,etc)

FILE *sourceFile,*finalCode;             //!<The program to be compiled.
int quadNumber;

struct list *programList;
struct listNode* compileCounter;


#endif
