//begin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"



// Assembler Directives: START, END, BYTE, WORD, RESB, RESW.

void splitString(char * line, char * label, char * opcode, char * operand, char * comment);
void pass1(FILE  * file);
void zeroOut(char * string, int length);
void initializeOpTab();
int OpTabContains(char * op);
int findNumOfBytes(int num);
void getOpCodeAddress(char * opCode, char * address);

void initializeErrorTables();
void insertError();
void outputErrorsToFile();
int hasIllegalCharacters(char * string);
void clearErrorTable();
int getNumErrors();

char ** OPTAB;
char ** ERRORMESSAGES;
int * ERRORS; 
const int MAXERRORS = 300;

char programName[6];
char programLength[6];
struct symTab SYMTAB;



int LOCCTR = 0;


// char * OPTAB[] = 

// int main(){


// 	// char label[20];
// 	// char opcode[20];
// 	// char operand[20]; 
// 	// char comment[100]; 

// 	// zeroOut(label, 20);
// 	// zeroOut(opcode, 20);
// 	// zeroOut(operand, 20);
// 	// zeroOut(comment, 100);

// 	// splitString("CLOOP   JSUB    RDREC		READ INPUT RECORD",label, opcode, operand, comment);


// 	// printf("Label:%s|\nOpcode:%s|\nOperand:%s|\nComment:%s|\n", label, opcode, operand, comment);


// 	FILE * file;

// 	file = fopen("./source.asm.txt", "r");


	


// 	pass1(file);

// 	printf("Program Name: %s", programName);
	


// 	return 0;
// }

void pass1(FILE * file){


	FILE * intermFile;
	FILE * symbolFile;


	intermFile = fopen("interm.txt", "w");
	symbolFile = fopen("symbol.txt", "w");


	// int i = 0;
	// while(i < 10){
	// 	int c;
	char line[256];
	// 	fgets(line, sizeof line, file);
	// 	printf("%s", line);
	// 	i++;
	// // printf("%d", getc
	// }

	char label[20];
	char opcode[20];
	char operand[20]; 
	char comment[256];
	char opHex[4]; 
	char opHexFormatted[10]; 


	// char * label = calloc(20, sizeof(char));
	// char * opcode = calloc(20, sizeof(char));
	// char * operand = calloc(20, sizeof(char)); 
	// char * comment = calloc(256, sizeof(char));
	// char * opHex = calloc(4, sizeof(char)); 
	// char * opHexFormatted = calloc(10, sizeof(char)); 


	//Used for hex address
	char address[5];

	zeroOut(label, 20);
	zeroOut(opcode, 20);
	zeroOut(operand, 20);
	zeroOut(comment, 256);

	int startingAddress;

	//Used for stroi()
	char *ptr;

	int lineNum = 1;


	// printf("%d", getc(file));
	//read first output line
	

	fgets(line, sizeof line, file);



	splitString(line, label, opcode, operand, comment);

	
	//Create Symbol Table
	initializeSymbolTable(&SYMTAB);
	initializeErrorTables();

	// Initialize OPTAB
	initializeOpTab();


	
	// printf("%d", strcmp(opcode, "START") == 0);


	//if opcode = 'START' then
	if(strcmp(opcode, "START") == 0){

		//printf("TRUE");
		//begin
			//save #[OPERAND] as starting address
			long n = strtol(operand, NULL, 16);
			startingAddress = n;
			//initialize LOCCTR to starting address
			LOCCTR = startingAddress;
			strcpy(programName, label);
			//write line to intermediate file
			
			
			//read next input line
			

	}	//end {if START}
	//else
	else{
		//initialize LOCCTR to
		LOCCTR = 0;
	}


	sprintf(address, "0x%x:", LOCCTR);
	fputs(address, intermFile);
	if(OpTabContains(opcode)) getOpCodeAddress(opcode, opHex);
	sprintf(opHexFormatted, "%s:", opHex);
	fputs(opHexFormatted, intermFile);

	
	lineNum++;
	fputs(operand, intermFile);
	fputs(":", intermFile);
	fputs(opcode, intermFile);
	fputs(";", intermFile);
	fputs(line, intermFile);

	fgets(line, sizeof line, file);
	splitString(line, label, opcode, operand, comment);


	//while OPCODE != END do
	while(strcmp(opcode,"END") != 0){

		if(hasIllegalCharacters(label) && label[0] != '.'){
				insertError(lineNum, 1);
		}
	
		//if this is not a comment line then
		if(strcmp(label, ".") != 0){
			//begin
				//if there is a symbol in the LABEL field then
				if(label[0] != 0){
					//begin
					//search SYMTAB for LABEL
					if(symTabContains(&SYMTAB, label) == 1){			
						//if found then
						//set error flag{duplicate symbol}
						insertError(lineNum, 2);
					}//else
					else{
						//insert (LABEL, LOCCTR) into symtab
						if(strlen(label) > 6){
							insertError(lineNum, 1);
						}

						addToSymTab(&SYMTAB, label, LOCCTR);
					}
				}//end {if symbol}
				
				sprintf(address, "0x%x:", LOCCTR);
				fputs(address, intermFile);

				//search OPTAB for OPCODE
				if(OpTabContains(opcode)){
					//if found then
					//add 3 {instruction length} to LOCCTR

					getOpCodeAddress(opcode, opHex);
					
					sprintf(opHexFormatted, "%s:", opHex);
					fputs(opHexFormatted, intermFile);
					fputs(operand, intermFile);
					fputs(":", intermFile);
					fputs(opcode, intermFile);
					fputs(";", intermFile);

					if(strcmp(opcode, "WORD") == 0){
						//else if OPCODE = 'WORD' then
						//add then add 3 to LOCCTR
						LOCCTR += 3;
					}
					else if(strcmp(opcode, "RESW") == 0){
						//else if OPCODE = 'RESW' then
						//add 3 * #[OPERAND] then to LOCCTR
						LOCCTR += 3 * atoi(operand);
					}
					else if(strcmp(opcode, "RESB") == 0){
						//else if OPCODE = 'RESB'
						//add #[OPERAND] to LOCCTR
					
						LOCCTR += atoi(operand);
					}
					else if(strcmp(opcode, "BYTE") == 0){
						//else OPCODE = "BYTE" then
						//find length of constant in bytes
						//add length to LOCCTR
						if(operand[0] == 'C')
							LOCCTR += strlen(operand) - 3;
						if(operand[0] == 'X')
							LOCCTR += (strlen(operand) - 3) / 2;
					}//end {if BYTE}
					else LOCCTR += 3;
				}
	
				else{

					sprintf(opHexFormatted, "%s:", "0000");
					fputs(opHexFormatted, intermFile);
					fputs(operand, intermFile);
					fputs(":", intermFile);
					fputs(opcode, intermFile);
					fputs(";", intermFile);
					//Flag error representing invalid opcode
					insertError(lineNum, 0);
				}


			
			
		} //end{if not a comment}
		else{
			fputs("         ", intermFile);
		}
		//write line to intermediate file
		

		
		fputs(line, intermFile);
		//read next input line
		zeroOut(line, 256);


		fgets(line, sizeof line, file);

		splitString(line, label, opcode, operand, comment);
		
		lineNum++;
	}//end {while not END}
	
	//write last line to intermediate file

	sprintf(address, "0x%x:", LOCCTR);
	fputs(address, intermFile);
	if(OpTabContains(opcode)) getOpCodeAddress(opcode, opHex);
	sprintf(opHexFormatted, "%s:", opHex);
	fputs(opHexFormatted, intermFile);
	fputs(operand, intermFile);
	fputs(":", intermFile);
	fputs(opcode, intermFile);
	fputs(";", intermFile);
	// fputs(operand, intermFile);
	fputs(line, intermFile);
	//save (LOCCTR - starting address) as program length
	sprintf(programLength, "%x", LOCCTR);

	// int i = 0;
	// while(i < 100){
	// 	printf("%d\n", ERRORS[i]);
	// 	i++;
	// }

	outputErrorsToFile(intermFile);


 	fclose(intermFile);

 	writeSymbolTableToFile(&SYMTAB, symbolFile);

 	fclose(symbolFile);

 	printSymbolTable(&SYMTAB);


 	// for(int i = 0; i < SYMTAB.length; i++)
  //   {
  //       free(SYMTAB.array[i]);
  //   }

    // free(SYMTAB.array);
}//end Pass 1





void initializeErrorTables(){
	ERRORMESSAGES = (char **)calloc(10, sizeof(char*));

	int i = 0;
	while(i < 10){
		//Allocate memory for individual strings
		ERRORMESSAGES[i] = (char * )calloc(50, sizeof(char));
		i++;
	}

	ERRORMESSAGES[0] = "ILLEGAL OPCODE";
	ERRORMESSAGES[1] = "ILLEGAL SYMBOL";
	ERRORMESSAGES[2] = "DUPLICATE SYMBOL";
	ERRORMESSAGES[3] = "UNDEFINED SYMBOL";
	ERRORMESSAGES[4] = "ILLEGAL OPERAND";
	ERRORMESSAGES[5] = "TOO MANY SYMBOLS IN SOURCE PROGRAM";
	ERRORMESSAGES[6] = "PROGRAM TOO LONG";
	ERRORMESSAGES[7] = "MISSING OR MISSPLACED START DIRECTIVE";
	


	//Table that will hold a line and the error that's on that line next to each other.
	ERRORS = (int *)calloc(MAXERRORS, sizeof(int));

	i = 0;
	while(i < MAXERRORS){
		ERRORS[i] = -1;
		i++;
	}


}

//Inserts an error to the error table
void insertError(int line, int errorCode){

	// printf("INSERTING ERROR %d at line %d", errorCode, line);

	int i = 0;
	while(i < 100){
		if(ERRORS[i] == -1){
			ERRORS[i] = line;
			ERRORS[i + 1] = errorCode;
			return;
		}
		i += 2;
	}
}


//Writes errors to intermediate file
void outputErrorsToFile(FILE * file){
	// fprintf(file, "\n\nERRORS\n");

	int i = 0;
	while(ERRORS[i] != -1 && i < 100){
		fprintf(file, "%d %d\n", ERRORS[i], ERRORS[i + 1]);
		i+=2;
	}
}



// Returns 1 if OPTAB contains an op
// Returns 0 if not
int OpTabContains(char * op){
	// if(DEBUGGING) printf("Searching OPTAB For OP: %s...\n", op);

	int i = 0;
	while(i < 61){
		if(strcmp(op, OPTAB[i]) == 0){
			// if(DEBUGGING) printf(" Found On Element: %d \n", i);
			return 1;
		}
		i += 2;			
	}

	debugPrint("	Not Found...\n");
	return 0;
}

void getOpCodeAddress(char * opCode, char * address){
	
	int i = 0;
	while(i < 62){
		if(strcmp(opCode, OPTAB[i]) == 0){
			strcpy(address, OPTAB[i + 1]);
		}
		i+=2;
	}
}


//Load opcode mnemonics and opcodes into OPTABLE
void initializeOpTab(){

	debugPrint("Initiazling OPTAB...\n");
	

	OPTAB = (char **)calloc(64, sizeof(char*));

	int i = 0;
	while(i < 64){
		//Allocate memory for individual strings
		OPTAB[i] = (char * )calloc(5, sizeof(char));
		i++;
	}

	debugPrint("	Assigning values to OPTAB...\n");

	OPTAB[0] = "START";
	OPTAB[1] = "0xA1";
	OPTAB[2] = "END";
	OPTAB[3] = "0xA2";
	OPTAB[4] = "BYTE";
	OPTAB[5] = "0xA3";
	OPTAB[6] = "WORD";
	OPTAB[7] = "0xA4";
	OPTAB[8] = "RESB";
	OPTAB[9] = "0xA5";
	OPTAB[10] = "RESW";
	OPTAB[11] = "0xA6";
	OPTAB[12] = "ADD";
	OPTAB[13] = "0x18";
	OPTAB[14] = "AND";
	OPTAB[15] = "0x58";
	OPTAB[16] = "COMP";
	OPTAB[17] = "0x28";
	OPTAB[18] = "DIV";
	OPTAB[19] = "0x24";
	OPTAB[20] = "J";
	OPTAB[21] = "0x3C";
	OPTAB[22] = "JEQ";
	OPTAB[23] = "0x30";
	OPTAB[24] = "JGT";
	OPTAB[25] = "0x34";
	OPTAB[26] = "JLT";
	OPTAB[27] = "0x38";
	OPTAB[28] = "JSUB";
	OPTAB[29] = "0x48";
	OPTAB[30] = "SUB";
	OPTAB[31] = "0x1C";
	OPTAB[32] = "TD";
	OPTAB[33] = "0xE0";
	OPTAB[34] = "TIX";
	OPTAB[35] = "0x2C";
	OPTAB[36] = "WD";
	OPTAB[37] = "0xDC";
	OPTAB[38] = "LDA";
	OPTAB[39] = "0x00";
	OPTAB[40] = "LDCH";
	OPTAB[41] = "0x50";
	OPTAB[42] = "LDL";
	OPTAB[43] = "0x08";
	OPTAB[44] = "LDX";
	OPTAB[45] = "0x04";
	OPTAB[46] = "MUL";
	OPTAB[47] = "0x20";
	OPTAB[48] = "OR";
	OPTAB[49] = "0x44";
	OPTAB[50] = "RD";
	OPTAB[51] = "0xD8";
	OPTAB[52] = "RSUB";
	OPTAB[53] = "0x4C";
	OPTAB[54] = "STA";
	OPTAB[55] = "0x0C";
	OPTAB[56] = "STCH";
	OPTAB[57] = "0x54";
	OPTAB[58] = "STL";
	OPTAB[59] = "0x14";
	OPTAB[60] = "STX";
	OPTAB[61] = "0x10";
	OPTAB[62][0] = 0;


	// i = 0;
	// while(i < 62){
	// 	printf("%2d: %9s %5s \n", i / 2 + 1, OPTAB[i], OPTAB[i + 1]);
	// 	i+= 2;
	// }

}




// This function will split a string into 4 different parts
// It uses " " or "\t" as delimiters, but will ignore conescutive spaces as delimiters
// when writing to the comment array.
void splitString(char * line, char * label, char * opcode, char * operand, char * comment){

	int noLabel = 0;
	int writingTo = 0; //Represents the current char array being written to. label = 0, opcode = 1, operand = 2, comment = 3
	int inSpaces = 0; //represents that repetetive spaces are being found

	// set all elements to 0
	zeroOut(label, 20);
	zeroOut(opcode, 20);
	zeroOut(operand, 20);
	zeroOut(comment, 256);

	int j = 0; //represents index of current argument
	int i = 0;

	//Check if line is a comment
	if(line[0] == '.'){
		while(i < 256){
			if(comment[i] == '\n')
				break;
			comment[i] = line[i];
			i++;
		}
		label[0] = '.';
		return;
	}

	i = 0;
	//loop untill null terminator
	while(line[i] != 0){

		// if(DEBUGGING == 1)
		// 	printf("At index %d, current char is: %d = %c. Writing to %d\n", i, line[i], line[i], writingTo);

		if(line[i] ==  ' ' || line[i] == 9){


			if(writingTo == 3){
				comment[j] = line[i];
				j++;
			}
			else if(inSpaces == 1){
				i++;
				continue;
			}
			else{ //null terminate appropriate char array
				if(writingTo == 0) label[j + 1] = 0;
				else if(writingTo == 1) opcode[j + 1] = 0;
				else if(writingTo == 2) operand[j + 1] = 0;
				else if(writingTo == 3) comment[j + 1] = 0;
				writingTo++;
				j = 0;
			}

			

			if(i == 0){
				noLabel = 1;
				label[0] = 0;
				writingTo = 1;
			}

			inSpaces = 1;  //indicate that the last character found was whitespace

		}	
		else{
			inSpaces = 0;

			if(writingTo == 0){
				label[j] = line[i];
				j++;
			}
			else if(writingTo == 1){
				opcode[j] = line[i];
				j++;
			}
			else if(writingTo == 2){
				operand[j] = line[i];
				j++;
			}
			else if(writingTo == 3){
				comment[j] = line[i];
				j++;
			}

		}

		if(line[i + 1] == '\n' || line[i+1] == 0){
			if(writingTo == 3)
				comment[j + 1] == 0;
		}

		i++;
	}

	i = 0;
	//hotfix fix any newlines in the label
	while(i < 20){
		if(label[i] == '\n' || label[i] == '\r' )
			label[i] = 0;
		if(opcode[i] == '\n' || opcode[i] == '\r' )
			opcode[i] = 0;
		if(operand[i] == '\n' || operand[i] == '\r')
			operand[i] = 0;
		i++;
	}
}

// Returns the number of bytes in a value
int findNumOfBytes(int num){
	int count = 1;

	while(num > 1){
		num = num / 2;
		count++;
	}

	return count / 8 + 1;
}



// Sets all characters in an array to 0
void zeroOut(char * string, int length){

	int i = 0;
	while(i < length){
		string[i] = 0;
		i++;
	}	
}

	
//Returns 1 if string has illegal characters, i.e. characters that are not a number or symbol
int hasIllegalCharacters(char * string){

	if(string[0] > 48 && string[0] < 58) return 1;
	// printf("%d", strlen(string));

	int i = 0;
	while(i < strlen(string)){
		if(string[i] < 48 || (string[i] > 57 && string[i] < 65) || (string[i] > 90 && string[i] < 97)){
			// printf("%c is an illegal character at index %d\n", string[i], i);
			return 1;
		}
		i++;
	}

	return 0;
}


int getNumErrors(){
	int i = 0;
	int numErrors = 0;

	while(i < MAXERRORS){
		if(ERRORS[i] != -1){
			printf("ERRORS[%d]: %d\n", i, ERRORS[i]);
			numErrors++;
		}
		i+=2;
	}
	return numErrors;
}

void clearErrorTable(){
	int i = 0;

	while(i < MAXERRORS){
		ERRORS[i] = -1;
		i++;
	}
}