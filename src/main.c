// Programme principal

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include "ht_data.h"
#include "hash_thread.h"

extern int selected_function;
extern int rndm_string_len;

// Available hash functions in the program
static const char *AVAILABLE_FUNCTIONS[] = {
  "md2","md4","md5",
  "mdc2",
  "sha1","sha224","sha256","sha384","sha512",
  "rmd160",
  "whirlpool",
  NULL
};

int main(int argc, char **argv){
  int i;
  int nb_thrd;
  double total_time;
  pthread_t *thrds;
  struct timeval beg, end;

  if(argc < 2){
    // usage ...
    exit(-1);
  }

  // Search if the function is available
  for(i = 0; AVAILABLE_FUNCTIONS[i] != NULL; i++){
    if(!strcmp(argv[1], (char *)AVAILABLE_FUNCTIONS[i])){
      selected_function = i;
      break;
    }
  }
  
  if(selected_function == -1){                              // Unknown function
    // function not found ...
    // usage ...
    exit(-2);
  }

  srand(time(0));                                           // Init random seed
  nb_thrd = get_nprocs();                                   // Number of proc
  rndm_string_len = (argc > 2) ? atoi(argv[2]) : 7;

  // Allocate threads necessary structures
  thrds = malloc(sizeof(pthread_t) * nb_thrd);

  printf("\n[+] Starting with : %d threads\n", nb_thrd);
  printf("[+] Length of random strings : %d\n\n", rndm_string_len);
  gettimeofday(&beg, NULL); // Start time

  // The threads start and we  waits for them to finish
  for(i = 0; i < nb_thrd; i++) pthread_create(&thrds[i], NULL, ht_run, NULL);
  for(i = 0; i < nb_thrd; i++) pthread_join(thrds[i], NULL);

  gettimeofday(&end, NULL); // Start time
  printf("DONE\n");

  total_time = (end.tv_sec-beg.tv_sec) + (end.tv_usec-beg.tv_usec)/1000000.0;
  printf("[+] TIME ELAPSED : %f secondes\n", total_time);
  exit(0);
}