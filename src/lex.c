#ifndef __LEX_C__
#define __LEX_C__
/**
*@file lex.c
*\authors{ Bouris Dimitris 1894 (bourisdim@gmail.com) ,
 Nikos Ginargyros 2038 (nikospaxos@hotmail.com) }
*@date  March ,2014

*@brief  Contains the implementation of lexical analyzer.  
*/

#include "functionsLib.h"
/**
*@param currentChar The last character read from file.
*@brief Recognizes the  type of current character  based in
* its ascii value and returns it to lex() function.\n Futhermore,
* helps to keep the lines of the input file needed for better
* description of an error.
*@return Returns an integer that describes the type of input character.
**/
int recognizeTypeOfInput(char currentChar)
{
	int asciiValue = (currentChar - 0);// This gives the ascci value of 
					   // current character.

	if((asciiValue >=48) && (asciiValue <= 57))
		return digit; // DIGIT
	else if(((asciiValue >=65) && (asciiValue <=90)) || ((asciiValue >=97) 
		&& (asciiValue <=122)))
		return letter; //LETTER
	else if(asciiValue == 43)
		return plus; // +
	else if(asciiValue == 45)
		return minus; // -
	else if(asciiValue == 42)
		return star; // *
	else if(asciiValue == 47)
		return slash; // /
	else if(asciiValue == 61)
		return equal; // =
	else if(asciiValue == 60)
		return lessThan; // <
	else if(asciiValue == 62)
		return moreThan; // >
	else if(asciiValue == 58)
		return colon; // :
	else if(asciiValue == 44)
		return comma; // ,
	else if(asciiValue == 59)
		return semiColon; // ;
	else if(asciiValue == 41)
		return rightPar; // )
	else if(asciiValue == 40)
		return leftPar; // (
	else if(asciiValue == 91)
		return leftBrk;
	else if(asciiValue == 93)
		return rightBrk;
	else if(currentChar == EOF)
		return eoff;
	else if (asciiValue==10)
	{
		currentLineInFile++;
		return whiteSpace;
	}
	else if ((asciiValue == 32) || (asciiValue==9) )
		return whiteSpace; // whitechar
	else
		return other; // UNRECOGNIZED OPTION
}

/**
*@param currentChar The last character read from file.
*@param typeOfInput The value that is returned from recognizeTypeOfInput()
*@param error_state Describes  witch was the previous state before
* the error occured.
* @brief Informs the user properly for errors-warnings that lexical analyzer 
* is responsilble to recognize.
**/
void errorHandler(char currentChar,int typeOfInput,int error_state)
{
	if(typeOfInput == other)
	{
		printf("%s :Error:%d: '%c' ilegal character.\n"
		,file_name,currentLineInFile,currentChar);
		exit(error);
	}
	else if(error_state == state2)
	{
		printf("%s :Error:%d: Numerical constants should contain only "
		"numbers.\n",file_name,currentLineInFile);
		printSpaces();
		printf("'%c' not expected.\n",currentChar); 
		exit(error);
	}
	else if(error_state == state6)
	{
		printf("%s :Error:%d:Unterminated comment.\"*/\" expected.\n"
		,file_name,currentLineInFile);
		exit(error);
	}
	else if(error_state == state8)
	{
		printf("%s :Error:%d:This character '=' cannot be used alone."
		"\n",file_name,currentLineInFile);
		printSpaces();
		printf("Suggestion:try \"==\".\n");
		exit(error);
	}
	else if(error_state == warning)
	{
		printf("%s :Warning:%d: All identifiers must have less than 30 "
		"characters.\n",file_name,currentLineInFile);
		printSpaces();
		printf("Only the first 30 characters will be used.\n");
	}
	else if(error_state == state12)
	{
		printf("%s :Error:%d: Unexpected \"*/\".\n"
		,file_name,currentLineInFile); 
		printSpaces();
		printf("This character is used to close comments.\n");
		printSpaces();
		printf("For comments use this syntax \"/*\" <comments>"
		" \"*/\".\n");
		exit(error);
	}
	else if (error_state == numberOverFlow)
	{
		printf("%s :Error:%d: Number overflow detected.\n"
		,file_name,currentLineInFile);
		printSpaces();
		printf("This number %s is not legal.\n",token); 
		printSpaces();
 		printf("Mini pascal supports numbers from -(2^31-1) to "
		"(2^31-1).\n");
		exit(error);
	}
}
/**
*@param currentChar The last character read from file.
*@param typeOfInput The value that is returned from recognizeTypeOfInput()
*@return Returns an integer that describes the recognized token.
*@brief This function uses the token to recognize keywords,symbols of 
*MiniPascal. Also It checks the value of numerical constants because Mini Pascal
* allows numbers from -(2^31 - 1) to (2^31 -1). 
**/
int keyWordRecognizer(char currentChar,int typeOfInput)
{
	unsigned long value;
	int upperBound;
	if(strcmp(token,"program")==0)
		return programTk;
	else if(strcmp(token,"function")==0)
		return functionTk;
	else if(strcmp(token,"procedure")==0)
		return procedureTk;
	else if(strcmp(token,"return")==0)
		return returnTk;
	else if(strcmp(token,"call")==0)
		return callTk;
	else if(strcmp(token,"begin")==0)
		return beginTk;
	else if(strcmp(token,"end")==0)
		return endTk;
	else if(strcmp(token,"if")==0)
		return ifTk;
	else if(strcmp(token,"then")==0)
		return thenTk;
	else if(strcmp(token,"else")==0)
		return elseTk;
	else if(strcmp(token,"do")==0)
		return doTk;
	else if(strcmp(token,"while")==0)
		return whileTk;
	else if(strcmp(token,"for")==0)
		return forTk;
	else if(strcmp(token,"to")==0)
		return toTk;
	else if(strcmp(token,"step")==0)
		return stepTk;
	else if(strcmp(token,"var")==0)
		return varTk;
	else if(strcmp(token,"const")==0)
		return constTk;
	else if(strcmp(token,"input")==0)
		return inputTk;
	else if(strcmp(token,"print")==0)
		return printTk;
	else if(strcmp(token,"and")==0)
		return andTk;
	else if(strcmp(token,"or")==0)
		return orTk;
	else if(strcmp(token,"not")==0)
		return notTk;
	else if(strcmp(token,"<=")==0)
		return lesseqTk;
	else if(strcmp(token,">=")==0)
		return moreeqTk;
	else if(strcmp(token,"<>")==0)
		return diffTk;
	else if(strcmp(token,"=")==0)
		return comparTk;
	else if(strcmp(token,":=")==0)
		return assignTk;
	else if(strcmp(token,"+=")==0)
		return pluseqTk;
	else if(strcmp(token,"-=")==0)
		return minuseqTk;
	else if(strcmp(token,">")==0)
		return moreThan;
	else if(strcmp(token,"<")==0)
		return lessThan;
	else if(strcmp(token,"*") == 0)
		return star;
	else if(strcmp(token,"/") == 0)
		return slash;
	else if(strcmp(token,"+") == 0)
		return plus;
	else if(strcmp(token,"-") == 0)
		return minus;
	else if(strcmp(token,":") == 0)
		return colon;
	else if(strcmp(token,";") == 0)
		return semiColon;
	else if(currentChar==EOF)
		return eoff;
	else if(strcmp(token,"select")==0)
		return selectTk;
	else if(strcmp(token,"endselect")==0)
		return endSelectTk;
	else if(strcmp(token,"is")==0)
		return isTk;
	else if(strcmp(token,"equal")==0)
		return equalKeyTk;
	else if(((token[0]-0) >=48) && ( (token[0]-0) <= 57))
	{
		value = atol(token);
		upperBound = (pow(2,31)-1);
		if(value > upperBound)
			errorHandler(currentChar,typeOfInput,numberOverFlow);
		return numConst;
	}
	else if(((token[0]-0 >=65) && (token[0]-0 <=90)) || ((token[0]-0 >= 97) 
		&& (token[0]-0 <= 122)))
		return identifierTk;
	else
		return typeOfInput; 
}

/**
*@return Returns the numerical description of Mini Pascal keywords. 
*@brief  Is responsible to find the tokens in the input files, recognize it
* and return it. \n To do that uses the lexical automaton. \n First of all reads
* the file char by char and change states based on input.\n
* Make's all the needed check's for illegal characters, numerical constant
* values\n  and comment handling (ignore comment's, catch unterminated comments,
* ...) 
**/
int lex()
{
	
	char currentChar,buffer[BUFFER_SIZE] = {0};
	int i=0,state=0,typeOfInput=0;
	int prev_state=0,commentCounter = 0,comment_exists;

	while( state != okState && state != error && state != okNoUnget)
	{
		currentChar=(char)fgetc(sourceFile);
		prev_state = state;
		typeOfInput = recognizeTypeOfInput(currentChar);

		state = trans_diag[state][typeOfInput];
	
		/*if the automato pass from state7 to state0 then comments
		exists.*/
		if (state == state0 && prev_state == state7)
			commentCounter ++;

		if(state == okState && currentChar != '\n' ) /* (1) */
			ungetc(currentChar,sourceFile);
		else if (state == okNoUnget)
			buffer[i] = currentChar;
		else if(state != state6 && state != state7 && 
			typeOfInput != whiteSpace) /* (2) */
		{
			if( i < BUFFER_SIZE) /*prevent buffer overflow*/
				buffer[i] = currentChar;
			i++;
		}
	
		/* (1) Sometimes the recognization of a token needs the next
		character. This character must be returned to file.
		Also \n character should not be returned because the lines of 
		the file are not calculated properly.*/

		/* (2) in state6,state7 automaton ignores comments so this 
		characters must not be written to buffer.Also there is no reason
		to keep whitespaces in buffer.*/
	}

	if(state == error)
		errorHandler(currentChar,typeOfInput,prev_state);
	if(strlen(buffer) > MAX_TOKEN_CHARACTERS )
		errorHandler(currentChar,typeOfInput,warning);

	/* if a comment exists then in state5 of automaton we put in buffer the
	character '/' and in state7 the character '/'. In this case we must not 
	copy these characters in token.*/

	if( commentCounter == 0)
		strncpy(token,buffer,MAX_TOKEN_CHARACTERS);
	else
		strncpy(token,buffer+(2*commentCounter),MAX_TOKEN_CHARACTERS);

	return (keyWordRecognizer(currentChar,typeOfInput));
}

#endif
