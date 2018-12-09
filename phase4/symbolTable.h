
//Initial size of array for symbol table
int INITSIZE = 40;
int DEBUGGING = 0;
struct symTab{	
	int length;
	char ** array;

};  


void debugPrint(char * s);
void addToSymTab(struct symTab * s, char * symbol, int LOCCTR);
void initializeSymbolTable(struct symTab * s);
void printSymbolTable(struct symTab * s);
int symTabContains(struct symTab * s, char * symbol);




//Adds a symbol to the symbol table
void addToSymTab(struct symTab * s, char * symbol, int LOCCTR){
	if(DEBUGGING) printf("Adding %s to Symbol Table at Location %d...\n", symbol, LOCCTR);


	for(int i = 0; i < s->length; i+= 2){
		if(s->array[i][0] == 0){
			strcpy(s->array[i], symbol);

			sprintf(s->array[i + 1], "%x", LOCCTR);
			break;
		}
	}

	debugPrint("Done Adding to Symbol Table...\n");
}


//Adds a symbol to the symbol table
char * getSymbolAddress(struct symTab * s, char * symbol, char * address){

	for(int i = 0; i < s->length; i+= 2){
		if(strcmp(s->array[i], symbol) == 0){
			strcpy(address, s->array[i + 1]);
		}
	}

}
	

//Returns 1 if symbol table contains the symbol
//Returns 0 if it does not
int symTabContains(struct symTab * s, char * symbol){


	if(DEBUGGING) printf("Searching for Symbol: %s\n", symbol);

	//TODO: Implement this
	for(int i = 0; i < s->length; i+=2){

		if(DEBUGGING) printf("Current Element: %d\n", i);


		if(strcmp(symbol, s->array[i]) == 0){
			if(DEBUGGING) printf("Symbol: %s == %s at index %d\n", symbol, s->array[i], i);
			if(DEBUGGING) printf("Symbol: %s Found\n", symbol);
			return 1;
		}
	}

		if(DEBUGGING) printf("Symbol: %s Not Found\n", symbol);
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

	for(int i = 0; i < s->length; i++){
		for(int j = 0; j < 12; j++){
			s->array[i][j] = 0;
		}
	}


	debugPrint("Initialization Done..\n");
}


void writeSymbolTableToFile(struct symTab * s, FILE * file){

	for(int i = 0; i < s->length; i+=2){
		fputs(s->array[i], file);
		fputs("|", file);
		fputs(s->array[i + 1], file);
		fputs("\n", file);
	}
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
