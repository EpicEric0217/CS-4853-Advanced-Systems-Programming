#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include "proctimes.h"

#define SYSCALL_NUM 251
#define NUM_INTS 209999872
#define PAGESIZE 4096
#define CURRENT_PROCESS 0
#define getproctimes(arg1, arg2) syscall(SYSCALL_NUM, arg1, arg2)

int main(int argc, char *argv[]) {
	int i; 
	pid_t parent;
	struct proctimes time1, time2;

	//First I allocate the 1GB worth of the ints
	char *large = (char *)mmap(NULL, sizeof(int) * NUM_INTS, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
	if(!large) {
		fprintf(stderr, "ERROR: mmap failed: '%s'\n", strerror(errno));
		return EXIT_FAILURE;
        }
	for(i = 0; i < sizeof(int) * NUM_INTS; i+= PAGESIZE) {
		large[i] = 1;
	}

	if(getproctimes(CURRENT_PROCESS, &time1) == EINVAL) {
		fprintf(stderr,"error calling getproctimes syscall\n");
		return -1;
	}

	//For the process' child we simply exit right away...
	if(!(parent = fork())) {
			return 0;
	}
	
	if(getproctimes(CURRENT_PROCESS, &time2) == EINVAL) {
		fprintf(stderr, "error calling getproctimes syscall\n");
		return -1;
	}

	unsigned long real_time = time2.proc.real_time - time1.proc.real_time;

	printf("real time = %ld\n", real_time);
	printf("user time = %lu\n", time2.proc.user_time);
	printf("sys time = %lu\n", time2.proc.sys_time);

	wait(NULL);

	return 0;
}
	

			
