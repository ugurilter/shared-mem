#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
	const char *shm_name = "shm_test";
	const int SIZE = sizeof(struct timespec);
	struct timespec tspec;

	int i, shm_fd;
	void *read_addr;


	/* open the shared memory segment */
	shm_fd = shm_open(shm_name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	read_addr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (read_addr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	while (1) {
		memcpy(&tspec, read_addr, sizeof(struct timespec));
		printf("tv_sec: %ld , tv_nsec: %lu\n", tspec.tv_sec, tspec.tv_nsec);
		sleep(1);
	}

	/* remove the shared memory segment */
	if (shm_unlink(shm_name) == -1) {
		printf("Error removing %s\n", shm_name);
		exit(-1);
	}

	return 0;
}

