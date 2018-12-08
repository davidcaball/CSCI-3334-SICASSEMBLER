//Pass 2

#include "phase2.h"

void splitString2(char * line, char * lineAddr, char * opcode, char * label, char * opcodeHex, char * operand, char * comment, int * lineIsComment);
char *  toUpperCase(char * string);
void appendString(char * string1, char * string2);
char * lengthenToSixBytes(char * buffer, char * string);
char * lengthenToFourBytes(char * buffer, char * string);


void pass2(FILE * intermFile){

	FILE * objFile;

	intermFile = fopen("interm.txt", "r");
	objFile = fopen("obj.txt", "w");

	char line[256];
	char label[20];
	char opcode[20];
	char operand[20];
	char operandAddress[20];
	char comment[256];
	char lineAddr[20];   //the address of the current line
	char opcodeHex[20];  //the hexcode for an opcode
	char buffer[10]; //used as a buffer for string functions
	char symbolAddress[10]; //Holds the address of a symbol temporatily
	char currentInstruction[10];
	char textRecordLengthHex[5];

	char prefix[9]; //Holds the beginnning of a text record
	char suffix[80]; //Holds the object code of a text record.

	zeroOut(prefix, 9);
	zeroOut(suffix, 80);
	zeroOut(symbolAddress, 10);
	zeroOut(currentInstruction, 10);
	zeroOut(textRecordLengthHex, 5);

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
				//TODO
			//read next input line
			
		}//end {if START}
		


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

		fgets(line, sizeof line, intermFile);
		splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);


		//while OPCODE != END
		printf("= END: %d", strcmp(opcode, "END"));
		while(strcmp(opcode, "END") != 0){
		// for(int i = 0; i < 11; i++){
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
						//else
						else{
							printf("COULDNT FIND %s\n", operand);
							//store 0 as operand address
							strcpy(symbolAddress, "0000");
							//set error flag, undefined tymbol
						}//end if {symbol}
					}
					//else
					else{
						//store 0 as the operand address
						strcpy(symbolAddress, "0000");
					}
					//assemble the object code insreuction
					appendString(currentInstruction, opcodeHex + 2);
					appendString(currentInstruction, toUpperCase(symbolAddress));

					

				}//end {if opcode found}
				//else if OPCODE = 'BYTE' or 'WORD'
				else if(strcmp(opcode, "BYTE") == 0 || strcmp(opcode, "WORD") == 0){
					//convert constant to object code
					appendString(currentInstruction, opcodeHex + 2);
					lengthenToFourBytes(buffer, operand);
					appendString(currentInstruction, toUpperCase(buffer));
				}
				//if object code will not fit into the current text record
				if(strlen(suffix) + 6 > 60){
					printf("TOOLONG**********************************\n");
					printf("String length + 6: %d\n", strlen(suffix) + 6);
					//write text record to object program
					sprintf(textRecordLengthHex, "%x", strlen(suffix) / 2);
					appendString(prefix, toUpperCase(textRecordLengthHex));
					fputs(prefix, objFile);
					fputs(suffix, objFile);
					fputs("\n", objFile);

					//initialize new text record
					zeroOut(prefix, 9);
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

			}//end if {not comment}
			//write listing line
			//read next input line


			fgets(line, sizeof line, intermFile);
			splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);



		}//end {while not END}


		//write last text record to object program
		//write end record to object program
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
		*lineIsComment = 1;
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
	 
	zeroOut(buffer, 10);

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