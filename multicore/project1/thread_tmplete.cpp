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

// function prototypes
void* MainThread(void* unused);
void* workerThread(void* unused);
// pthread initializer
pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;
int r_thread =0; // current running pthread
ui num_thread; // given by a command line parameter
pthread_cond_t wakeup;
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
    pthread_mutex_destroy(&q_lock);
    return (EXIT_SUCCESS);

}
void* workerThread(void* unused){
    while(!done){
        char action;
        long num;
        pthread_mutex_lock(&q_lock);
        //printf("%lu : empty? : %d, start? : %d\n",pthread_self(),task_q.empty(),start);
        while(!start) {
            pthread_cond_wait(&wakeup,&q_lock);
        }
        //printf("%lu : empty? : %d, start? : %d\n",pthread_self(),task_q.empty(),start);
        
        if(task_q.empty() == true) {
        pthread_mutex_unlock(&q_lock);
            continue;
        }
        action = task_q.front().first; 
        num = task_q.front().second;
        task_q.pop();
        /*
        cout << "self pid:" << pthread_self() << "\n";
        if(action != 'p') printf("%c %ld\n",action,num);
        else printf("%c (print)\n",action);
        */
        pthread_mutex_unlock(&q_lock);
        if (action == 'i') {            // insert
            list.insert(num,num);
            // update aggregate variables
            sum += num;
            if (num % 2 == 1) {
                odd++;
            }
        }else if (action == 'q') {      // qeury
            if(list.find(num)!=num) cout << "ERROR: Not Found: " << num << "\n";
            //else cout << "SUCCESS: Found: " << num << "\n";
        } else if (action == 'w') {     // wait
            //printf("sleep....\n");
            //usleep(num);
            struct timeval u_time;
            u_time.tv_sec = num/1000;
            u_time.tv_usec = num;
            select(0,0,0,0,&u_time);
        } else if (action == 'p') {     // wait
	        cout << list.printList() << "\n";
        } else {
            if(task_q.empty() != true){
                printf("ERROR: Unrecognized action: '%c'\n", action);
                //sleep(1);
                exit(EXIT_FAILURE);
            } //else sleep(0); // context switch
        }
    }
    printf("Thread ends %lu \n",pthread_self());
    pthread_exit(0);
    // one thread is not exiting
}

void* MainThread(void* unused){
    // pthread setting
    pthread_t workers[num_thread];
    int retval;
    
    for(ui i=0; i<num_thread; ++i){
        errno = pthread_create(&workers[i],NULL,workerThread,NULL);
        printf("pthread_id[%d] : %lu\n",i,workers[i]);
        if(errno <0){
            perror("pthread_create() fail");
            return (void*)EXIT_FAILURE;
        }
    }
    
    //get the action from input
    char action;
    long num;
    while (fscanf(fin, "%c %ld\n", &action, &num) > 0) {
        pthread_mutex_lock(&q_lock);
        //printf("%c %ld inserted in queue\n",action,num);
        start = true;
        task_q.push(make_pair(action,num));
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
        sleep(1);
    }
    done =true;
    printf("2-hi\n");
    pthread_cond_broadcast(&wakeup);
    // when task is end (queue is empty)
    for(ui i=0; i<num_thread; ++i){
        pthread_mutex_trylock(&q_lock);
        //cout << "thread id:" << workers[i] << "\n";
        printf("3-hi %d \n",i);
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