#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *func(void *arg){
  for(int i=0;i<5;i++){
    printf("child\n");
    sleep(2);
  }
  return NULL;
}

int main(){
  pthread_t th;
  pthread_create(&th,NULL,func,NULL);
  for(int i=0;i<10;i++){
    printf("parent\n");
    sleep(1);
  }
  pthread_join(th,NULL);

  return 0;
}
