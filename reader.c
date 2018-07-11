#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "writer.h"

int main()
{
	struct timespec tspec;
    
	/* init shm from reader's context */
    void *read_addr = attach_writer();

	while (1) {
		memcpy(&tspec, read_addr, sizeof(struct timespec));
		printf("tv_sec: %ld , tv_nsec: %ld\n", tspec.tv_sec, tspec.tv_nsec);
		sleep(1);
	}

	return 0;
}
