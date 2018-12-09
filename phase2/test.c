#include <stdio.h>
#include <stdlib.h>
#include <string.h>




//Returns 1 if string has illegal characters, i.e. characters that are not a number or symbol
int hasIllegalCharacters(char * string){

	if(string[0] > 48 || string[0] < 58) return 1;
	
	for(int i = 0; i < strlen(string); i++){
		if(string[i] < 48 || (string[i] > 57 && string[i < 65]) || (string[i] > 90 && string[i] < 97)){
			return 1;
		}
	}

	return 0;
}


int main(){

	char test[10] = "HELLOTHERE";


	printf("HAS ILLEGAL CHARACTER: %d\n", hasIllegalCharacters(test));

	return 0;
}