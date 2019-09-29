#ifndef OUTPUT_HELPERS_
#define OUTPUT_HELPERS_

/**
 * output_helpers.h - A library to facilitate shadow_breaker output.
 */


/**
 * Prints the username associated with the password that will be broken
 *
 * @username: the username of the task the thread is working on
 *
 * THIS FUNCTION SHOULD ONLY BE USED FOR SHADOW_BREAKER_PARR
 */
void print_parr_start_user(char *username);


/**
 * Prints out a start message as a thread begins to process a task
 *
 * @threadId: the id of the worker thread
 * @username: the username associated with the password being broken
 * @offset: the offset from which this thread will work on
 * @startPassword: the starting password of the range/set for this thread
 *
 * THIS FUNCTION SHOULD ONLY BE USED FOR SHADOW_BREAKER_PARR
 */
void print_thread_parr_start(int threadId, char *username, long offset,
						   char *startPassword);


/**
 * Prints out a thread's success/failure result
 *
 * @threadId: the id of the worker thread
 * @hashCount: the number of hashes computed by the thread
 * @result: 0 - the password was broken by this thread
 * 			1 - the thread was stopped early due to success by another thread
 *			2 - the thread has finished searching in the range it was given and
 *				failed to find the password
 *
 * THIS FUNCTION SHOULD ONLY BE USED FOR SHADOW_BREAKER_PARR
 */
void print_thread_parr_result(int threadId, int hashCount, int result);


/**
 * Print the summary of the parallel shadow_breaker task.
 *
 * @username: the username associated with the password
 * @password: the password for the user if found
 * @hashCount: the total number of hashes performed by all threads
 * @timeElapsed: total time elapsed
 * @totalCPUTime: total CPU time used
 * @result: 0 for success, or 1 for failure.
 *
 * THIS FUNCTION SHOULD ONLY BE USED FOR SHADOW_BREAKER_PARR
 */
void print_parr_summary(char *username, char *password, int hashCount,
                        double elapsedTime, double totalCPUTime, int result);

#endif
