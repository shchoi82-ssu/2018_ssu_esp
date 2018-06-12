#include <stdio.h>
#include <pthread.h>

int shared_data;

void *thr_fn1(void *arg)
{
    for(int i = 0 ; i < 10000000 ; i++) 
        shared_data = shared_data + 1;

    return((void *)NULL);
}

void *thr_fn2(void *arg)
{
    for(int i = 0 ; i < 10000000 ; i++) 
        shared_data = shared_data + 1;
    return((void *)NULL);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
    if (err != 0)
        fprintf(stderr, "can’t create thread 1");
    err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
    if (err != 0)
        fprintf(stderr, "can’t create thread 2");
    err = pthread_join(tid1, NULL);
    if (err != 0)
        fprintf(stderr, "can’t join with thread 1");
    err = pthread_join(tid2, NULL);
    if (err != 0)
        fprintf(stderr, "can’t join with thread 2");

    printf("shared_data:%d\n", shared_data);
    return 0;
}
