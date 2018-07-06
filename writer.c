#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MICROSEC_TO_MSEC 1000
#define SLEEP_MS 100 * MICROSEC_TO_MSEC

int main(int argc, char *argv[])
{
	const int SIZE = sizeof(struct timespec);
	const char *shm_name = "shm_test";
	struct timespec tspec;

	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd, SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	while (1) {
		clock_gettime(CLOCK_REALTIME, &tspec);
		memcpy(ptr, (void *)&tspec, sizeof(struct timespec));
		//usleep(SLEEP_MS);
	}

	return 0;
}

