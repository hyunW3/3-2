/*
 * main.cpp
 *
 * Serial version
 *
 * Compile with -O2
 */
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <string>
#include "skiplist.h"

using namespace std;
typedef unsigned int ui;

// aggregate variables
long sum = 0;
long odd = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;
bool start = false;
volatile int running_thread=0;
FILE* fin;
skiplist<int, int> list(0,1000000);
queue<pair<char,int>> task_q;

// function prototypes
void* MainThread(void* unused);
void* workerThread(void* unused);
// pthread initializer
pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
volatile int r_thread =0; // current running pthread
ui num_thread; // given by a command line parameter
pthread_cond_t wakeup = PTHREAD_COND_INITIALIZER;
pthread_cond_t stop_all_cond = PTHREAD_COND_INITIALIZER;
bool stop_all=false;
volatile int iter=-1;

int main(int argc, char* argv[])
{
    struct timespec start, stop;
    pthread_t master;

    // check and parse command line options
    if (argc != 3) {
        printf("Usage: %s <infile> <Worker Thread #>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    num_thread = atoi(argv[2]);
    printf("file :%s number of thread : %d\n",fn,num_thread);
    clock_gettime( CLOCK_REALTIME, &start);

    // load input file
    fin = fopen(fn, "r");
    pthread_create(&master,NULL,MainThread,NULL);
    
    errno = pthread_join(master,NULL);
    if(errno != 0){
        perror("thread_join fail");
        return EXIT_FAILURE;
    }
    clock_gettime( CLOCK_REALTIME, &stop);

    // print results
    cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << endl;
    // clean up and return
    pthread_mutex_destroy(&q_lock);
    //pthread_cond_destroy(&stop_all);
    pthread_cond_destroy(&wakeup);
    pthread_rwlock_destroy(&rw);
    return (EXIT_SUCCESS);

}
void* workerThread(void* unused){
    while(!start) {
            //printf("%lu fall into cond_wait\n",pthread_self());
            //pthread_cond_wait(&wakeup,&q_lock);
        }
        char action;
    while(!done){
        long num;
        pthread_mutex_lock(&q_lock);
        //printf("%lu : empty? : %d, start? : %d\n",pthread_self(),task_q.empty(),start);
 
        if(task_q.empty() == true) {
            pthread_mutex_unlock(&q_lock);
            continue;
        }

        r_thread++;
        if(stop_all == true){
            while(iter == 0);
            iter--;
        }
        action = task_q.front().first; 
        num = task_q.front().second;
        task_q.pop();
        
        //pthread_mutex_unlock(&q_lock);
        //cout << "self pid:" << pthread_self() << "\n";
        /*
        if(action != 'p') printf("%c %ld\n",action,num);
        else printf("%c (print)\n",action);
        
        cout << "size :" << task_q.size() << "\n";
*/
        //pthread_mutex_unlock(&q_lock);
        //pthread_mutex_lock(&q_lock);
        if (action == 'i') {            // insert
            pthread_rwlock_wrlock(&rw);
            list.insert(num,num);
            pthread_rwlock_unlock(&rw);
            // update aggregate variables
            sum += num;
            if (num % 2 == 1) {
                odd++;
            }
        }else if (action == 'q') {      // qeury
            pthread_rwlock_rdlock(&rw);
            if(list.find(num)!=num) printf("ERROR: Not Found: %lu\n",num);
            pthread_rwlock_unlock(&rw);
            //else cout << "SUCCESS: Found: " << num << "\n";
        } else if (action == 'w') {     // wait
            //printf("sleep....\n");
            //usleep(num);
            struct timeval u_time;
            u_time.tv_sec = num/1000;
            u_time.tv_usec = num;
            select(0,0,0,0,&u_time);
        } else if (action == 'p') {     // wait
            pthread_rwlock_wrlock(&rw);
            
	        cout << list.printList() << "\n";
            iter = -1;
            stop_all = false;
            pthread_cond_broadcast(&stop_all_cond);
            pthread_rwlock_unlock(&rw);

        } else {
            if(task_q.empty() != true){
                printf("ERROR: Unrecognized action: '%c'\n", action);
                //sleep(1);
                //continue;
                exit(EXIT_FAILURE);
            } //else sleep(0); // context switch
        }
        //pthread_mutex_lock(&q_lock);
        //r_thread--;
        pthread_mutex_unlock(&q_lock);
    }
    //printf("Thread ends %lu \n",pthread_self());
    pthread_exit(0);
    // one thread is not exiting
}

void* MainThread(void* unused){
    // pthread setting
    pthread_t workers[num_thread];
    int retval;
    
    for(ui i=0; i<num_thread; ++i){
        errno = pthread_create(&workers[i],NULL,workerThread,NULL);
        //printf("pthread_id[%d] : %lu\n",i,workers[i]);
        if(errno <0){
            perror("pthread_create() fail");
            return (void*)EXIT_FAILURE;
        }
    }
    
    //get the action from input
    char action;
    long num;
    while (fscanf(fin, "%c %ld\n", &action, &num) > 0) {
        while(stop_all == true && action =='p');
        pthread_mutex_lock(&q_lock);
        //if(action == 'i') printf("%c %ld inserted in queue\n",action,num);
        start = true;
        task_q.push(make_pair(action,num));
        if(action == 'p'){
            //pthread_cond_wait(&stop_all,&q_lock);
            //printf("stop all called ");
            iter = task_q.size();
            //printf("iter : %u\n",iter);
            stop_all = true;
        }
        if(task_q.size()==1) {
            pthread_cond_broadcast(&wakeup);
        }
        pthread_mutex_unlock(&q_lock);
    }
    fclose(fin);
    // waiting for worker thread ends
    //printf("1-hi\n");
    while(task_q.empty() != true){  // when true, exit while loop
        //cout << "queue size : " << task_q.size() <<"\n";
        //sleep(1);
    }
    done =true;
    printf("2-hi\n");
    pthread_cond_broadcast(&wakeup);
    // when task is end (queue is empty)
    for(ui i=0; i<num_thread; ++i){
        pthread_mutex_lock(&q_lock);
        //cout << "thread id:" << workers[i] << "\n";
        //printf("3-hi %d \n",i);
        pthread_mutex_unlock(&q_lock);
        errno = pthread_join(workers[i],NULL);
        if(errno != 0){
            perror("thread_join fail");
            return (void*)EXIT_FAILURE;
        }
    }
    //printf("4-hi\n");
    //pthread_mutex_destroy(&q_lock);
    return (void*)EXIT_SUCCESS;
}