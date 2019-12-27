// Create child processes: X and Y. 
// b. Modify the program so that X is not allowed to start iteration i before process Y has terminated its own iteration i-1. 
// Use semaphore to implement this synchronization. 

#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
pid_t x,y;
int main(){
   sem_t *sem1;
   sem1= sem_open("/semaphore1", O_CREAT,  0644, 0);	
	
	y = fork();
	if(y<0) {
		printf("Fork error\n");
		abort();		
}
	else if(y==0){
	printf("child Y\tprocess id=%d\n",getpid());
		int i;
		for(i=0;i<10;i++){
			 sem_wait(sem1);
			//display(i,"X");
			printf("child Y id=%d  iteration no=%d\n",getpid(),i);
			//sem_post(&s);
			sleep(1);
		}
		exit(0);
}
	x=fork();
	if(x<0) {
		printf("FORK ERRor\n");
		abort();		
}
	else if(x==0){
	printf("child X\tprocess id=%d\n\n\n",getpid());
		int i;
		for(i=0;i<10;i++){
			//sem_wait(&s);
			//sem_post(&s);
			//display(i,"Y");
			printf("child X id=%d  iteration no=%d\n",getpid(),i);
		sem_post(sem1);
			sleep(1);
			}
		exit(0);
		}
	//wait(NULL);
	//wait(NULL);
    sem_close(sem1);
    sem_unlink("/semaphore1");
	int status,i,n;
	pid_t temp;
	n=2;
	while(n>0) {
		temp=wait(&status);
		printf("child process = %ld	 exit status= %d\n",(long)temp,status);
		n--;
}
return 0;
}
