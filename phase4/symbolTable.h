
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

	int i = 0;
	while(i < s->length){
		if(s->array[i][0] == 0){
			strcpy(s->array[i], symbol);

			sprintf(s->array[i + 1], "%x", LOCCTR);
			break;
		}
		i+= 2;
	}

	debugPrint("Done Adding to Symbol Table...\n");
}


//Adds a symbol to the symbol table
char * getSymbolAddress(struct symTab * s, char * symbol, char * address){

	int i = 0;
	while(i < s->length){
		if(strcmp(s->array[i], symbol) == 0){
			strcpy(address, s->array[i + 1]);
		}
		i+= 2;
	}

}
	

//Returns 1 if symbol table contains the symbol
//Returns 0 if it does not
int symTabContains(struct symTab * s, char * symbol){


	if(DEBUGGING) printf("Searching for Symbol: %s\n", symbol);

	//TODO: Implement this
	int i = 0;
	while(i < s->length){

		if(DEBUGGING) printf("Current Element: %d\n", i);


		if(strcmp(symbol, s->array[i]) == 0){
			if(DEBUGGING) printf("Symbol: %s == %s at index %d\n", symbol, s->array[i], i);
			if(DEBUGGING) printf("Symbol: %s Found\n", symbol);
			return 1;
		}

		i+=2;
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

	int i = 0;
	while(i < s->length){
		//Allocate memory for individual strings
		s->array[i] = (char * )calloc(12, sizeof(char));
		 i++;
	}

	i = 0;
	int j = 0;
	while(i < s->length){
		j = 0;
		while(j < 12){
			s->array[i][j] = 0;
			j++;
		}
		i++;
	}


	debugPrint("Initialization Done..\n");
}


void writeSymbolTableToFile(struct symTab * s, FILE * file){

	int i = 0;
	while(i < s->length){
		fputs(s->array[i], file);
		fputs("|", file);
		fputs(s->array[i + 1], file);
		fputs("\n", file);

		i+=2;
	}
}


//DEBUGCODE

void printSymbolTable(struct symTab * s){
	debugPrint("Printing Symbol Table..\n");

	printf("%12s %11s\n", "SYMBOL", "ADDRESS");
	printf("---------------------------\n");

	int i = 0;
	while( i < s->length){
		printf("%2d: %9s %5s \n", i / 2 + 1, s->array[i], s->array[i + 1]);
		i+= 2;
	}


	debugPrint("Printing Done..\n");
}

void debugPrint(char * s){
	if(DEBUGGING)
		printf("%s", s);
}
