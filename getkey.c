#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int encrypt(unsigned char *plaintext, int plainlen, unsigned char *key,unsigned char *iv, unsigned char *ciphertext);
void padWord(char *word);
void handleErrors(void);

int main (int argc, char** argv)
{
        unsigned char iv[17];
        memset(iv, 0 ,17); //Allocates 0's
        unsigned char *plain = "This is a top secret."; //Initialize our plaintext
        //unsigned char cipher[] = {0x8d, 0x20, 0xe5, 0x05, 0x6a, 0x8d, 0x24, 0xd0, 0x46, 0x2c, 0xe7, 0x4e, 0x49, 0x04, 0xc1, 0xb5, 0x13, 0xe1, 0x0d, 0x1d, 0xf4, 0xa2, 0xef, 0x2a, 0xd4, 0x54, 0x0f, 0xae, 0x1c, 0xa0, 0xaa, 0xf9}; // Initialize our cipher
        unsigned char *cipher =  "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";
        unsigned char encryptBuf[100]; //Initialize a place to store ciphertext output
        if(argc < 2)
        {
            fprintf(stderr, "No word list argument provided");
            return 1;
        }
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();
        OPENSSL_config(NULL);
        FILE * wordList = fopen(argv[1], "r");
        if(wordList == NULL)
        {
            fprintf(stderr, "File not found");
            return 1;
        }
        
        char word[50];
        //memset(word, ' ' , 17); //Set everything to spaces so remaining pad will be spaces
        //size_t len = 0;
        //ssize_t read;
        
        int cipherLength;
        while(fscanf(wordList, "%s\n", word ) != EOF)
        {
            int i = strlen(word);
            if(i > 16)
                continue;
            else 
                padWord(word);
            
            cipherLength = encrypt(plain,strlen(plain), word, iv, encryptBuf);
            
            unsigned char crypto[50];
            for(int i = 0 ; i < cipherLength; i++)
            {
                sprintf(crypto+i*2, "%02x", encryptBuf[i]);
            }
            crypto[cipherLength*2]  = '\0';
        
            //printf("\n Word: %s\nOutput: %s \n Cipher: %s\n",word, crypto, cipher);
            if(0 == strcmp(crypto, cipher))
            {
                printf("Key found! The key is: %s\n", word);
                EVP_cleanup();
                ERR_free_strings();
                return 0;
            }
            //memset(word, ' ' , 16);//Return word back to all spaces
        }
    printf("\nKey not found!\n");
	return 0;
}


void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int encrypt(unsigned char *plaintext, int plainlen, unsigned char *key, unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int cipherlen;

  if(!(ctx = EVP_CIPHER_CTX_new()))
      handleErrors();

  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors();

  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plainlen))
    handleErrors();
  cipherlen = len;

  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  cipherlen += len;

  EVP_CIPHER_CTX_free(ctx);

  return cipherlen;
}

 
void padWord(char *word)
{
    int l = (int)strlen(word);
    for(;l<16;l++)
    {
        word[l]=' ';
    }
    word[16] = '\0';
}
