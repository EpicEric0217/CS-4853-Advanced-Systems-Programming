#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

//macro to define support of my getpages system call
#define getpages(arg1) syscall(252, arg1)

#define PAGE_SIZE 4096
#define NUM_MAP_PAGES 10
#define NUM_GLOB_PAGES 10
#define MY_GLOB_SIZE (NUM_GLOB_PAGES * PAGE_SIZE)
#define MY_MAP_SIZE (NUM_MAP_PAGES * PAGE_SIZE)
#define MY_MAP_PROT (PROT_READ | PROT_WRITE | PROT_EXEC)
#define MY_MAP_FLAGS (MAP_ANONYMOUS | MAP_PRIVATE)

char my_glob[MY_GLOB_SIZE] = "this is a test...";

int main(int argc, char *argv[]) {
	int i;
	unsigned long result = getpages(&result);
	printf("number of stack pages before mapping: %lu\n\n", result);

	void * my_mmap;
	void * my_mmap_ending;
	if((my_mmap = mmap(NULL, MY_MAP_SIZE, MY_MAP_PROT, MY_MAP_FLAGS, -1, 0)) == MAP_FAILED) {
		perror("mmap failed");
		return EXIT_FAILURE;
	}
	my_mmap_ending = (void *) ((char *)my_mmap + MY_MAP_SIZE);
	printf("my mapping @ %p ~ %p\n", my_mmap, my_mmap_ending);

	result = getpages(my_mmap);
	printf("num pages before touching: %lu\n\n", result);

	for(i = 0; i <NUM_MAP_PAGES; i++) {
		char * mod_mmap = (char *)(my_mmap + i * PAGE_SIZE);
		*mod_mmap = 'a';
		result = getpages(my_mmap);
		printf("number of pages after touching %d pages: %lu\n", (i + 1), result);
	}

	result = getpages(my_glob);
	char * my_glob_end = my_glob + MY_GLOB_SIZE; //creating a global variable
	printf("my global array @ %p ~ %p\n", &my_glob, my_glob_end); //global array is between my_glob and my_glob_end...
	printf("number of pages by global: %lu\n", result);

	result = getpages(&result);
	printf("number of stack pages after mapping: %lu\n\n", result);
	//while(1); //busy wait for me to check /proc
	
	return EXIT_SUCCESS;
}
