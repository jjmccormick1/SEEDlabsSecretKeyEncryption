#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char** argv) 
{printf("here");
        FILE * wordList = fopen(argv[1], "r");
        
        if(wordList == NULL)
        {
            fprintf(stderr, "File not found");
            return 1;
        }
        
        char * word = malloc(17 * sizeof(char));
        //memset(word, ' ' , 17); //Set everything to spaces so remaining pad will be spaces
        //size_t len = 0;
        //ssize_t read;
        //unsigned char * plainDecrypt = calloc(21, sizeof(char*)); //Initialize a place to store plaintext output. Sized for our 21 character string
        
        while(fgets(word, 17, wordList) )
        {
            
            printf("here:%s\n", word);
        }
        return 0;
}
