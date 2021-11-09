#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "proctimes.h"
#include <errno.h>

#define getproctimes(arg1, arg2) syscall(251, arg1, arg2)
#define ONE_THOUSAND 1000
#define ONE_MILLION 1000000
#define ONE_BILLION 1000000000

int main(int argc, char *argv[]) {
	struct proctimes time1, time2;
	struct timeval tv1, tv2;
	
	if(getproctimes(0, &time1) == EINVAL) {
		fprintf(stderr, "error calling getproctimes syscall\n");
		return -1;
	}

	gettimeofday(&tv1, NULL);

	printf("sleeping for 5 seconds...\n");
	sleep(5);
	
	if(getproctimes(0, &time2) == EINVAL) {
		fprintf(stderr, "error calling getproctimes syscall\n");
		return -1;
	}

	gettimeofday(&tv2, NULL);

	unsigned long timediff = time2.proc.real_time - time1.proc.real_time;
	unsigned long tv1_usecs = tv1.tv_sec * ONE_MILLION + tv1.tv_usec;
	unsigned long tv2_usecs = tv2.tv_sec * ONE_MILLION + tv2.tv_usec;
	unsigned long gtoddiff = tv2_usecs - tv1_usecs;

	printf("getproctimes diff (s) = %0.3f\n", timediff / (double)ONE_THOUSAND);
	printf("gettimeofday diff (s) = %0.3f\n", gtoddiff / (double)ONE_MILLION);

	return 0;
}
