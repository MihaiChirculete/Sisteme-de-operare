#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void * str_rev(void *params)
{
	char *str = (char*)params;

	int count = strlen(str);

	char *revstr = malloc(count+1);

	for(int i=0; i<count; i++)
	{
		revstr[i] = str[count - 1 - i];
	}

	revstr[count] = '\0';

	return (void*)revstr;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Usage: strrev WORD\n");
		return 0;
	}

	pthread_t thread;
	void* result;

	if(pthread_create(&thread, NULL, str_rev, argv[1]))
	{
		perror(NULL);
		return errno;
	}

	if(pthread_join(thread, &result))
	{
		perror(NULL);
		return errno;
	}

	printf("%s\n", (char*)result);

	return 0;
}