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
FILE* fin;
skiplist<int, int> list(0,1000000);
queue<pair<char,int>> task_q;

// function prototypes
void* MainThread(void* unused);
void* workerThread(void* unused);
// pthread initializer
pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;
int r_thread =0; // current running pthread
ui num_thread; // given by a command line parameter
pthread_cond_t start;
pthread_rwlock_t rw_lock;

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
    return (EXIT_SUCCESS);

}
void* workerThread(void* unused){
    while(!done){
        char action;
        long num;
        pthread_mutex_lock(&q_lock);
        while(task_q.empty()== true && !done) {
            pthread_cond_wait(&start,&q_lock);
        }
        action = task_q.front().first; 
        num = task_q.front().second;
        task_q.pop();
        pthread_mutex_unlock(&q_lock);
        printf("%c %ld\n",action,num);
        if (action == 'i') {            // insert
            list.insert(num,num);
            // update aggregate variables
            sum += num;
            if (num % 2 == 1) {
                odd++;
            }
        }else if (action == 'q') {      // qeury
            if(list.find(num)!=num) cout << "ERROR: Not Found: " << num << "\n";
            else cout << "SUCCESS: Found: " << num << "\n";
        } else if (action == 'w') {     // wait
            printf("sleep....\n");
            usleep(num);
        } else if (action == 'p') {     // wait
	        cout << list.printList() << "\n";
        } else {
            if(task_q.empty() != true){
                printf("ERROR: Unrecognized action: '%c'\n", action);
                //sleep(1);
                //exit(EXIT_FAILURE);
            }else sleep(0); // context switch
        }
    }
    printf("Thread ends\n");
    pthread_exit(0);
    // one thread is not exiting
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
        printf("%c %ld inserted in queue\n",action,num);
        pthread_mutex_lock(&q_lock);
        task_q.push(make_pair(action,num));
        if(task_q.size()==1) pthread_cond_broadcast(&start);
        pthread_mutex_unlock(&q_lock);
    }
    fclose(fin);
    // waiting for worker thread ends
    printf("1-hi\n");
    while(task_q.empty() != true){  // when true, exit while loop
        cout << "queue size : " << task_q.size() <<"\n";
        sleep(10);
    }
    printf("2-hi\n");
    done =true;

    // when task is end (queue is empty)
    for(ui i=0; i<num_thread; ++i){
        printf("3-hi %d\n",i);
        errno = pthread_join(workers[i],NULL);
        if(errno != 0){
            perror("thread_join fail");
            return (void*)EXIT_FAILURE;
        }
    }
    printf("4-hi\n");
    pthread_mutex_destroy(&q_lock);
    return (void*)EXIT_SUCCESS;
}