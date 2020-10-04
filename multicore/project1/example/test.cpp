#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>

#define thread_num 3
using namespace std;
int num=0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t pt_barrier;
pthread_mutex_t num_lock = PTHREAD_MUTEX_INITIALIZER;
void *print_out(void* a){
    //pid_t pid = getpid();
    unsigned int pid = (unsigned int)getpid();
    //pthread_t tid = pthread_self();
    unsigned int tid =  (unsigned int)pthread_self();
    unsigned int a1 = *((unsigned int*)a);
    pthread_mutex_lock(&num_lock);
    num++;
    int num1 = num;
    pthread_mutex_unlock(&num_lock);
    printf("thread %d starts\n",num1);
    printf("%dth thread\n",a1);
    for(int j=0; j<10; ++j){
        //int j = 1;
        printf("%d - %u %x - [%2d ]\n",a1, pid,tid,j);
        if(j%(a1+1) == 0) sleep(0.5);
    }
    printf("thread %d ends\n",num1);
    //pthread_exit(0);
}


int main(){
    pthread_t thrd[thread_num];
    int status;
    pthread_attr_t p_attr;
    printf("%d %d\n",ESRCH,EINVAL);
    pthread_attr_init(&p_attr);
    //pthread_barrier_init(&pt_barrier,NULL,);
    //pthread_attr_setdetachstate(&p_attr, PTHREAD_CREATE_DETACHED);
    //pthread_attr_setdetachstate(&p_attr, PTHREAD_CREATE_JOINABLE);
    unsigned int  i=0;
    
    while(num<4){
        //pthread_create(&thrd[i],&p_attr,print_out,(void*)&i);
        pthread_create(&thrd[i],NULL,print_out,(void*)&i);
        int ret = pthread_join(thrd[i], (void**)&status);
    }
    
    pthread_attr_destroy(&p_attr);

    return 0;
}