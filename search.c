#include <stdio.h>
#include <unistd.h>

/*
make data structure to hold values read in from the inverted index file
write function to parse through the file

*/

int main(int argc, char **argv){
	
	while(1){
		char *a = malloc(1000);
		fgets(a, 1000, stdin);

		//add conditions for sa a
		if(strcmp(a, "q") == 0){
			break;
		}
		
		}
	}
	return 0;
}