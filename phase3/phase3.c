//Pass 2

#include "phase2.h"

void splitString2(char * line, char * lineAddr, char * opcode, char * label, char * opcodeHex, char * operand, char * comment, int * lineIsComment);
char *  toUpperCase(char * string);
void appendString(char * string1, char * string2);
char * lengthenToSixBytes(char * buffer, char * string);
char * lengthenToTwoBytes(char * buffer, char * string);
char * lengthenToFourBytes(char * buffer, char * string);
void removeApostrophes(char * string, char * buffer);
int containsComma(char * string);
void removeComma(char * string);
void addBit(char * string);
void convertStringToHex(char * string);

void convertCharToHex(char * string, char * buffer);


void pass2(FILE * intermFile){

	FILE * objFile;
	FILE * listingFile;


	intermFile = fopen("interm.txt", "r");
	objFile = fopen("obj.txt", "w");
	listingFile = fopen("listing.txt", "w");

	char line[256];
	char label[20];
	char opcode[20];
	char operand[20];
	char operandAddress[20];
	char comment[256];
	char lineAddr[20];   //the address of the current line
	char opcodeHex[20];  //the hexcode for an opcode
	char buffer[20]; //used as a buffer for string functions
	char symbolAddress[10]; //Holds the address of a symbol temporatily
	char currentInstruction[12];
	char textRecordLengthHex[5];

	char startAddress[20];
	

	char prefix[20]; //Holds the beginnning of a text record
	char suffix[80]; //Holds the object code of a text record.

	zeroOut(prefix, 20);
	zeroOut(suffix, 80);
	zeroOut(startAddress, 20);
	zeroOut(symbolAddress, 10);
	zeroOut(currentInstruction, 12);
	zeroOut(textRecordLengthHex, 5);

	int lineNumber = 1;



	//Flag for if line is a comment
	int lineIsComment;
	lineIsComment = 0;

	fgets(line, sizeof line, intermFile);
	splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);

	printf("LINE: %s\n", line);
	//begin
		//read first input line from intermediate file
		
		//if OPCODE = 'START' then
		if(strcmp(opcode, "START") == 0){
			//write listing file
			writeToListingFile(lineNumber, line);
			writeErrors(lineNumber);
			//read next input line
			
		}//end {if START}
		
		//Save the starting address
		strcpy(startAddress, operand);

		//write header record to object program
			fputs("H", objFile);
			fputs(toUpperCase(programName), objFile);
			for(int i = 0; i < 7 - strlen(programName); i++){
				fputs(" ", objFile);
			}
			for(int i = 0; i < 6 - strlen(operand); i++){
				fputs("0", objFile);
			}
			fputs(toUpperCase(operand), objFile);
			for(int i = 0; i < 6 - strlen(programLength); i++){
				fputs("0", objFile);
			}
			fputs(toUpperCase(programLength), objFile);
			fputs("\n", objFile);

		

		//initialize first text record
		appendString(prefix, "T");
		appendString(prefix, lengthenToSixBytes(buffer, lineAddr + 2));

		printf("%s", prefix);

		lineNumber++;
		fgets(line, sizeof line, intermFile);
		splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);


		//while OPCODE != END
		printf("= END: %d", strcmp(opcode, "END"));

		while(strcmp(opcode, "END") != 0){
		// for(int i = 0; i < 30; i++){
			printf("OPCODE: %s\n", opcode);
			printf("String length + 6: %d\n", strlen(suffix) + 6);
			//if this is not a comment line
			printf("LINE IS COMMENT: %d\n", lineIsComment);



			if(!lineIsComment){
				//search OPTAB for OPCODE
				//if found then
				printf("OPCODE: %s\n", opcode);


				if(OpTabContains(opcode)){
					//if there is a symbol in the operand field
					if(operand[0] > 60){
						//search SYMTAB for operand
						//if found then
						if(symTabContains(&SYMTAB, operand)){
							//store symbol value as operand address
							getSymbolAddress(&SYMTAB, operand, symbolAddress);
						}
						else if(containsComma(operand)){
							removeComma(operand);

							if(symTabContains(&SYMTAB, operand)){
								//store symbol value as operand address
								getSymbolAddress(&SYMTAB, operand, symbolAddress);
							}
							addBit(symbolAddress);
						

						}
						else{
							printf("COULDNT FIND %s\n", operand);
							//store 0 as operand address
							strcpy(symbolAddress, "0000");
							//set error flag, undefined tymbol

						}//end if {symbol}
					}
					else if(operand[0] > 47 && operand[0] < 58){
						printf("OPERAND: %s\n", operand);
						convertStringToHex(operand);
						printf("OPERAND: %s\n", operand);
						
						printf("buffer: %s\n", buffer);
					
						printf("SYMBOL ADDRESS: %s\n", lengthenToFourBytes(buffer, operand));
					}
			
					else{
						//store 0 as the operand address
						strcpy(symbolAddress, "0000");
					}
					//assemble the object code insreuction

					printf("APPENDING TO INSTRUCTION: ADDING %s to %s\n", opcodeHex + 2, currentInstruction);
					appendString(currentInstruction, opcodeHex + 2);
					appendString(currentInstruction, toUpperCase(symbolAddress));

					

				}//end {if opcode found}
				else{
					//SET ERROR FLAG
				}


				//else if OPCODE = 'BYTE' or 'WORD'
				if(strcmp(opcode, "BYTE") == 0){
					//convert constant to object code
					zeroOut(currentInstruction, 10);

					if(containsApostrophe(operand) && operand[0] == 'C'){
						printf("CONTAINS APOSTROPHE!\n\n");
						int i = 2;

						while(operand[i] != 0){

							if(operand[i] == '\'') break;
							if(strlen(suffix) + 6 > 60) break;

							printf("OPERAND: %s\n", operand);
							printf("OPERAND[i]: %c\n", operand[i]);

							convertCharToHex(operand + i, buffer);

							appendString(currentInstruction, toUpperCase(buffer));
							printf("CUR INT: %s\n", currentInstruction);
							
							i++;
						}
					}
					else if(containsApostrophe(operand) && operand[0] == 'X'){
						removeApostrophes(operand, buffer);
						appendString(currentInstruction, toUpperCase(buffer));
					}
					else{
						appendString(currentInstruction, operand);
					}


					
				}
				else if(strcmp(opcode, "WORD") == 0){

					zeroOut(currentInstruction, 10);
					lengthenToSixBytes(buffer, operand);
					appendString(currentInstruction, toUpperCase(buffer));
				}

				else if(strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0){

					while(strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0 || lineIsComment){
						printf("GETTING NEXT LINE");
						fgets(line, sizeof line, intermFile);
						lineNumber++;
						printf("LINE: %s\n",line);	
						splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);

					}

					zeroOut(currentInstruction, 10);
					sprintf(textRecordLengthHex, "%x", strlen(suffix) / 2);
					
					appendString(prefix, toUpperCase(textRecordLengthHex));
					
					fputs(prefix, objFile);
					fputs(suffix, objFile);
					fputs("\n", objFile);

					//initialize new text record
					zeroOut(prefix, 20);
					zeroOut(suffix, 80);
					zeroOut(symbolAddress, 10);
					zeroOut(textRecordLengthHex, 5);

					


					appendString(prefix, "T");
					appendString(prefix, lengthenToSixBytes(buffer, toUpperCase(lineAddr + 2)));

				
					
					splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);

					continue;

				}
				//if object code will not fit into the current text record
				else if(strlen(suffix) + 6 > 60){
					printf("TOOLONG**********************************\n");
					printf("String length + 6: %d\n", strlen(suffix) + 6);
					//write text record to object program
					sprintf(textRecordLengthHex, "%x", strlen(suffix) / 2);
					
					appendString(prefix, lengthenToTwoBytes(toUpperCase(textRecordLengthHex), buffer));
					
					fputs(prefix, objFile);
					fputs(suffix, objFile);
					fputs("\n", objFile);

					//initialize new text record
					zeroOut(prefix, 20);
					zeroOut(suffix, 80);
					zeroOut(symbolAddress, 10);
					zeroOut(textRecordLengthHex, 5);

					appendString(prefix, "T");
					appendString(prefix, lengthenToSixBytes(buffer, toUpperCase(lineAddr + 2)));

				}

				printf("CURRENT INSTRUCTION: %s\n", currentInstruction);
				appendString(suffix, currentInstruction);
				zeroOut(currentInstruction, 10);
				printf("PREFIX: %s\n", prefix);
				printf("SUFFIX: %s\n", suffix);


				fgets(line, sizeof line, intermFile);
				lineNumber++;
				splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);


			


			}//end if {not comment}
			else{

				fgets(line, sizeof line, intermFile);
				lineNumber++;
				splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);
			}
			//write listing line
			//read next input line


			

		}//end {while not END}

		sprintf(textRecordLengthHex, "%x", strlen(suffix) / 2);
		printf("textRecordLengthHex: %s\n", textRecordLengthHex);	
		appendString(prefix, lengthenToTwoBytes(toUpperCase(textRecordLengthHex), buffer));
		
		//write last text record to object program
		printf("LAST PREFIX: %s\n", prefix);
		printf("LAST SUFFIX: %s\n", suffix);

		fputs(prefix, objFile);
		fputs(suffix, objFile);
		fputs("\n", objFile);



		
		//write end record to object program
		fputs("E", objFile);
		lengthenToSixBytes(buffer, startAddress);
		fputs(buffer, objFile);
		//write last listing line
	//end pass 2

		

}

int main(){

	// printf("Starting Main...\n");

	// char line[256];
	// char label[20];
	// char opcode[20];
	// char operand[20];
	// char lineAddr[20];
	// char opcodeHex[20];
	// char comment[256];
	
	// printf("Creating Int...\n");

	// int lineIsComment = 0;

	// printf("Done Creating Int...\n");

	// char  test[256] = "0x1030:0xA4:0:WORD;ZERO    WORD    0";
 
 // 	printf("Starting SplitString2...\n");

	// splitString2(test, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);



	// printf("Line: %s\n\n", test);

	// if(lineIsComment) printf("LineIsComment: True\n");
	// else printf("LineIsComment: False\n");


	// printf("lineAddr: |%s|\n", lineAddr);
	// printf("opcodeHex: |%s|\n", opcodeHex);
	// printf("label: |%s|\n", label);
	// printf("opcode: |%s|\n", opcode);
	// printf("operand: |%s|\n", operand);
	// printf("comment: |%s|\n", comment);

	// for(int i = 0; i < 10; i++){
	// 	printf("lineAddr[%d]: %c\n", i, lineAddr[i]);
	// }



	FILE * file = fopen("./source.asm", "r");
	pass1(file);
	FILE * file2 = fopen("./interm.txt", "r");
	pass2(file2);


	// char symbol[10] = "THREEdfdfd";
	// char address[10];

	// zeroOut(address, 10);

	// getSymbolAddress(&SYMTAB, symbol, address);

	// printf("ADDRESS: %s|\n", address);

	return 0;


}




// This function will split a string into 4 different parts
// It uses " " or "\t" as delimiters, but will ignore conescutive spaces as delimiters
// when writing to the comment array.
void splitString2(char * line, char * lineAddr, char * opcode, char * label, char * opcodeHex, char * operand, char * comment, int * lineIsComment){


	*lineIsComment = 0;


	printf("Starting SplitString2...\n");

	int noLabel = 0;
	int writingTo = 0; //Represents the current char array being written to. label = 0, opcode = 1, operand = 2, comment = 3
	int inSpaces = 0; //represents that repetetive spaces are being found

	// set all elements to 0
	zeroOut(label, 20);
	zeroOut(opcode, 20);
	zeroOut(operand, 20);
	zeroOut(comment, 256);
	zeroOut(lineAddr, 8);
	zeroOut(opcodeHex, 8);

	int j = 0; //represents index of current argument
	int i = 0;


	if(line[0] == ' ' || line[0] == '.'){
		printf("THIS LINE IS COMMENT");
		*lineIsComment = 1;
		strcpy(comment, line);
		return;
	}

	while(line[i] != 0){
		
	
		
		// printf("On Char: %c at index %d\nWriting %c to %d at index %d\n\n", line[i], i, line[i], writingTo, j);
		if(writingTo == 0){
			if(line[i] == ':') lineAddr[j] = 0;
			else lineAddr[j] = line[i];
		}
		else if(writingTo == 1){
			if(line[i] == ':') opcodeHex[j] = 0;
			else opcodeHex[j] = line[i];
		}
		else if(writingTo == 2){
			if(line[i] == ':') operand[j] = 0;
			else operand[j] = line[i];
		}
		else if(writingTo == 3){
			if(line[i] == ';') opcode[j] = 0;
			else opcode[j] = line[i];
		}

		j++;

		if(line[i] == ':'){
			writingTo++;
			j = 0;
		}
		if(line[i] == ';'){
			break;
		}

		i++;
	}
}

// Turns all lowercase characters in a char array to uppercase.
char * toUpperCase(char * string){

	for(int i = 0; string[i] != 0; i++){		
		if(string[i] > 96 && string[i] < 123){
			printf("Changing %c to %c\n", string[i], string[i] - 32);
			string[i] = string[i] - 32;
		}
	}

	return string;
}
	

// Will append string2 to string1, assumes that string1 has memory allocated after its last non-null character
void appendString(char * string1, char * string2){
	int i;
	for(i = 0; i < strlen(string1); i++);

	for(int j = 0; string2[j] != 0; j++){
		string1[i] = string2[j];
		i++;
	}

	// printf("string1: %s\n", string1);
}



//Lengthens an address to 6 characters by adding zeroes to the beginning
char * lengthenToSixBytes(char * buffer, char * string){
	 
	zeroOut(buffer, 10);

	if(strlen(string) >= 6) return string;
	else{
		int numZeroes = 6 - strlen(string);

		
		for(int i = 0; i < numZeroes; i++){
			buffer[i] = '0';
		}
		appendString(buffer, string);
		
		return buffer;
	}
}

//Lengthens an address to 4 characters by adding zeroes to the beginning
char * lengthenToFourBytes(char * buffer, char * string){
	 
	zeroOut(buffer, 20);

	if(strlen(string) >= 4) return string;
	else{
		int numZeroes = 4 - strlen(string);

		
		for(int i = 0; i < numZeroes; i++){
			buffer[i] = '0';
		}
		appendString(buffer, string);
		
		return buffer;
	}
}

//Lengthens an address to 4 characters by adding zeroes to the beginning
char * lengthenToTwoBytes(char * string, char * buffer){
	 
	zeroOut(buffer, 10);

	if(strlen(string) >= 2) return string;
	else{
		int numZeroes = 2 - strlen(string);

		
		for(int i = 0; i < numZeroes; i++){
			buffer[i] = '0';
		}
		appendString(buffer, string);
		
		return buffer;
	}
}


//Returns 1 if the string contains an apostrophe
int containsApostrophe(char * string){
	for(int i = 0; string[i] != 0; i++){
		if(string[i] == '\'') return 1;
	}
	return 0;
}


//Returns 1 if the string contains an apostrophe
int containsComma(char * string){
	for(int i = 0; string[i] != 0; i++){
		if(string[i] == ',') return 1;
	}
	return 0;
}

// void convertToHexString(char * string, char * ){
// 	sprintf(textRecordLengthHex, "%x", strlen(suffix) / 2);
// }

void convertCharToHex(char * string, char * buffer){
	zeroOut(buffer, 20);

	sprintf(buffer, "%x", string[0]);
	
}

//Fomats an operand to remove the apostrophes
void removeApostrophes(char * string, char * buffer){
	zeroOut(buffer, 20);

	int i = 2;
	while(string[i] != 0){
		if(string[i] == '\''){
			string[i] = 0;
			break;
		}
		i++;
	}

	sprintf(buffer, "%s", string + 2);
}


//removes ",X" from a string
void removeComma(char * string){

	int i = 0;
	while(string[i] != 0){
		if(string[i] == ','){
			string[i] = 0;
			break;
		}
		i++;
	}


}

//adds a bit to an operand to indeicate indexing
void addBit(char * string){
	int number = (int)strtol(string, NULL, 16);
	number += 32768;

	sprintf(string, "%x", number);
	printf("%s\n", string);
}

void convertStringToHex(char * string){
	printf("StrinG: %s\n", string);
	int number = (int)strtol(string, NULL, 10);
	printf("NUMBER: %d\n", number);
	sprintf(string, "%x", number);
	printf("String: %s\n", string);
}