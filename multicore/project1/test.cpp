#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>

#define thread_num 3
using namespace std;
int num=0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t

void *print_out(void* a){
    //pid_t pid = getpid();
    unsigned int pid = (unsigned int)getpid();
    //pthread_t tid = pthread_self();
    unsigned int tid =  (unsigned int)pthread_self();
    int a1 = *((int*)a);
    printf("%dth thread\n",a1);
    for(int j=0; j<10; ++j){
        printf("%d - %u %x - [%2d ]\n",a1, pid,tid,j);
        if(j%(a1+1) == 0) sleep(1);
    }
    num++;
    printf("thread %d ends\n",num);
    //pthread_exit(0);
}


int main(){
    pthread_t thrd[thread_num];
    int status;
    pthread_attr_t p_attr;
    printf("%d %d\n",ESRCH,EINVAL);
    pthread_attr_init(&p_attr);
    //pthread_attr_setdetachstate(&p_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setdetachstate(&p_attr, PTHREAD_CREATE_JOINABLE);
    
    for(unsigned int i=0; i<thread_num; i++){
        //pthread_create(&thrd[i],&p_attr,print_out,(void*)&i);
        pthread_create(&thrd[i],NULL,print_out,(void*)&i);
        int ret = pthread_join(thrd[i], (void**)&status);
    }
    
    pthread_attr_destroy(&p_attr);

    return 0;
}