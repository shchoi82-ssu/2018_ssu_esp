#include <stdio.h>
#include <pthread.h>

int shared_data;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *thr_fn1(void *arg)
{
    for(int i = 0 ; i < 10000000 ; i++){ 
        pthread_mutex_lock(&mtx);
        shared_data = shared_data + 1;
        pthread_mutex_unlock(&mtx);
    }

    return((void *)NULL);
}

void *thr_fn2(void *arg)
{
    for(int i = 0 ; i < 10000000 ; i++){ 
        pthread_mutex_lock(&mtx);
        shared_data = shared_data + 1;
        pthread_mutex_unlock(&mtx);
    }
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
