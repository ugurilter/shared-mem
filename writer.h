void *attach_writer(void)
{
	int shm_fd;
	int size;
	char *shm_name; 
	void *ptr;


	shm_name = "shm_test";
	size = sizeof(struct timespec);

	/* open the shared memory segment */
	shm_fd = shm_open(shm_name, O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		goto bail;
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0, size, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		goto bail;
	}

	return ptr;
bail:
	return NULL;
}
