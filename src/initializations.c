#ifndef __INITIALIZATIONS_C__
#define __INITIALIZATIONS_C__
/**
*@file initializations.c
*\authors{ Bouris Dimitris 1894 (bourisdim@gmail.com) ,
 Nikos Ginargyros 2038 (nikospaxos@hotmail.com) }
*@date  March ,2014
*@brief Impelements the functions that initialize basic parameters\n 
* and the automaton used by lexical analyzer.
**/

#include "functionsLib.h"

/**@brief The lexical automaton is beeing initialized.\n
*To implement the automaton a two dimension array is used \n
*named trans_diag.The rows are the states and columns \n
*are the input tokens(keywords,symbols,numerical constants etc.).\n
*For every combination of state and input there is  a sequence of \n
* states that end up in a final state.This final state can be \n
* okstate,okNoUnget and Error.
**/
void initializeLexAutomaton()
{
	int i=0,j=0;
	//FILL TRANS DIAGRAM TABLE
	//OTHER
	for(j=0;j<rows;j++)
		trans_diag[j][other]=error;
	//state0
	trans_diag[state0][letter] = state1;
	trans_diag[state0][digit] = state2;
	trans_diag[state0][plus] = state3;
	trans_diag[state0][minus] = state4;
	trans_diag[state0][star] = state12;
	trans_diag[state0][slash] = state5;
	trans_diag[state0][equal] = okNoUnget;
	trans_diag[state0][lessThan] = state9;
	trans_diag[state0][moreThan] = state10;
	trans_diag[state0][colon] = state11;
	trans_diag[state0][comma] = okNoUnget;
	trans_diag[state0][semiColon] = okNoUnget;
	trans_diag[state0][leftPar] = okNoUnget;
	trans_diag[state0][rightPar] = okNoUnget;
	trans_diag[state0][leftBrk] = okNoUnget;
	trans_diag[state0][rightBrk] = okNoUnget;
	trans_diag[state0][whiteSpace] = state0;
	trans_diag[state0][eoff] = okState;

	//state1
	trans_diag[state1][letter] = state1;
	trans_diag[state1][digit] = state1;
	for(i=2 ; i< columns-1; i++)
		trans_diag[state1][i] = okState;
	
	//state2
	trans_diag[state2][letter] = error;
	trans_diag[state2][digit] = state2;	
	for(i=2 ; i< columns-1; i++)
		trans_diag[state2][i] = okState;

	//state3
	for(i=0 ; i< columns-1; i++)
		trans_diag[state3][i] = okState;	
	trans_diag[state3][equal] =  okNoUnget ;
	//state4
	for(i=0 ; i< columns-1; i++)
		trans_diag[state4][i] = okState;
	trans_diag[state4][equal] = okNoUnget;
	//state5
	for(i=0 ; i< columns-1; i++)
		trans_diag[state5][i] = okState;		
	trans_diag[state5][star] = state6;
	//state6
	for(i=0 ; i< columns-2; i++)
		trans_diag[state6][i] = state6;
	trans_diag[state6][eoff] = error;
	trans_diag[state6][star] = state7;
	//state7
	for(i=0 ; i< columns-2; i++)
		trans_diag[state7][i] = state6;
	trans_diag[state7][eoff] = error;
	trans_diag[state7][slash] = state0;
	//state8
	for(i=0 ; i< columns-1; i++)
		trans_diag[state8][i] = error;	
	trans_diag[state8][equal] = okNoUnget;
	//state9
	for(i=0 ; i< columns-1; i++)
		trans_diag[state9][i] = okState;
	trans_diag[state9][equal] = okNoUnget;
	trans_diag[state9][moreThan] = okNoUnget;	
	//state10
	for(i=0 ; i< columns-1; i++)
		trans_diag[state10][i] = okState;
	trans_diag[state10][equal] = okNoUnget;

	//state11	
	for(i=0 ; i< columns-1; i++)
		trans_diag[state11][i] = okState;
	trans_diag[state11][equal] = okNoUnget;	

	//state12
	for(i=0 ; i< columns-1; i++)
		trans_diag[state12][i] = okState;
	trans_diag[state12][slash] = error;
}

/**
*Initializes the basic parameters of program.
*@param programFileName Contains the name of inputFile.
**/
void initializeParameters(char* programFileName)
{
	allocateList(&programList);
	countTempVariables = 0;
	quadNumber = 0;
	currentLineInFile = 1;
	token_id=1;
	strncpy(file_name,programFileName,INPUT_FILENAME_SIZE);

	sourceFile = fopen(file_name,"r");
	token = malloc (TOKEN_SIZE* sizeof(char));
	if(token == NULL)
	{
		printf("mpas: Fatal error. System unable to alocate memory.\n");
		exit(-3);
	}
	if( sourceFile == NULL )
	{
		printf("mpas: File: %s doesn't exists. Compilation terminated."
		"\n",file_name); 
		if(strlen(programFileName) > INPUT_FILENAME_SIZE)
			printf("mpas: Fatal Error: The size of filename can't"
			" be more than %d.\n",INPUT_FILENAME_SIZE);
			
		exit(-2);
	}

	fileNameLength = strlen(programFileName) + 2;
	//openFinalCodeFile();
	initializeLexAutomaton();
}

void printSpaces()
{
	int i;
	for(i = 0;i < fileNameLength;i++)
		printf(" ");
} 

void customExit(){
	close(finalCode);
	char *args[] = {"rm", finalCodeFileName, NULL};
	if( execvp( args[0], args) == -1){
		printf("Command not Found : %s \n",args[0]);
		kill(getpid(),SIGKILL);//KILL if exec not execute the command
	}
}


#endif
