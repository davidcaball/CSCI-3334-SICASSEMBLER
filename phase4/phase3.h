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
void * writeErrors(FILE * file, int lineNumber);
int containsApostrophe(char * string);
void * writeToListingFile(FILE * file, int lineNumber,char * line, char * currentInstruction, char * lineAddr, int lineIsComment);

void convertCharToHex(char * string, char * buffer);


// Used as buffers for outputting errors
char currentError[50];
char currentLine[300];

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
	zeroOut(currentError, 50);
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

	// printf("LINE: %s\n", line);
	//begin
		//read first input line from intermediate file
		
		//if OPCODE = 'START' then
		if(strcmp(opcode, "START") == 0){
			//write listing file
			writeToListingFile(listingFile, lineNumber, comment, currentInstruction, lineAddr, lineIsComment);
			writeErrors(listingFile, lineNumber);
			//read next input line
			
		}//end {if START}
		
		//Save the starting address
		strcpy(startAddress, operand);

		//write header record to object program
			fputs("H", objFile);
			fputs(toUpperCase(programName), objFile);

	

			int i = 0;
			while(i < 7 - strlen(programName)){
				fputs(" ", objFile);
				i++;
			}

			i = 0;
			while(i < 6 - strlen(operand)){
				fputs("0", objFile);
				i++;
			}
			fputs(toUpperCase(operand), objFile);

			i = 0;
			while(i < 6 - strlen(programLength)){
				fputs("0", objFile);
				i++;
			}

			fputs(toUpperCase(programLength), objFile);
			fputs("\n", objFile);

		//initialize first text record
		appendString(prefix, "T");
		appendString(prefix, lengthenToSixBytes(buffer, lineAddr + 2));


		// printf("%s", prefix);

		//Get the next line
		lineNumber++;
		fgets(line, sizeof line, intermFile);;
		splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);



		//while OPCODE != END
		while(strcmp(opcode, "END") != 0){


		// for(int i = 0; i < 30; i++){
			// printf("OPCODE: %s\n", opcode);
			// printf("String length + 6: %d\n", strlen(suffix) + 6);
			// //if this is not a comment line
			// printf("LINE IS COMMENT: %d\n", lineIsComment);



			if(!lineIsComment){
				//search OPTAB for OPCODE
				//if found then
				// printf("OPCODE: %s\n", opcode);


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

							if(!containsApostrophe(operand)){
								
								insertError(lineNumber, 3);
								//store 0 as operand address
								strcpy(symbolAddress, "0000");
								//set error flag, undefined tymbol
							}

						}//end if {symbol}
					}
					else if(operand[0] > 47 && operand[0] < 58){
			
						convertStringToHex(operand);		
					}
			
					else{
						//store 0 as the operand address
						strcpy(symbolAddress, "0000");
					}
					//assemble the object code insreuction

					// printf("APPENDING TO INSTRUCTION: ADDING %s to %s\n", opcodeHex + 2, currentInstruction);
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

					// printf("OPCODE IS %s\n", opcode);

					if(containsApostrophe(operand) && operand[0] == 'C'){
						// printf("CONTAINS APOSTROPHE!\n\n");
						int i = 2;


						while(operand[i] != 0){

							if(operand[i] == '\'') break;
							if(strlen(suffix) + 6 > 60) break;

							
							convertCharToHex(operand + i, buffer);

							appendString(currentInstruction, toUpperCase(buffer));
							// printf("CUR INT: %s\n", currentInstruction);
							
							i++;
						}
					}
					else if(containsApostrophe(operand) && operand[0] == 'X'){

						if(strlen(operand) % 2 == 0){
							insertError(lineNumber, 8);
						}
						
						removeApostrophes(operand, buffer);
						appendString(currentInstruction, toUpperCase(buffer));
					}
					else{
						appendString(currentInstruction, operand);
					}


					
				}
				else if(strcmp(opcode, "WORD") == 0){

					//Just format the output into 3 bytes
					zeroOut(currentInstruction, 10);
					lengthenToSixBytes(buffer, operand);
					appendString(currentInstruction, toUpperCase(buffer));
				}

				else if(strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0){

					while(strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0 || lineIsComment){
						

						
						writeToListingFile(listingFile, lineNumber, comment, currentInstruction, lineAddr, lineIsComment);
						writeErrors(listingFile, lineNumber);
						fgets(line, sizeof line, intermFile);
						lineNumber++;
					
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
				
					//write text record to object program
					sprintf(textRecordLengthHex, "%x", strlen(suffix) / 2);
					
					appendString(prefix, lengthenToTwoBytes(toUpperCase(textRecordLengthHex), buffer));
					
					//Write to object file
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

				// printf("CURRENT INSTRUCTION: %s\n", currentInstruction);
				appendString(suffix, currentInstruction);
				
				// printf("PREFIX: %s\n", prefix);
				// printf("SUFFIX: %s\n", suffix);

				//Out put line and errors to listing file
				writeToListingFile(listingFile, lineNumber, comment, currentInstruction, lineAddr, lineIsComment);
				writeErrors(listingFile, lineNumber);
				zeroOut(currentInstruction, 10);
				fgets(line, sizeof line, intermFile);
				lineNumber++;
				splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);


			


			}//end if {not comment}
			else{

				//Output line and errors to listing file
				writeToListingFile(listingFile, lineNumber, comment, currentInstruction, lineAddr, lineIsComment);
				writeErrors(listingFile, lineNumber);
				fgets(line, sizeof line, intermFile);
				lineNumber++;
				splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);
			}
			//write listing line
			//read next input line


			

		}//end {while not END}

		sprintf(textRecordLengthHex, "%x", strlen(suffix) / 2);

		appendString(prefix, lengthenToTwoBytes(toUpperCase(textRecordLengthHex), buffer));
		
		//write last text record to object program
		// printf("LAST PREFIX: %s\n", prefix);
		// printf("LAST SUFFIX: %s\n", suffix);

		fputs(prefix, objFile);
		fputs(suffix, objFile);
		fputs("\n", objFile);



		
		//write end record to object program
		fputs("E", objFile);
		lengthenToSixBytes(buffer, startAddress);
		fputs(buffer, objFile);

		//write last listing line
		writeToListingFile(listingFile, lineNumber, comment, currentInstruction, lineAddr, lineIsComment);
		writeErrors(listingFile, lineNumber);

		//end pass 2

		fclose(listingFile);
		fclose(objFile);
		fclose(intermFile);

		int numErrors = getNumErrors();
		
		if(numErrors == 0){
			printf("0 Errors Found. Assembled Succesfully..\n");
		}
		else{
			printf("%d Errors Found. Asemmbly Unscuccesful..\n", numErrors);
			remove("obj.txt");
		}

		clearErrorTable();

}
 
// int main(){


// }




// This function will split a string into 4 different parts
// It uses " " or "\t" as delimiters, but will ignore conescutive spaces as delimiters
// when writing to the comment array.
void splitString2(char * line, char * lineAddr, char * opcode, char * label, char * opcodeHex, char * operand, char * comment, int * lineIsComment){


	*lineIsComment = 0;


	// printf("Starting SplitString2...\n");

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
		// printf("THIS LINE IS COMMENT");
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

	strcpy(comment, line + i + 1);

	if(strcmp(opcode, "RSUB") == 0) zeroOut(operand, 20);
}

// Turns all lowercase characters in a char array to uppercase.
char * toUpperCase(char * string){
	int i = 0;
	while(string[i] != 0){		
		if(string[i] > 96 && string[i] < 123){
			// printf("Changing %c to %c\n", string[i], string[i] - 32);
			string[i] = string[i] - 32;
		}
		i++;
	}

	return string;
}
	

// Will append string2 to string1, assumes that string1 has memory allocated after its last non-null character
void appendString(char * string1, char * string2){
	
	int i = 0;
	while(i < strlen(string1)) i++;


	int j = 0;
	while(string2[j] != 0){
		string1[i] = string2[j];
		i++;
		j++;
	}

	// printf("string1: %s\n", string1);
}



//Lengthens an address to 6 characters by adding zeroes to the beginning
char * lengthenToSixBytes(char * buffer, char * string){
	 
	zeroOut(buffer, 10);

	if(strlen(string) >= 6) return string;
	else{
		int numZeroes = 6 - strlen(string);

		int i = 0;
		while(i < numZeroes){
			buffer[i] = '0';
			i++;
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

		int i = 0;
		while(i < numZeroes){
			buffer[i] = '0';
			 i++;
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

		int i = 0;
		while(i < numZeroes){
			buffer[i] = '0';
			i++;
		}
		appendString(buffer, string);
		
		return buffer;
	}
}


//Returns 1 if the string contains an apostrophe
int containsApostrophe(char * string){
	int i = 0;
	while(string[i] != 0){
		if(string[i] == '\'') return 1;
		i++;
	}
	return 0;
}


//Returns 1 if the string contains an apostrophe
int containsComma(char * string){
	
	int i = 0;
	while(string[i] != 0){
		if(string[i] == ',') return 1;
		i++;
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
	// printf("%s\n", string);
}


//Converts a character string that is in decimal to it's hex equivalent
void convertStringToHex(char * string){

	int number = (int)strtol(string, NULL, 10);
	sprintf(string, "%x", number);

}


//Will write to listing file in the format:

// "Line [#]: [Instructions address] [Instruction] [source line]"
void * writeToListingFile(FILE * file, int lineNumber,char * line, char * currentInstruction, char * lineAddr, int lineIsComment){
	
	zeroOut(currentLine, 300);

	if(lineIsComment){
		
		sprintf(currentLine, "Line %d: %s", lineNumber, line);
		fputs(currentLine, file);
	}
	else{
	
		sprintf(currentLine, "Line %d: %s %s %s", lineNumber, lineAddr, currentInstruction, line);
		fputs(currentLine, file);
	}
}


//Will take a file and a line number and will write any errors found on that line to the file
void * writeErrors(FILE * file, int lineNumber){
	int i = 0;
	int hasError = 0;
	while(i < MAXERRORS && ERRORS[i] != -1){
		if(ERRORS[i] == lineNumber){
			hasError =1;
			sprintf(currentError,"Error: %s\n", ERRORMESSAGES[ERRORS[i + 1]]);
			fputs(currentError, file);
			zeroOut(currentError, 50);
		}
		i+=2;
	}

	if(hasError) fputs("\n", file);
}
