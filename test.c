#include <pthread.h>
#include <stdio.h>

void *worker(void *arg)
{
    (void)arg;
    printf("Thread is running!\n");
    return NULL;
}

int main(void)
{
    pthread_t thread;

    pthread_create(&thread, NULL, worker, NULL);
    pthread_join(thread, NULL);

    return 0;
}