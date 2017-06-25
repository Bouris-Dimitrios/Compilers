/**
*@file main.c
*\authors{ Bouris Dimitris 1894 (bourisdim@gmail.com) ,
 Nikos Ginargyros 2038 (nikospaxos@hotmail.com) }
*@date  March ,2014

*@brief Calls initialization function and then starts the syntaxAnalyzer.
*/

/*! \mainpage Project: Mini Pascal Compiler 
 *
 * \section intro_sec Introduction
 *
 * This is the Main Page of our project Documentation.\n
 * This project is developed for Lesson Compilers [PLY602].\n
 * In this project we implement a mini pascal compiler.\n

 * \subsection comp Compiler
 * A compiler is a computer program (or set of programs) that
 * transforms source code written in a programming language \n
 * (the source language) into another computer language 
 * (the target language, often   having a binary form known as object code).\n
 * The most common reason for wanting to transform source code is to create an
 * executable program.
 * The name "compiler" is primarily used for programs that translate source \n
 * code from a high-level programming language to a lower level language \n
 *(e.g., assembly language or machine code). If the compiled program can run
 * on a computer whose CPU or operating system is different from the one on \n
 * which the compiler runs, the compiler is known as a cross-compiler. \n
 * A program that translates from a low level language to a higher level one 
 * is a decompiler. A program that translates between high-level languages is\n
 * usually called a language translator, source to source translator, or
 * language converter. A language rewriter is usually a program that \n
 * translates the form of expressions without a change of language.

 * \subsection phase Phases of Compilation :
 * - Lexical Analysis
 * - Syntactical Analysis
 * - Semantic Analysis
 * - Code Generation
 * - Code Optimization

 * \section docum Documentation Info
 * In this documentation you will see :\n
 * - Files description\n
 * - Functions description\n
 * - Call graph for every function\n
 * - Dependency Graph for every function\n
 * - Complete describe of variables and constants\n
 * \section author_sec Authors
 *
 * \subsection students Students :
 *
 * - Bouris Dimitris 1894 (bourisdim@gmail.com)\n
 * - Nikos Ginargyros 2038 (nikospaxos@hotmail.com)
 * \subsection professor Professor :
 *
 * - George Manis (manis@cs.uoi.gr)

 * \subsection uni University :
 *
 * - University of Ioannina ( www.uoi.gr )
 * - Deparment of Computer Science & Engineering ( www.cs.uoi.gr )
 * \section date Date 
 *
 * - March of 2014, (Spring Semester)
 */

#include <stdio.h>
#include <stdlib.h>
#include "functionsLib.h"
#include "symbolTableStructures.h"

void main(char argc,char * argv[])
{
	
	if(argc < 2)
	{
		printf("mpas:Fatal Error.No input files.\n");
		printf("mpas:usage: %s <fileName.mpas>\n", argv[0]);
		exit(-1);
	}

	initializeParameters(argv[1]);
	syntaxAnalyzer();

}
