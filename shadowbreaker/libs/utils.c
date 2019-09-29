#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "utils.h"

double getTime() 
{
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec + 1e-9 * t.tv_nsec;
}

double getCPUTime() 
{
  struct timespec t;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
  return t.tv_sec + 1e-9 * t.tv_nsec;
}

double getThreadCPUTime() 
{
  struct timespec t;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
  return t.tv_sec + 1e-9 * t.tv_nsec;
}

int getPrefixLength(const char *str) 
{
  // keep the dot and the string after the dot
  char *first_dot = strchr(str, '.'); 
  if (first_dot) // if the string is not empty (contains a dot)
    return first_dot - str;
  else // if the string contains no dots
    return strlen(str);
}

void setStringPosition(char *result, long n) 
{
  char *p = result + strlen(result) - 1; // the last char

  while (p >= result) {
    *p = 'a' + (n % 26);
    p -= 1;
    n /= 26;
  }
}

int incrementString(char *str) 
{
  assert(str);

  char *p = str + strlen(str) - 1;

  // find the last character after the prefix that is not a z
  while (p >= str && *p == 'z')
    p -= 1;;

  // if we found one, increment it
  if (p >= str) {

    // increment this character and move to the next one
    *p += 1;
    p  += 1;

    // and set all the remaining characters to 'a'
    while (*p) {
      *p = 'a';
      p +=1;
    }
    return 1;

  } else {

    // reached the end
    return 0;
  }
}

void getSubrange(int unknown_letter_count, size_t thread_count, int thread_id,
                 long *start_index, long *count) 
{
  int i;
  long max = 1, end_index;
  for (i = 0; i < unknown_letter_count; i++)
    max *= 26;
  *start_index = max *(thread_id - 1) / thread_count;
  end_index = max * thread_id / thread_count;
  if (end_index > max)
    end_index = max;
  *count = end_index - *start_index;
}