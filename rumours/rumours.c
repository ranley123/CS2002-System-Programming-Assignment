/**
 * @file rumours.c
 * @author 170011474
 * @brief 
 * 
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *swap(char *input, int index);
void getNewChild(FILE *file, char *input, int depth, int len, int index);
char *getInput(int argc, char *argv[]);

int main(int argc, char *argv[])
{
   while (argc <= 1) // check the input
   {
      printf("ERROR: expected a string input \n");
      return 0;
   }

   char *input = getInput(argc, argv);
   int len = strlen(input);
   int index = 0;
   int max_depth = (len - 1) / 2;
   FILE *file = fopen("rumours.out", "w");
   char buffer[len + 1];

   strcpy(buffer, input);
   if (file == NULL)
   {
      perror("open failed");
      exit(EXIT_FAILURE);
   }

   getNewChild(file, input, max_depth, len, index);
   free(input);
   fclose(file);
   return 0;
}

/**
 * @brief Function to get input string from the current argv arguments
 * - concatenate each argument to the current input string
 * 
 * @param int argc - the size of argv[]
 * @param char *argv[] - the array of arguments
 * @return char * - input string
 */
char *getInput(int argc, char *argv[])
{
   int cur_len = 0;
   char *input = (char *)malloc(cur_len); // allocate temporary space to it

   for (int i = 1; i < argc; i++)
   {
      input = (char *)realloc(input, (cur_len + strlen(argv[i]))); // realloc new memory
      strcat(input, argv[i]);
      strcat(input, " "); // add a space to form a new sentence string
   }
   return input;
}

/**
 * @brief Function to swap two indices in the string according to specific indices
 * 
 * @param char *input - the string which needs modification
 * @param int index - the index recording the first index to be modified
 * @return char * - the modified string
 */
char *swap(char *input, int index)
{
   int first = index;
   int second = strlen(input) - index - 2; // get the second index that needs modification

   char temp = input[first]; // swap
   input[first] = input[second];
   input[second] = temp;

   return input;
}

/**
 * @brief A recursion function to fork a new child and perform the telephone functionality
 * 
 * @param char *input - the input string that needs modifications.
 * @param int depth - the current depth; 0 is the threshold to control the times of recursion.
 * @param int len - the length of char *input
 * @param int index - the current index that needed to be passed to swap()
 * @return void
 */
void getNewChild(FILE *file, char *input, int depth, int len, int index)
{
   if (depth == 0) // the base case
      return;
   int child = 0;
   int fd[2];
   char buffer[len + 1];

   strcpy(buffer, input);
   pipe(fd);
   child = fork();

   if (child == 0)
   { // child
      close(fd[1]);
      read(fd[0], buffer, sizeof(buffer)); // read new string from the pipe
      printf("New process: %i, parent: %i \n", getpid(), getppid());
      printf("pid: %i received string: %s \n", getpid(), buffer);

      getNewChild(file, buffer, depth - 1, len, index + 1);
      fprintf(file, "pid: %i received string: %s \n", getpid(), buffer);
      exit(child);
   }
   else if (child > 0)
   { // parent
      close(fd[0]);
      strcpy(buffer, swap(buffer, index)); // update the string after modification
      printf("pid: %i Swapped indices %i, %i \n", getpid(), index, len - index - 2);
      fprintf(file, "pid: %i Swapped indices %i, %i \n", getpid(), index, len - index - 2);
      write(fd[1], buffer, strlen(buffer) + 1); // write the new string to the pipe

      wait(NULL); // wait for the child to terminate
   }
   else
   {
      printf("ERROR: fork failed");
      exit(0);
   }
}
