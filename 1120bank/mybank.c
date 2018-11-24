#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define acntNum 10
#define acntBalance 1000
// Note: intptr_t

typedef struct Account {
    char* name;
    long  balance;
    pthread_mutex_t mutex;
} Account;

Account* accounts[acntNum];

Account* make_account(char* name, long balance)
{
    Account* acc = malloc(sizeof(Account));
    acc->name = strdup(name);
    acc->balance = balance;
    pthread_mutex_init(&acc->mutex,0);
    return acc;
}

void free_account(Account* acc)
{
    free(acc->name);
    free(acc);
}

void lock_both(pthread_mutex_t* a, pthread_mutex_t* b)
{
    // lock in ordered sequence
    if(a < b) {
        pthread_mutex_lock(a);
        pthread_mutex_lock(b);
    }
    else {
        pthread_mutex_lock(b);
        pthread_mutex_lock(a);
    }
}

int transfer(Account* a, Account* b, long amount)
{
    // create helper
    lock_both(&(a->mutex),&(b->mutex));

    if (a->balance < amount) {
        return -1;
    }

    a->balance -= amount;
    b->balance += amount;
    printf("%s=>%s,%ld   ",a->name,b->name,amount);
    printf("%s:%ld,%s:%ld\n",a->name,a->balance,b->name,b->balance);

    pthread_mutex_unlock(&(a->mutex));
    pthread_mutex_unlock(&(b->mutex));

    return 0;
}

void* test_thread(void* seed_ptr)
{
    unsigned int seed = *((unsigned int*) seed_ptr);
    free(seed_ptr);

    for (int k = 0; k < 3; ++k) {
        int i = rand_r(&seed) % acntNum;
        int j = rand_r(&seed) % acntNum;
        int a = 1 + rand_r(&seed) % 100;

        if (i == j) {
            continue;
        }

        transfer(accounts[i], accounts[j], a);
    }

    return 0;
}

int main(int _ac, char* _av[]) 
{
    pthread_t threads[10];

    accounts[0] = make_account("acnt1", acntBalance);
    accounts[1] = make_account("acnt2", acntBalance);
    accounts[2] = make_account("acnt3", acntBalance);
    accounts[3] = make_account("acnt4", acntBalance);
    accounts[4] = make_account("acnt5", acntBalance);
    accounts[5] = make_account("acnt6", acntBalance);
    accounts[6] = make_account("acnt7", acntBalance);
    accounts[7] = make_account("acnt8", acntBalance);
    accounts[8] = make_account("acnt9", acntBalance);
    accounts[9] = make_account("acnt10", acntBalance);

    for (int i = 0; i < 10; ++i) {
        unsigned int* seed = malloc(sizeof(unsigned int));
        *seed = i + getpid();
        pthread_create(&(threads[i]), 0, test_thread, seed);
    }

    for (int i = 0; i < 10; ++i) {
        pthread_join(threads[i], 0);
    }

    long sum = 0;
    for (int i = 0; i < acntNum; ++i) {
        sum += accounts[i]->balance;
        printf("%s:\t%ld\n", accounts[i]->name, accounts[i]->balance);
        free_account(accounts[i]);
    }

    printf("\nSum:\t%ld\n", sum);

    return 0;
}
