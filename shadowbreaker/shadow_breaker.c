#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "shadow_breaker.h"
#include "utils.h"
#include "output_helpers.h"
#include "queue_r.h"

/******* OPTIONAL TEMPLATE CODE STARTS HERE -- MODIFY OR DELETE *******/

char *password;
int hashcount = 0;

/**
 * @brief Function to make a struct Info to contain arguments passed to thread
 * 
 * @param char *username - the username
 * @param cahr *pw_hash - the given hash code of the password
 * @param char *pw_prefix - the given prefix of the password
 * @param size_t thread_count - the number of threads
 * @return Info *user - a struct type 
 */
Info *make_info(char *username, char *pw_hash, char *pw_prefix, size_t thread_count)
{
  Info *user = malloc(sizeof(Info));
  user->username = strdup(username);
  user->pw_hash = strdup(pw_hash);
  user->pw_prefix = strdup(pw_prefix);
  user->thread_count = thread_count;
  return user;
}
/**
 * @brief Function to free the Info *
 * @param Info *user - the Info that needs to be freed
 */
void free_info(Info *user)
{
  free(user->username);
  free(user->pw_hash);
  free(user->pw_prefix);
  free(user);
}

/**
 * @brief Function to get start_password according to the prefix and start_index
 * - each thread should have a start_password to retrieve the password
 * - create a new string to store the prefix and initialise the string
 * - the string should be start_password
 * 
 * @param long start_index - the index that indicates the start position of the password
 * @param const char *prefix - the given prefix of the password
 * @param int unknown_count - the number of unknown letters
 * @return char * - the start_password for the current thread
 */
char *get_start_password(long start_index, const char *prefix, int unknown_count)
{
  int prefix_len = getPrefixLength(prefix);
  char *start_password = (char *)malloc(strlen(prefix) + 1);
  strncpy(start_password, prefix, prefix_len); // get known prefix
  char unknown[unknown_count + 1]; // array to store the unknown letters

  for (int i = 0; i < unknown_count; i++) // initilise 
    unknown[i] = 'a';
  
  setStringPosition(unknown, start_index);
  strcat(start_password, unknown); // concatenate them together
  return start_password;
}

/**
 * @brief Function to set up the successful password (returned later)
 * @param int len - the length of the password
 * @param char *ps - the genuine password
 * @return void
 */
void setPassword(int len, char *ps)
{
  password = (char *)malloc(len + 1);
  strcpy(password, ps);
}

/**
 * @brief Function to return the successful password
 * @return char * - the global password
 */
char *getPassword()
{
  return password;
}

/**
 * @brief Function to initialise the hashcount
 *  - print_parr_summary() needs to total hashcount
 *  - initialise it for the next password and next username
 * @return void
 */
void initialise_hashcount()
{
  hashcount = 0;
}

/**
 * @brief Function to update the hashcount
 * @long count - the hashcount for the current thread
 * @return void
 */
void increase_hashcount(long count)
{
  hashcount += count;
}

/**
 * @brief Function to return the hashcount for the current user and password
 * @return int
 */
int get_hashcount()
{
  return hashcount;
}

/******* OPTIONAL POSSIBLE TEMPLATE CODE __ENDS__ HERE *******/

/**
 * REQUIRED FUNCTION - This is the start of your threads.
 * @brief Function to retrieve the genuine password
 *  - figures out the subrange the current thread needs to manipulate
 *  - obtains the start_password for the current thread
 *  - calls proper print methods 
 *  - increments the start_password to obtain the password
 *  
 * @param void *attr - the argument contains all information that needed
 *        - username, pw_hash, pw_prefix from the stdin
 *        - the successful state: whether there is a thread succeed
 *        - thread_id allocated by the created process to identify each thread
 * @return void *
 */
void *thread_start_routine(void *attr)
{
  Info *user = (Info *)attr; // cast back
  char *username = user->username;
  const char *prefix = user->pw_prefix;
  char *pw_hash = user->pw_hash;
  size_t thread_count = user->thread_count;
  int thread_id = user->id; //
  int unknown_letter_count = strlen(prefix) - getPrefixLength(prefix);
  long start_index = 0;
  long count = 0; // the number of operations that the thread needs to do
  const char *salt_str = "cs2002";
  char *start_password = NULL;
  int result_for_print = 1; // for print_thread_parr_result()
  int hash_count = 0;

  getSubrange(unknown_letter_count, thread_count, thread_id, &start_index, &count);
  start_password = get_start_password(start_index, prefix, unknown_letter_count);
  print_thread_parr_start(pthread_self(), username, start_index, start_password);

  for (int i = 0; i < count; i++)
  {
    if (user->success == 1)
      break;
    struct crypt_data temp_data;
    temp_data.initialized = 0;
    const char *hashed_temp = crypt_r(start_password, salt_str, &temp_data);
    hash_count++; // the current iterations we have done

    if (strcmp(pw_hash, hashed_temp) == 0)
    { // if the trial password matches
      setPassword(strlen(start_password), start_password); // to return it later
      result_for_print = 0; // for print_thread_parr_result()
      user->success = 1; // update the flag to let other threads know
      break;
    }
    incrementString(start_password); // if not match, increment it for a new trial
  }

  if (user->success == 0) 
    result_for_print = 2;

  increase_hashcount(hash_count); // update the hash_count for print_parr_summary()
  print_thread_parr_result(pthread_self(), hash_count, result_for_print);
  free(start_password);
  return NULL;
}

/**
 * REQUIRED FUNCTION - Called by main to launch and manage threads.
 * 
 * @brief Function only for the thread launching point
 * - calls pthread_create(..., ..., &thread_start_routine, ...) 
 * - joins all threads
 * 
 * @param size_t thread_count - the number of threads
 * @param Info *user - the arguments and information of users that 
 *        needs to be passes to thread_start_routine()
 * @return int - 0 indicates successful
 */
int start(size_t thread_count, Info *user)
{
  pthread_t thread_id[thread_count];
  int err = 0;
  user->success = 0;
  print_parr_start_user(user->username);

  for (int i = 0; i < (int)thread_count; i++)
  {
    user->id = i + 1; // allocate thread_ids
    err = pthread_create(&thread_id[i], NULL, thread_start_routine, (void *)user);
    sleep(1);
    if (err)
    {
      printf("ERROR: return code from thread_create is %d \n", err);
      exit(1);
    }
  }

  for (int i = 0; i < (int)thread_count; i++)
  {
    pthread_join(thread_id[i], NULL);
  }

  return 0; // 0 indicates success
}
