#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void timer_handler(int signum){
  printf("hello\n");
}

int main(){
  struct itimerval timval;
  if(signal(SIGALRM,timer_handler)==SIG_ERR){
    perror("signal");
    exit(1);
  }

  timval.it_interval.tv_sec = 0;
  timval.it_interval.tv_usec = 0;
  timval.it_value.tv_sec = 1;
  timval.it_value.tv_usec = 0;

  if(setitimer(ITIMER_REAL,&timval,NULL)==-1){
    perror("setitime");
    exit(1);
  }

  while(1){
    printf("z\n");
    sleep(1);
  }
}
