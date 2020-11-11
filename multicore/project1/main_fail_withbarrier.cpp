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
FILE* fin;
skiplist<int, int> list(0,1000000);
queue<pair<char,int>> task_q;
bool stop_all=false;

// function prototypes
void* MainThread(void* unused);
void* workerThread(void* unused);
// pthread initializer
pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;
int r_thread =0; // current running pthread
ui num_thread; // given by a command line parameter
pthread_mutex_t rw = PTHREAD_MUTEX_INITIALIZER; // for r_thrad
pthread_barrier_t bar;
void* workerThread(void* unused){
    while(!start);
    while(!done){
        char action;
        long num;        
        while(stop_all){
            pthread_barrier_wait(&bar);
        }
        pthread_mutex_lock(&q_lock);

        if(task_q.empty() == true) {
            pthread_mutex_unlock(&q_lock);
            continue;
        }
        action = task_q.front().first; 
        num = task_q.front().second;
        task_q.pop();
        if(action == 'p')   stop_all = true;

        pthread_mutex_unlock(&q_lock);

        if (action == 'i') {            // insert
            list.insert(num,num);
            // update aggregate variables
            sum += num;
            if (num % 2 == 1) {
                odd++;
            }
        }else if (action == 'q') {      // qeury
            if(list.find(num)!=num) printf("ERROR: Not Found: %lu\n",num);
        } else if (action == 'w') {     // wait
            usleep(num);
        } else if (action == 'p') {     // wait
            //while(r_thread >1);
	        cout << list.printList() << "\n";
            pthread_mutex_lock(&q_lock);
            stop_all = false;
            pthread_mutex_unlock(&q_lock);
        } else {
            if(task_q.empty() != true){
                printf("ERROR: Unrecognized action: '%c'\n", action);
                //sleep(1);
                exit(EXIT_FAILURE);
            } //else sleep(0); // context switch
        }
    }
    pthread_exit(0);
}

void* MainThread(void* unused){
    // pthread setting
    pthread_t workers[num_thread];
    int retval;
    
    for(ui i=0; i<num_thread; ++i){
        errno = pthread_create(&workers[i],NULL,workerThread,NULL);
        if(errno <0){
            perror("pthread_create() fail");
            return (void*)EXIT_FAILURE;
        }
    }
    
    //get the action from input
    char action;
    long num;
    while (fscanf(fin, "%c %ld\n", &action, &num) > 0) {
        start = true;
        pthread_mutex_lock(&q_lock);      
        task_q.push(make_pair(action,num));  
        pthread_mutex_unlock(&q_lock);
    }
    fclose(fin);
    // waiting for worker thread ends
    while(task_q.empty() != true);
    done =true;
    
    // when task is end (queue is empty)
    for(ui i=0; i<num_thread; ++i){
        errno = pthread_join(workers[i],NULL);
        if(errno != 0){
            perror("thread_join fail");
            return (void*)EXIT_FAILURE;
        }
    }
    
    return (void*)EXIT_SUCCESS;
}


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
    pthread_barrier_init(&bar,NULL,num_thread-1);
    //printf("file :%s number of thread : %d\n",fn,num_thread);
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
    pthread_mutex_destroy(&rw);
    //check whether destroy
    return (EXIT_SUCCESS);
}