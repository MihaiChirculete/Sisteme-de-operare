#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/syscall.h>

int openFile(char* path, int createFile)
{
	int descriptor;
	if(createFile)
		descriptor = open(path, O_RDWR | O_CREAT, S_IRWXU);
	else
		descriptor = open(path, O_RDONLY);

	return descriptor;
}

int copyFile(int src_descriptor, int dest_descriptor)
{
	char* buf = (char*)malloc(32 * sizeof(char));

	int bytesRead, bytesWrite;
	while(bytesRead = read(src_descriptor, (void*)buf, 32))
	{
		if(bytesRead < 0)
		{
			close(src_descriptor);
			close(dest_descriptor);
			perror("read error");
			return errno;
		}

		bytesWrite = write(dest_descriptor, (void*)buf, bytesRead);
		buf = buf+bytesWrite;

		if(bytesWrite < 0)
		{
			close(src_descriptor);
			close(dest_descriptor);
			perror("write error");
			return errno;
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	int source, dest;

	if(argc < 3)
	{
		printf("Sunt necesare 3 argumente!\n");
		return -1;
	}

	source = openFile(argv[1], 0);
	if(source < 0)
	{
		perror("cant open source file\n");
		return errno;
	}

	dest = openFile(argv[2], 1);
	if(dest < 0)
	{
		perror("cant open destination file\n");
		return errno;
	}

	if(copyFile(source, dest) != 0)
		return errno;

	return 0;
}