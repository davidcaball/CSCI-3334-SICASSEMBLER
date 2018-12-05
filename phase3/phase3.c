//Pass 2


#include "phase2.h"


void pass2(){
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