#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
// typedef struct symTab symTab

//Initial size of array for symbol table
int INITSIZE = 20;
int DEBUGGING = 1;

struct symTab{	
	int length;
	char ** array;

};  


void debugPrint(char * s);
void addToSymTab(struct symTab * s, char * symbol, int LOCCTR);
void initializeSymbolTable(struct symTab * s);
void printSymbolTable(struct symTab * s);
int symTabContains(struct symTab * s, char * symbol);



int main(){
	struct symTab s;
	initializeSymbolTable(&s);
	addToSymTab(&s, "DFDFSDF", 1000);
	addToSymTab(&s, "ASFLKNE", 1005);
	addToSymTab(&s, "SDFLKNE", 1003);
	addToSymTab(&s, "LKNESDF", 1007);
	addToSymTab(&s, "ASDSAWE", 1009);
	addToSymTab(&s, "5443534", 1006);




	//Test Code for Symbol Table
	if(symTabContains(&s, "DFDFSDF"))
		printf("Passed\n");
	else(printf("Failed\n"));
	if(symTabContains(&s, "ASFLKNE"))
		printf("Passed\n");
	else(printf("Failed\n"));
	if(symTabContains(&s, "SDFLKNE"))
		printf("Passed\n");
	else(printf("Failed\n"));
	if(symTabContains(&s, "LKNESDF"))
		printf("Passed\n");
	else(printf("Failed\n"));
	if(symTabContains(&s, "ASDSAWE"))
		printf("Passed\n");
	else(printf("Failed\n"));
	if(symTabContains(&s, "5443534"))
		printf("Passed\n");
	else(printf("Failed\n"));
	if(!symTabContains(&s, "5s43534"))
		printf("Passed\n");
	else(printf("Failed\n"));


	printSymbolTable(&s);

}


//Adds a symbol to the symbol table
void addToSymTab(struct symTab * s, char * symbol, int LOCCTR){
	debugPrint("Adding to Symbol Table...\n");


	for(int i = 0; i < s->length; i+= 2){
		if(s->array[i][0] == 0){
			s->array[i] = symbol;

			sprintf(s->array[i + 1], "%x", LOCCTR);
			break;
		}
	}

	debugPrint("Done Adding to Symbol Table...\n");
}
	

//Returns 1 if symbol table contains the symbol
//Returns 0 if it does not
int symTabContains(struct symTab * s, char * symbol){

	//TODO: Implement this
	for(int i = 0; i < s->length; i+=2){
		if(strcmp(symbol, s->array[i]) == 0){
			return 1;
		}
	}
	return 0;
}

// Initialize the symbol table
void initializeSymbolTable(struct symTab * s){
	debugPrint("Initializing Symbol Table...\n");

	//Set length of array to initial size
	s->length = INITSIZE;

	printf("s.length: %d\n", s->length);
	printf("INITSIZE: %d\n", INITSIZE);


	//Allocate that much memory
	s->array = (char **)calloc(s->length, sizeof(char*));

	for(int i = 0; i < s->length; i++){
		//Allocate memory for individual strings
		s->array[i] = (char * )calloc(12, sizeof(char));
	}




	debugPrint("Initialization Done..\n");
}


//DEBUGCODE

void printSymbolTable(struct symTab * s){
	debugPrint("Printing Symbol Table..\n");

	printf("%12s %11s\n", "SYMBOL", "ADDRESS");
	printf("---------------------------\n");
	for(int i = 0; i < s->length; i+= 2){
		printf("%2d: %9s %5s \n", i / 2 + 1, s->array[i], s->array[i + 1]);
	}


	debugPrint("Printing Done..\n");
}

void debugPrint(char * s){
	if(DEBUGGING)
		printf("%s", s);
}
