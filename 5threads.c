#include<stdio.h>
#include<pthread.h>
#include<time.h>

#define OUTER 100
#define INMER 10
#define COUNT OUTER*INMER
#define LOOP 10000
struct timespec ts[4][COUNT];

//mutexを3つ作成
pthread_mutex_t mutex[2];

void busy(){
  int index=0;
  for(int i=0;i<LOOP;i++){
    index++;
  }
  return;
}
void lock_two_resources(int *r1,int *r2){

  while(1){
    for(int i=0;i<2;i++){
      if(pthread_mutex_trylock(&mutex[i]==0){
	  for(int j=0;j<2;j++){
	    if(j==1){
	      continue;
	    }
	    if(pthread_mutex_trylock(&mutex[j])==0){
	      *r1=i;
	      *r2=j;
	      return;
	    }
	  }
	  //1つめok２つ目NGのとき
	  pthread_mutex_unlock(&mutex[i]);
	}
    }
    busy();
  }
}

void *func(void *arg){
  int thn=(int)arg;
  int ts_index=0;
  intr1,r2;
  
  for(int i=0;i<OUTER;i++){
    lock_two_resources(&r1,&r2);
    
    for(int j=0;j<INMER;j++){
      busy();
      clock_gettime(CLOCK_REALTIME,&ts[thn][ts_index]);
      ts_index++;
    }
    pthread_mutex_unlock(&mutex[r2]);
    pthread_mutex_unlock(&mutex[r1]);
    busy();
  }
  return NULL;
}

long nsec(struct timespec x){
  return x.tv_sec*1000000000+x.tv_nsec;
}

int main(){
  pthread_t th0;
  pthread_t th1;
  pthread_t th2;
  pthread_t th3;

  pthread_mutex_init(&mutex,NULL);
  struct timespec x;
  clock_gettime(CLOCK_REALTIME,&x);
  long startt=nsec(x);

  pthread_create(&th0,NULL,func,(void*)0);
  pthread_create(&th1,NULL,func,(void*)1);
  pthread_create(&th2,NULL,func,(void*)2);
  pthread_create(&th3,NULL,func,(void*)3);

  pthread_join(th0,NULL);
  pthread_join(th1,NULL);
  pthread_join(th2,NULL);
  pthread_join(th3,NULL);

  long t;
  for(int thn=0;thn<4;thn++){
    for(int i=0;i<COUNT;i++){
      t=nsec(ts[thn][i]);
      printf("%ld\t%d\n",t-startt,thn);
    }
  }

  return 0;
}

  
  
