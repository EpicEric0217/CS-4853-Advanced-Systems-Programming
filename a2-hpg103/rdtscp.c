#include <stdio.h>
#include <stdlib.h>

inline unsigned long long rdtscp() {
	unsigned int lo, hi;
	asm volatile (
		"rdtsc"
		: "=a"(lo), "=d"(hi) /* outputs */
		: "a"(0)             /* inputs  */
		: "%ebx", "%ecx");     /* clobbers*/
	unsigned long long retval = ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
	return retval;
}

int main(int argc, char ** argv) {
	unsigned long long timestamp;

	timestamp = rdtscp();

	printf("rdtscp=%llu\n", timestamp);

	return EXIT_SUCCESS;
}
