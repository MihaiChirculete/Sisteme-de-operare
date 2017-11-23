#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace std;

#define MAX_RESOURCES 50
int available_resources = MAX_RESOURCES;
pthread_mutex_t mtx;

int decrease_count(int count)
{
	if(count > MAX_RESOURCES)
			return -1;

	while(available_resources < count)
	{}

	pthread_mutex_lock(&mtx);
	available_resources -= count;
	cout << "Got " << count << " resources " << available_resources << " remaining.\n";
	pthread_mutex_unlock(&mtx);

	return 0;
	
}

int increase_count(int count)
{
	pthread_mutex_lock(&mtx);
	available_resources += count;
	cout << "Released " << count << " resources " << available_resources << " remaining\n";
	pthread_mutex_unlock(&mtx);
	return 0;
}

void * reserveResources(void *count)
{
	int reserved_res = *(int*)count;

	if(decrease_count(reserved_res) == -1)
		return NULL;
	increase_count(reserved_res);
	return NULL;
}


int main()
{
	if(pthread_mutex_init(&mtx, NULL))
	{
		perror(NULL);
		return errno;
	}

	pthread_t *thread = (pthread_t*)malloc(sizeof(pthread_t) * 50);
	void* result;

	int res;

	for(int i=0; i<49; i++)
	{
		res = i+1;
		if(pthread_create(&thread[i], NULL, reserveResources, &res))
		{
			perror(NULL);
			return errno;
		}
	}

	for(int i=0; i<49; i++)
	{
		if(pthread_join(thread[i], &result))
		{
			perror(NULL);
			return errno;
		}
	}

	pthread_mutex_destroy(&mtx);
	return 0;
}