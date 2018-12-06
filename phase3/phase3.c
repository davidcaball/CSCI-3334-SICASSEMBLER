//Pass 2

#include "phase2.h"

void splitString2(char * line, char * lineAddr, char * opcode, char * label, char * opcodeHex, char * operand, char * comment, int * lineIsComment);

void pass2(FILE * intermFile){

	intermFile = fopen("interm.txt", "r");


	char line[256];
	char label[20];
	char opcode[20];
	char operand[20];
	char operandAddress[20] 
	char comment[256];
	char lineAddr[20];
	char opcodeHex[20];

	//Flag for if line is a comment
	int lineIsComment;
	lineIsComment = 0;

	fgets(line, sizeof line, intermFile);
	splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);


	//begin
		//read first input line from intermediate file
		
		//if OPCODE = 'START' then
		if(strcmp(opcode, "START") == 0){
			//write listing file
				//TODO
			//read next input line
			fgets(line, sizeof line, intermFile);
			splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);
		}//end {if START}
		


		//write header record to object program
			//TODO
		//initialize first text record
			//TODO

		//while OPCODE != END
		while(strcmp(opcode, "END") != 0){
			//if this is not a comment line
			if(!lineIsComment){
				//search OPTAB for OPCODE
				//if found then
				if(opTabContains(opcode)){
					//if there is a symbol in the operand field
					if(operand[0] > 60){
						//search SYMTAB for operand
						//if found then
						if(symTabContains(operand)){
							//store symbol value as operand address
								//TODO
						}
						//else
						else{
							//store 0 as operand address
							//set error flag, undefined tymbol
						}//end if {symbol}
					}
					//else
					else{
						//store 0 as the operand address
					}
					//assemble the object code insreuction



				}//end {if opcode found}
				//else if OPCODE = 'BYTE' or 'WORD'
				else if(strcmp(opcode, "BYTE") ==0 || strcmp(opcode, "WORD") == 0){
					//convert constant to object code
				}
				//if object code will not fit into the current text record
					//write text record to object program
					//initialize new text record
				//add object code to text record
			}//end if {not comment}
			//write listing line
			//read next input line
		}//end {while not END}
		//write last text record to object program
		//write end record to object program
		//write last listing line
	//end pass 2
}

int main(){

	printf("Starting Main...\n");

	char line[256];
	char label[20];
	char opcode[20];
	char operand[20];
	char lineAddr[20];
	char opcodeHex[20];
	char comment[256];
	
	printf("Creating Int...\n");

	int lineIsComment = 0;

	printf("Done Creating Int...\n");

	char  test[256] = "0x1030:0xA4:0:WORD;ZERO    WORD    0";
 
 	printf("Starting SplitString2...\n");

	splitString2(test, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);



	printf("Line: %s\n\n", test);

	if(lineIsComment) printf("LineIsComment: True\n");
	else printf("LineIsComment: False\n");


	printf("lineAddr: |%s|\n", lineAddr);
	printf("opcodeHex: |%s|\n", opcodeHex);
	printf("label: |%s|\n", label);
	printf("opcode: |%s|\n", opcode);
	printf("operand: |%s|\n", operand);
	printf("comment: |%s|\n", comment);

	// for(int i = 0; i < 10; i++){
	// 	printf("lineAddr[%d]: %c\n", i, lineAddr[i]);
	// }

	
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
		lineIsComment = 1;
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
