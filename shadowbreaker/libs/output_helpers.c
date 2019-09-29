/**
 * output_helpers.c -- print helpers for the shadow breaker practical
 * DO NOT MODIFY THIS FILE
 */
#include <assert.h>
#include <stdio.h>
#include "output_helpers.h"


void print_parr_start_user(char *username) 
{
	printf("Start %s\n", username);
}

void print_thread_parr_start(int threadId, char *username, long offset,
						   char *startPassword) 
{
    printf("Thread %u: Start %s at %ld (%s)\n",
		   threadId, username, offset, startPassword);
}

void print_thread_parr_result(int threadId, int hashCount, int result) 
{
    assert(result >= 0 && result <= 2);
    if (result == 0) {
	    printf("Thread %u: Stop after %d iterations (found)\n", threadId, hashCount);
    }
    else if (result == 1) {
	    printf("Thread %u: Stop after %d iterations (cancelled)\n", threadId, hashCount);
    }
    else {
	    printf("Thread %u: Stop after %d iterations (end)\n", threadId, hashCount);
    }
}

void print_parr_summary(char *username, char *password, int hashCount,
                        double timeElapsed, double totalCPUTime, int result) 
{
    assert(result >= 0 && result <= 2);
    printf("Password for %s ", username);
    if (result == 0) printf("is %s ", password);
    else printf("not found ");
    printf("(%d hashes in %.2f seconds)\n", hashCount, timeElapsed);
    printf("Total CPU time: %.2f seconds.\n", totalCPUTime);
    printf("CPU usage: %.2fx\n", totalCPUTime / timeElapsed);
}
