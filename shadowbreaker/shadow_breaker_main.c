#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <crypt.h>
#include "../libs/utils.h"
#include <string.h>
#include "shadow_breaker.h"
#include "output_helpers.h"

#define DEFAULT_THREADS 4
#define DEFAULT_READ_SIZE 128
#define DEFAULT_USERNAME_LENGTH 9
#define DEFAULT_PW_HASH_LENGTH 14
#define DEFAULT_PW_PREFIX_LENGTH 14

void usage() 
{
  fprintf(stderr, "\n  ./shadow_breaker [thread_count] < pwd_hashes.txt\n\n");
  exit(1);
}

int main(int argc, char **argv) 
{
  size_t thread_count = DEFAULT_THREADS;
  char username[DEFAULT_USERNAME_LENGTH] = "";
  char pw_hash[DEFAULT_PW_HASH_LENGTH] = "";
  char prefix[DEFAULT_PW_PREFIX_LENGTH] = "";
  char temp_username[128] = ""; // for input check
  char temp_pw_hash[128] = ""; // for input check
  char temp_pw_prefix[128] = ""; // for input check
  double start_time = getTime();
  double start_cpu_time = getCPUTime();
  if (argc != 2)
    usage();

  if (argc > 1) {
    if (1 != sscanf(argv[1], "%lu", &thread_count) || thread_count < 1)
      usage();
    if(thread_count > 13){
      printf("The thread_count should be less than 14 \n");
      exit(1);
    }
  }

  while(scanf("%s", temp_username) != EOF){
    if(strlen(temp_username) > 8){ // check if it is invalid
      printf("the length of username should be between 1 and 8 \n");
      exit(1);
    }
    scanf("%s", temp_pw_hash);
    if(strlen(temp_pw_hash) != 13){
      printf("the length of hash should be 13 \n");
      exit(1);
    }
    scanf("%s", temp_pw_prefix);
    if(strlen(temp_pw_prefix) != 8){
      printf("the length of prefix should be 8 \n");
      exit(1);
    }

    // after we make sure all inputs are valid
    strcpy(username, temp_username);
    strcpy(pw_hash, temp_pw_hash);
    strcpy(prefix, temp_pw_prefix);

    Info *user = make_info(username, pw_hash, prefix, thread_count);
    initialise_hashcount();
    printf("\n");
    int ret = start(thread_count, user);
    free_info(user);

    double elapsed = getTime() - start_time;
    double total_cpu_time = getCPUTime() - start_cpu_time;
    char *password = getPassword();

    print_parr_summary(username, password, get_hashcount(),
                          elapsed, total_cpu_time, ret);

    free(password);
  }
  return 0;
}
