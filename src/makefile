CC = gcc -g
CFLAGS = -Wall 
OBJECTS = main.o syntaxAnalyzer.o lex.o rules.o initializations.o intermediateCodeFunctions.o listStructures.o symbolTableStructures.o finalCodeFunctions.o

mpas: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o mpas
main.o: main.c 
	$(CC) -c main.c
syntaxAnalyzer.o: syntaxAnalyzer.c functionsLib.h definitions.h rules.h
	$(CC) -c syntaxAnalyzer.c
lex.o: lex.c  functionsLib.h definitions.h
	$(CC) -c lex.c
rules.o: rules.h rules.c definitions.h
	$(CC) -c rules.c
initializations.o: initializations.c definitions.h functionsLib.h
	$(CC) -c initializations.c
listStructures.o: listStructures.c listStructures.h definitions.h
	$(CC) -c listStructures.c	
symbolTableStructures.o: symbolTableStructures.c symbolTableStructures.h 
	$(CC) -c symbolTableStructures.c
intermediateCodeFunctions.o: intermediateCodeFunctions.c functionsLib.h
	$(CC) -c intermediateCodeFunctions.c	
finalCodeFunctions.o: finalCodeFunctions.c finalCodeFunctions.h
	$(CC) -c finalCodeFunctions.c	
clean:
	rm  *.o *.mp mpas

