/**
 * Header to link and launch YOUR start function from _main.c
 */

#include <stddef.h>

#define DEFAULT_USERNAME_LENGTH 9
#define DEFAULT_PW_HASH_LENGTH 14
#define DEFAULT_PW_PREFIX_LENGTH 14
typedef struct {
  char *username;
  char *pw_hash;
  char *pw_prefix;
  size_t thread_count;
  int id;
  int success;
} Info;

typedef struct {
    Info *user;
    long start_index;
    char *start_password;
} Thread_attr;

Info *make_info(char *username, char *pw_hash, char *pw_prefix, size_t thread_count);
void free_info(Info *user);

char *get_start_password(long start_index, const char *prefix, int unknown_count);
// Function called from shadow_breaker main()
void *thread_start_routine(void *attr);

int start(size_t thread_count, Info *user);
void setPassword(int len, char *ps);
char *getPassword();
void initialise_hashcount();
void increase_hashcount(long count);
int get_hashcount();