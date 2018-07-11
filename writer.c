#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "writer.h"

/* global pointer to shared memory */
void *ptr;

int create_shm(void);

int main(int argc, char *argv[])
{
	int rc;
	struct timespec tspec;

	rc = create_shm();
	if (rc < 0) {
		printf("shared memory initialization failed!\n");
		return -1;
	}

	while (1) {
		clock_gettime(CLOCK_MONOTONIC, &tspec);
		memcpy(ptr, &tspec, sizeof(struct timespec));
		sleep(1);
	}

	return 0;
}

int create_shm(void)
{
	int shm_fd;
	const char *shm_name = "shm_test";
	const int size = sizeof(struct timespec);

	/* open the shared memory segment */
	shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	if (shm_fd < 0) {
		printf("shared memory failed\n");
		goto bail;
	}

	ftruncate(shm_fd, size);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		goto bail;
	}

	return 0;

bail:
	return -1;
}
