#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *worker(void *data){
    sleep(1);
    if(data){
        printf("current: %u, joins: %u \n", pthread_self(), (pthread_t)data);
        pthread_join((pthread_t)data, NULL);
    }
    return NULL;
}

int main(){
    pthread_t thread = (pthread_t) NULL;
    printf("the main thread %u \n",pthread_self());
    for(int i = 0; i < 5; i++){
        // print out the current index and the current thread
        printf("the %i iteration %u thread\n", i, thread);
        pthread_create(&thread, NULL, worker, (void *)thread);
        // sleep(1);
    }
    printf("the %i iteration %u thread\n", 5, thread);
    pthread_join(thread, NULL);
    printf("current: %u, joins: %u \n", pthread_self(), (pthread_t)thread);
}