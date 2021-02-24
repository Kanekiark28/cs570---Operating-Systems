#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

void* myturn(void* arg ){
    int *iptr = (int *)arg;
     while(1){
         sleep(1);
         printf("My Turn\n");
         (*iptr)++;
     }
     return NULL;
}

void yourturn(){
    while(2){
        sleep(2);
        printf("Your Turn\n");
    }
}
int main(){
    pthread_t thrd;
    int v = 5;
    pthread_create(&thrd,NULL,myturn,&v);

    yourturn();

    pthread_join(thrd, NULL);
    printf("thread's done: v=%d",v);
}