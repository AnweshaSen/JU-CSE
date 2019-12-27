// Create child processes: X and Y. 
// a. Each child process performs 10 iterations. The child process displays its name/id and the current iteration number, 
// and sleeps for some random amount of time. Adjust the sleeping duration of the processes to have different outputs 
// (i.e. another interleaving of processes’ traces). 

#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#define max_time 2
int main(){
	pid_t x,y,temp;
	srand(time(NULL));
	x = fork();
    y = fork();
	if(x<0) {
		printf("FORK ERRor\n");
		abort();		
    }
	else if(x==0){
		printf("child X\n");
		int i;
		for(i=1;i<=10;i++){
			printf("child X id=%d  iteration no=%d\n",getpid(),i);
			sleep(0.001+(rand()%max_time));
		}
		exit(0);
    }
	else if(y<0) {
		printf("FORK ERRor\n");
		abort();		
    }
	else if(y==0){
		printf("child Y\n");
		int i;
		for(i=1;i<=10;i++){
			printf("child Y id=%d  iteration no=%d\n",getpid(),i);
			sleep(0.001+(rand()%max_time));
			}
		exit(0);
	}
    else
    {
        temp=wait(&x);
		printf("Got exit status from child X\n");
        temp=wait(&y);
        printf("Got exit status from child y\n");
    }
return 0;
}
