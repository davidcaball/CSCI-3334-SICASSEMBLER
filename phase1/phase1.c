#include <stdio.h>
#include <stdlib.h>


// Command function Prototypes
void commandHelp();
void commandLoad();
void commandDebug();
void commandDump();
void commandAssemble();
void commandDirectory();
void commandExit();
void commandExecute();

void splitString(char * input, char args[3][30]);
void getCommand(char * string, char * command);
int isEqual(char * string1, char * string2);
void zeroOut(char * string, int length);


int main()
{
        char input[20]; // char array for user input
        char args[3][30];

        int running = 1; //running flag

        // Print a welcome message
        printf("Hello, welcome to the command line!\n");

        while(running){
                //gets an input of 20 characters from stdin
                fgets(input, 200, stdin);

                // call splitstring passingin the input and the string array to store the arguments
                // the command will be stored in args[0]
                splitString(input, args);


                // compare the command to known commands
                if(isEqual(args[0], "help") == 0)
                        commandHelp();
                else if(isEqual(args[0], "directory") == 0)
                        commandDirectory();
                else if(isEqual(args[0], "exit") == 0){
                        commandExit();
                        running = 0;
                }
                else if(isEqual(args[0], "debug") == 0){
                        commandDebug();
                }
                else if(isEqual(args[0], "assemble") == 0){
                        commandAssemble();
                }
                else if(isEqual(args[0], "execute") == 0){
                        commandExecute();
                }
                else if(isEqual(args[0], "load") == 0){
                        commandLoad();
                }

                // Fallback
                else printf("Command Not Recongized..\n");

        }

        return 0;
}



                                        


// ********************************************************
//						COMMANDS				
// ********************************************************
void commandHelp(){
	printf("*********************************** Help *********************************** \n\n");
	printf("load [filename]: Call load function to specified file\n");
	printf("execute: Call computer simulation to execture the program previously in memory\n");
	printf("debug: Allows you to exeute in debug mode\n");
	printf("dump start end: Call the dump function, passing the values of start and end.\n");
	printf("help: print out a list of available commands \n");
	printf("assemble [filename]: assemble an SIC assembly language program into a load module and store it in a file\n");
	printf("directory: list the files of the current directory\n");
	printf("exit: exit the simulator\n");
	printf("**************************************************************************** \n\n");


}

void commandExit(){
	printf("\n\nThank you!\n\n");	
}


void commandExecute(){
	printf("Sorry, this command is not available yet");	
}

void commandDirectory(){
	printf("\n");	
	system("ls");
	printf("\n");	
}

void commandLoad(){
	printf("Sorry, this command is not available yet\n");	
}

void commandDebug(){
	printf("Sorry, this command is not available yet\n");	
}

void commandDump(){
	printf("Sorry, this command is not available yet\n");	
}

void commandAssemble(){
	printf("Sorry, this command is not available yet\n");	
}







// ********************************************************
//						UTILTY FUNCTIONS				
// ********************************************************


// isEqual() takes 2 strings as arguments, it will compare every character
// it will return 0 if they are equal, 1 if string1 > string2, and -1 if string2 > string1
// wil return -2 if the string was not long enough to be recognized (min length 3)
int isEqual(char * string1, char * string2){
	int i = 0;
	while(string1[i] > 64){ // While there is still letter values

		if(string1[i] > string2[i]) //character comparisons
			return 1;
		else if(string1[i] < string2[i])
			return -1;
		i++;
	}
	if(i > 2) return 0; //make sure string is long enough
	return -2;
}

// splitString() takes a string input and a string array, will split
// a string using spaces as delimiters and assign the individual values
// to the elements of the string array
void splitString(char * input, char args[3][30]){
	int j= 0; 							// j = the current argument thats being written to
	int k = 0;							// k = the position of the current argument that's being written in
	int i = 0;							// i = index of the input
	while(input[i] != 0){ 				// Loop through the input string
		if(input[i] == ' '){ 			// If a space is found, move to net argument
			args[j][k] = 0; 			// Null terminator
			k = 0; 						// Reset arg index back to 0
			j++; 						// Increment te argument
			if(j > 2) break; 			// Break if over 3 arguments
			continue;
		}
		args[j][k] = input[i];			// Write the value to the argument
		k++;							// Increment index
		i++;							//Increment index in the input
	}

}

// zeroOut() takes 2 arguments, a char array and an int, it will assign
// 0 to every address up to that length away from the pointer
void zeroOut(char * string, int length){
	int i = 0;
	while(i < length){
		string[i] = 0;
		i++;
	}
}