//Pass 2

#include "phase2.h"

void splitString2(char * line, char * lineAddr, char * opcode, char * label, char * opcodeHex, char * operand, char * comment, int * lineIsComment);

void pass2(FILE * intermFile){

	intermFile = fopen("interm.txt", "r");


	char line[256];
	char label[20];
	char opcode[20];
	char operand[20]; 
	char comment[256];
	char lineAddr[8];
	char opcodeHex[5];

	//Flag for if line is a comment
	int * lineIsComment;
	*lineIsComment = 0;

	fgets(line, sizeof line, intermFile);
	splitString2(line, lineAddr, opcode, label, opcodeHex, operand, comment, lineIsComment);


	//begin
		//read first input line from intermediate file
		//if OPCODE = 'START' then
			//write listing file
			//read next input line
		//end {if START}
		
		//write header record to object program
		//initialize first text record
		//while OPCODE != END
			//if this is not a comment line
				//search OPTAB for OPCODE
				//if found then
					//if there is a symbol in the operand field
						//search SYMTAB for operand
						//if found then
							//store symbol value as operand address
						//else
							//store 0 as operand address
							//set error flag, undefined tymbol
						//end if {symbol}
					//else
						//store 0 as the operand address
					//assemble the object code insreuction
				//end {if opcode found}
				//else if OPCODE = 'BYTE' or 'WORD'
					//convert constant to object code
				//if object code will not fit into the current text record
					//write text record to object program
					//initialize new text record
				//add object code to text record
			//end if {not comment}
			//write listing line
			//read next input line
		//end {while not END}
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
	char lineAddr[8];
	char opcodeHex[5];
	char comment[256];
	
	printf("Creating Int...\n");

	int lineIsComment = 0;

	printf("Done Creating Int...\n");

	char  test[256] = "0x3eb    0x48    CLOOP   JSUB    RDREC		READ INPUT RECORD";
 
 	printf("Starting SplitString2...\n");

	splitString2(test, lineAddr, opcode, label, opcodeHex, operand, comment, &lineIsComment);



	printf("Line: %s\n\n", test);

	if(lineIsComment) printf("LineIsComment: True\n");
	else printf("LineIsComment: False\n");


	printf("lineAddr: %s\n", lineAddr);
	printf("opcodeHex: %s\n", opcodeHex);
	printf("label: %s\n", label);
	printf("opcode: %s\n", opcode);
	printf("operand: %s\n", operand);
	printf("comment: %s\n", comment);

	for(int i = 0; i < 8; i++){
		printf("Value of lineaddr at %d: %d -- %c\n", i, lineAddr[i], lineAddr[i]);
	}
	
	
	
	return 0;


}




// This function will split a string into 4 different parts
// It uses " " or "\t" as delimiters, but will ignore conescutive spaces as delimiters
// when writing to the comment array.
void splitString2(char * line, char * lineAddr, char * opcode, char * label, char * opcodeHex, char * operand, char * comment, int * lineIsComment){

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
	zeroOut(opcodeHex, 5);

	int j = 0; //represents index of current argument
	int i = 0;

	//Check if line is a comment
	if(line[0] == ' ' || line[0] == '.'){

		

		printf("LINE IS COMMENT\n");
		*lineIsComment = 1;
		for(int i = 0; i < 256; i++){
			
			comment[i] = line[i];

			if(line[i] == '\n'){
				comment[i] = line[i];
				break;
			}
			
		}
		return;
	}

	i = 0;
	//loop untill null terminator
	while(line[i] != 0){
		printf("LineAddr at 0 is: %d -- %c\n", lineAddr[0], lineAddr[0]);
		printf("CURRENT CHAR: %c\n", line[i]);
		printf("WRITING TO %d\n", writingTo);

		// if(DEBUGGING == 1)
		// 	printf("At index %d, current char is: %d = %c. Writing to %d\n", i, line[i], line[i], writingTo);

		if(line[i] ==  ' ' || line[i] == 9){
		
			if(writingTo == 2 && !inSpaces){
				writingTo = 3;
				continue;
			}


			if(writingTo == 5){
				comment[j] = line[i];
				j++;
			}
			else if(inSpaces == 1){
				i++;
				continue;
			}
			else{ //null terminate appropriate char array
				if(writingTo == 0){
					printf("DONTG THIS at %d\n",j);

					lineAddr[j + 1] = 0;
				}
				else if(writingTo == 1) opcodeHex[j] = 0;

				else if(writingTo == 2) label[j + 1] = 0;
				else if(writingTo == 3) opcode[j + 1] = 0;
				else if(writingTo == 4) operand[j + 1] = 0;
				printf("INCREMENTING\n");
				writingTo++;
				j = 0;
			}

			

			// if(i == 0){
			// 	noLabel = 1;
			// 	label[0] = 0;
			// 	writingTo = 1;
			// }

			inSpaces = 1;  //indicate that the last character found was whitespace

		}	
		else{
			inSpaces = 0;

			if(writingTo == 0){
				printf("WRITING %d TO LINEADDR at index %d\n", line[i], j);
				lineAddr[j] = line[i];
				j++;
			}
			else if(writingTo == 1){
				opcodeHex[j] = line[i];
				j++;
			}
			else if(writingTo == 2){
				label[j] = line[i];
				j++;
			}
			else if(writingTo == 3){
				opcode[j] = line[i];
				j++;
			}
			else if(writingTo == 4){
				operand[j] = line[i];
				j++;
			}
			else if(writingTo == 5){
				comment[j] = line[i];
				j++;
			}

		}

		if(line[i + 1] == '\n' || line[i+1] == 0){
			if(writingTo == 5)
				comment[j] == 0;
		}

		i++;
	}

	//hotfix fix any newlines in the label
	// for(int i = 0; i < 20; i++){
	// 	if(label[i] == '\n' )
	// 		label[i] = 0;
	// 	if(opcode[i] == '\n' )
	// 		opcode[i] = 0;
	// 	if(operand[i] == '\n' )
	// 		operand[i] = 0;
	// }
}
