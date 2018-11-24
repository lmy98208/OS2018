/*Date: 2018.11.20
Author:19160209
Intro:bank
 */
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#define personNum 10
#define random(x) (rand()%x)
int bankAcnt[personNum];
pthread_mutex_t mutex;
#define personNum 10
void err_quit(const char *api);
void thread_fun(void *arg);
void moneyTrading(int i,int j,int amount);
int main(){
		for(int i=0;i<personNum;i++)
				bankAcnt[i]=100;

//		if(pthread_mutex_init(&mutex,NULL)<0)
//				err_quit("pthread_mutex_init");
		for(int i=0;i<5;i++){
				pthread_t th;
				if(pthread_create(&th,NULL,(void *)thread_fun,NULL)!=0)
						err_quit("pthread_create");
		}
//		pthread_mutex_destory(&mutex);
		return 0;
}
void err_quit(const char *api)
{
		perror(api);
		exit(1);
}
void thread_fun(void *arg)
{
		int retval;
		pthread_t tid = pthread_self();
		pthread_detach(tid);
		srand((int)time(0));
		int i=random(personNum),j;
		do{

				j=random(personNum);
		}while(i==j);
		int amount=random(100);
		moneyTrading(i,j,amount);
		printf("tid->%ld exit\n", tid);
		pthread_exit(&retval);
}
void moneyTrading(int i,int j,int amount)
{
		pthread_mutex_lock(&mutex);
		if(bankAcnt[i]<amount)
				printf("fail!\t");
		else{
				bankAcnt[i]=bankAcnt[i]-amount;
				bankAcnt[j]+=amount;
				printf("success!\t");
		}
		printf("bankAcnt %d:%d,bankAcnt %d:%d",
						i,bankAcnt[i],j,bankAcnt[j]);
		pthread_mutex_unlock(&mutex);
}
