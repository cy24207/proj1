#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<errno.h>

#define ACCOUNT_NUM 100
#define THREAD_NUM 10
#define MAX_TRANS 20000
int trans_num=0;

int from[MAX_TRANS];
int to[MAX_TRANS];
int amount[MAX_TRANS];
int account[ACCOUNT_NUM];
int done[MAX_TRANS];

pthread_mutex_t mutex;
pthread_cond_t cvar;

long nsec(struct timespec t){
  return t.tv_sec * 1000000000L+t.tv_nsec;
}

int sumAmount(void){
  int sum=0;
  for(int i=0;i<ACCOUNT_NUM;i++){
    sum+=account[i];
  }
  return sum;
}

void readTransaction(void){
  FILE *fp;
  char type;
  int f,t,a;
  int val;

  fp=fopen("./pj/txn/trans.csv","r");
  if(fp==NULL){
    perror("fopen");
    exit(1);
  }

  while(1){
    val=fscanf(fp,"%c,%d,%d,%d",&type,&f,&t,&a);
    if(val==EOF){
      break;
    }

    if(trans_num>MAX_TRANS){
      printf("too many transaction");
      exit(1);
    }

    from[trans_num]=f;
    to[trans_num]=t;
    amount[trans_num]=a;

    trans_num++;

  }
  fclose(fp);
}

void *threadFunc(void *arg){
  int thn = (int)arg;

  int start=trans_num*thn/THREAD_NUM;
  int end =trans_num*(thn+1)/THREAD_NUM;

  for(int i=start;i<end;i++){
    
    //no meaning loop
    int index=0;
    for(int j=0;j<amount[i]*100;j++){
      index++;
    }

    pthread_mutex_lock(&mutex);

    while(account[from[i]]<amount[i]){
	pthread_cond_wait(&cvar,&mutex);
    }
      

    account[from[i]]-=amount[i];
    account[to[i]]+=amount[i];

    pthread_cond_broadcast(&cvar);
    pthread_mutex_unlock(&mutex);

  }
}

int main(){
  pthread_t th[THREAD_NUM];
  struct timespec start_time,end_time;
  long elapsed;
  int before,after;

  readTransaction();

  printf("transaction num=%d\n",trans_num);

  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cvar,NULL);

  for(int i=0;i<ACCOUNT_NUM;i++){
    account[i]=10000;
  }
  before=sumAmount();
  printf("before=%d\n",before);

  clock_gettime(CLOCK_MONOTONIC,&start_time);

  //create thread
  for(int i=0;i<THREAD_NUM;i++){
    pthread_create(&th[i],NULL,threadFunc, (void *)(int)i);
  }

  //join thread
  for(int i=0;i<THREAD_NUM;i++){
    pthread_join(th[i],NULL);
  }

  clock_gettime(CLOCK_MONOTONIC,&end_time);

  after=sumAmount();
  printf("after=%d\n",after);

  elapsed=nsec(end_time)-nsec(start_time);
  printf("time=%d",elapsed);
  
  
}
