#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "proctimes.h"
#include <sys/syscall.h>

#define __NR_getproctimes 251

void function1(){
	unsigned long i=1000000;
	double z;
	while(i--){
		z=i*(1000000-i);
	}
}

void function2(){
	unsigned long i=1000000;
	double z;
	while(i--){
		z=i + 1;
	}
}

int main(){
	pid_t tid;
	struct proctimes time1, time2;
	tid = getpid();

	syscall(__NR_getproctimes, tid, &time1);
	function1();
	syscall(__NR_getproctimes, tid, &time2);
	printf("Multiplication/Division used time: %ld\n", time2.proc.real_time-time1.proc.real_time);
	

	
	syscall(__NR_getproctimes, tid, &time1);
	function2();
	syscall(__NR_getproctimes, tid, &time2);
	printf("Addition/Subtraction used time: %ld\n", time2.proc.real_time-time1.proc.real_time);


	return 0;
}
