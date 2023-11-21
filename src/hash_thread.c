// Hash thread routine

#include <stdlib.h>
#include <stdio.h>
#include <openssl/md5.h>

#include "ht_data.h"

static const char CHARSET[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const int CHARSET_LEN = 62;

int finished = 0;
int rndm_string_len;
int hash_length = 16;
int selected_function;
char *result;

// Verify is a certain hash is fitting the juggling attack
static int hash_ok(unsigned char *hash){
  if(hash[0] != 0x0e) return 0;           // Must begin with 0x0e
  
  for(int i = 1; i < hash_length; i++){   // The following is letterless
    if((hash[i]&(0xf))>>1 > 4) return 0;
    if(hash[i]>>5 > 4) return 0;
  }
  return 1;
}

// Generate a random string
void randomize_string(char *string){
  for(int i = 0; i < rndm_string_len; i++){
    string[i] = CHARSET[rand()%CHARSET_LEN];
  }
}

// Hash thread routine
void *ht_run(void *args){
  int i;
  char rndm_string[rndm_string_len];
  unsigned char hash[hash_length];
  char *string = malloc(sizeof(char)*2*hash_length+1);
  MD5_CTX ctx;

  // While string is not found, the thread search a candidate
  while(!finished){
    // Generate a new random string to hash
    randomize_string(rndm_string);

    // Hashing the string
    if(selected_function == 2){
      MD5_Init(&ctx);
      MD5_Update(&ctx, rndm_string, rndm_string_len);
      MD5_Final(hash, &ctx);
    }

    // Verify is the hash is fitting the juggling attack
    if(hash_ok(hash)){
      finished++;
      for (i=0; i < hash_length; i++) {
        sprintf(string + 2*i, "%02x", hash[i]);
      }
      printf("[+] Hash found : %s\n", string);
      printf("[+] Source string : %s\n", rndm_string);
    }
  }

  return NULL;  // No need of return as it passes the result globally
}