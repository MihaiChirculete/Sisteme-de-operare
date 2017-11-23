#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>

int main(int argc, char **argv)
{
	int n, *numbers, i, k;

	pid_t parent = getpid();

	// shared memory specific
	char *shm_name = "collatz_shm";
	int shm_fd;

	if(argc < 2)
	{
		printf("usage: collatz NUMBER1 NUMBER2 ...\n");
		return 0;
	}

	n = argc - 1;

	// allocate memory for the n numbers
	numbers = malloc(sizeof(int) * n);

	// parse the arguments of main into the numbers array
	for(i=0; i<n; i++)
	{
		numbers[i] = atoi(argv[i+1]);
	}

	// create the shared memory object
	shm_fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if(shm_fd < 0)
	{
		perror(NULL);
		return errno;
	}

	// allocate the shared memory
	size_t shm_size = 1024;
	if(ftruncate(shm_fd, shm_size) == -1)
	{
		perror(NULL);
		shm_unlink(shm_name);
		return errno;
	}

	k = 0;	// keeps track of the current number in the array

	// spawn n processes to process the n numbers in the array
	for(i=0; i<n; i++)
	{
		pid_t child = fork();
		if(child < 0)
			return errno;

		else if(child == 0)
		{
			// child instructions
			// printf("child with k=%d\n", k);

			// map the sharedmem region
			char *shm_ptr = mmap(0, getpagesize(), PROT_WRITE, MAP_SHARED, shm_fd, k * getpagesize());
			if(shm_ptr == MAP_FAILED)
			{
				perror(NULL);
				shm_unlink(shm_name);
				return errno;
			}

			//printf("%d: ", numbers[k]);
			//int offset = 0;
			while(numbers[k] > 1)
			{
				//printf("%d ", numbers[k]);
				//shm_ptr[offset] = numbers[k];
				int written = sprintf(shm_ptr, "%d", numbers[k]);
				shm_ptr += written;
				

				if(numbers[k] % 2 == 0)
					numbers[k] /= 2;
				else if(numbers[k] % 2 != 0)
					numbers[k] = 3*numbers[k] + 1;
			}
			//printf("%d.\n", 1);
			sprintf(shm_ptr, "%d", 1);

			munmap(shm_ptr, getpagesize());
			break;	// break the loop that keeps spawning processes
		}

		else
		{
			// parent instructions

		}

		k++;

	}

	if(getpid() == parent)
	{
		for(i=0; i<n; i++)
		{
			pid_t ended_proc = wait(NULL);
			if(ended_proc > 0)
				printf("Process %d ended.\n", ended_proc);
		}

		printf("Afisare\n");
		// print the results from the shared memory
		for(i=0; i<n; i++)
		{
			
			printf("%d: ", atoi(argv[i+1]));
			// map the sharedmem region
			
			char *shm_ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, shm_fd, i * getpagesize());
			if(shm_ptr == MAP_FAILED)
			{
				perror(NULL);
				shm_unlink(shm_name);
				return errno;
			}

			//k = 0;
			printf("%c", *shm_ptr);
			
		}
	}

	return 0;
}