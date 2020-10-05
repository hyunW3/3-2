/*
 * main.cpp
 *
 * Serial version
 *
 * Compile with -O2
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include "skiplist.h"

#define C_QUEUE_NUM 30

using namespace std;

// aggregate variables
long sum = 0;
long odd = 0;
bool done = false;
long min = INT_MAX;
long max = INT_MIN;
FILE* fin;
int thd_num;

skiplist<int, int> list(0,1000000);
typedef struct { char act; long num; } input;
pthread_mutex_t q_lock;
pthread_cond_t no_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t no_full = PTHREAD_COND_INITIALIZER;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
input worker_queue[C_QUEUE_NUM];
long loc_insert, loc_take;


// function prototypes

void *worker(void* n){
	input job; 
	while(!done){

		pthread_mutex_lock(&q_lock);
		while(worker_queue[(loc_take)%C_QUEUE_NUM].num==-1 && !done) pthread_cond_wait(&no_empty,&q_lock);
		job.act=worker_queue[(loc_take)%C_QUEUE_NUM].act;
		job.num=worker_queue[(loc_take)%C_QUEUE_NUM].num;
		worker_queue[(loc_take++)%C_QUEUE_NUM].num=-1;
		pthread_cond_signal(&no_full);
		pthread_mutex_unlock(&q_lock);

		if(job.act=='i'){	
			pthread_rwlock_wrlock(&rwlock); 
			list.insert(job.num,job.num);
			sum += job.num;

			if (job.num % 2 == 1) {
			    odd++;
			}
			pthread_rwlock_unlock(&rwlock);	// write lock
		}


		else if(job.act=='q'){	
			pthread_rwlock_rdlock(&rwlock);
			if(list.find(job.num)!=job.num)
				cout << "ERROR: Not Found: " << job.num << endl;
			pthread_rwlock_unlock(&rwlock);
		}

		else if(job.act=='w'){					
			usleep(job.num*1000);
		}
	
		else if(job.act=='x' && job.num==0){
			done=true;
			for(int i=0;i<C_QUEUE_NUM;i++){
			worker_queue[i].act='x';
			worker_queue[i].num=-1;}
			pthread_cond_broadcast(&no_empty);
		}


	}
}

void master(int tnum){
	int num = tnum;
	int i;
	int tid, tmp;
	input work,end;
	pthread_t work_thread[num];
	pthread_rwlock_init(&rwlock,NULL);
	for(int i=0;i<num;i++){
	    tid = pthread_create(&work_thread[i],NULL,worker,(void*)&tmp);
	    if (tid < 0){
		perror("thread create error : ");
		exit(0);
	    }
	}
	for (;fscanf(fin, "%c %ld\n", &work.act, &work.num) == 2;) {

	    pthread_mutex_lock(&q_lock);
	    while(worker_queue[(loc_insert)%C_QUEUE_NUM].num!=-1) pthread_cond_wait(&no_full,&q_lock);
	    worker_queue[(loc_insert)%C_QUEUE_NUM].act=work.act;
	    worker_queue[(loc_insert++)%C_QUEUE_NUM].num=work.num;
	    pthread_cond_signal(&no_empty);
	    pthread_mutex_unlock(&q_lock);

	}

        pthread_mutex_lock(&q_lock);
	while(worker_queue[(loc_insert)%C_QUEUE_NUM].num!=-1) pthread_cond_wait(&no_full,&q_lock);
	worker_queue[(loc_insert)%C_QUEUE_NUM].act='x';
	worker_queue[(loc_insert++)%C_QUEUE_NUM].num=0;
        pthread_cond_broadcast(&no_empty);
        pthread_mutex_unlock(&q_lock);

	for (int i=0;i<num;i++){
	    pthread_join(work_thread[i],(void **)&tmp);
	}
	pthread_rwlock_destroy(&rwlock);


}


int main(int argc, char* argv[])
{
    struct timespec start, stop;
    int thr_id,tmp;
    int state;
    int pid = 0;

    // check and parse command line options
    if (argc != 3) {
        printf("Usage: sum <infile>\n");
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    thd_num = atoi(argv[2]);
    pthread_t main_thread, worker_thread[thd_num];

    clock_gettime( CLOCK_REALTIME, &start);
    // load numbers and add them to the queue
    fin = fopen(fn, "r");

    master(thd_num);
    
    fclose(fin);

    clock_gettime( CLOCK_REALTIME, &stop);
    // print results
    cout << list.printList() << endl;
    cout << sum << " " << odd << endl;
    cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec)+ ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << endl;

    // clean up and return
    return (EXIT_SUCCESS);

}

