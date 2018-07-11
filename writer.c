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

int create_shm(void *ctx);

int main(int argc, char *argv[])
{
	int rc;
	void *ctx;
	struct timespec tspec;

	rc = create_shm(ctx);
	if (rc < 0) {
		printf("shared memory initialization failed!\n");
		return -1;
	}

	printf("ctx: %p\n", ctx);
	while (1) {
		clock_gettime(CLOCK_MONOTONIC, &tspec);
		//memcpy(ctx, &tspec, sizeof(struct timespec));
		sleep(1);
	}

	return 0;
}

int create_shm(void *ctx)
{
	int shm_fd;
	void *ptr;
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

	printf("ctx: %p , ptr: %p\n", ctx, ptr);
	ctx = ptr;
	printf("ctx: %p , ptr: %p\n", ctx, ptr);

	return 0;

bail:
	return -1;
}
