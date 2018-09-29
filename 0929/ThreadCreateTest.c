#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#define NUM_THREADS 10
void *create(void *args)
{
		int thread_arg=*(int*)args;
		sleep(1);
		for(int i=0;i<10;i++)
		{
				printf("New thread %d created...\n",thread_arg);
		}
		return NULL;
}

int main(int argc,char *argv[])
{
		pthread_t tidp[NUM_THREADS];
		int error;
		int i[NUM_THREADS];
		for(int t=0;t<NUM_THREADS;t++)
		{
				i[t]=t;
				error=pthread_create(&tidp[t],NULL,create,&i[t]);
				if(error!=0)
				{	
					printf("pthread_create %d is not created...\n",t);
					return -1;
				}
		}
		sleep(5);
		for(int t=0;t<NUM_THREADS;t++)
		{
				pthread_join(tidp[t],NULL);
		}
		printf("pthread_create is created...\n");
		return 0;
}
