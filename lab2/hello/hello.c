#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/syscall.h>

int main()
{
	int desc = open("hello.txt", O_RDWR | O_CREAT, S_IRWXU);

	if(desc < 0)
	{
		perror("open error");
		return errno;
	}

	int bytesCount;

	bytesCount = write(desc, "Hello world.", 12);
	if(bytesCount < 0)
	{
		close(desc);
		perror("write error");
		return errno;
	}

	close(desc);

	return 0;
}