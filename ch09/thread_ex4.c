#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void *thr_fn(void *arg)
{
    int count = 0; 
    while(1){
        sleep(1);
        printf("%d\n", ++count);
    }
}

int main(void)
{
    int err;
    pthread_t tid;
    int ret;
    err = pthread_create(&tid, NULL, thr_fn, NULL);
    if (err != 0){
        fprintf(stderr, "can’t create thread");
    }
    sleep(5);
    pthread_cancel(tid);
    err = pthread_join(tid, (void *)&ret);
    if (err != 0){
        fprintf(stderr, "can’t join with thread");
    }

    if(ret == PTHREAD_CANCELED)
        printf("PTHREAD_CANCELED\n");
    return 0;
}






