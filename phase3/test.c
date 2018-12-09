#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void addBit(char * string){
	int number = (int)strtol(string, NULL, 16);
	number += 32768;

	sprintf(string, "%x", number);
	printf("%s\n", string);
}


int main(){

	char test[11] = "1039";
	
	addBit(test);

	return 0;
}