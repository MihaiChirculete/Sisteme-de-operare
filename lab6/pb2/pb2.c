#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define max(a,b) = (a>b)? a:b

struct numere
{
	int count;
	int* v;
};

void * calcul(void *params)
{
	int *nr = (int*)params;

	return NULL;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Usage: prod_mat INPUT_FILE\n");
		return 0;
	}

	FILE* fd = fopen(argv[1], "rw");

	int **mat1, **mat2, **mat3;
	int n1, m1, n2, m2, n3, m3;

	fscanf(fd, "%d", &n1);
	fscanf(fd, "%d", &m1);
	fscanf(fd, "%d", &n2);
	fscanf(fd, "%d", &m2);

	int i, j;

	mat1 = (int**)malloc(n1 * sizeof(int*));
	for(i=0; i<n1; i++)
		mat1[i] = (int *)malloc(m1 * sizeof(int));

	mat2 = (int**)malloc(n2 * sizeof(int*));
	for(i=0; i<n2; i++)
		mat2[i] = (int *)malloc(m2 * sizeof(int));

	n3 = max

	mat3 = (int**)malloc(n3 * sizeof(int*));
	for(i=0; i<n3; i++)
		mat3[i] = (int *)malloc(m3 * sizeof(int));

	for(i=0; i<n1; i++)
		for(j=0; j<m1; j++)
		{
			fscanf(fd, "%d", &mat1[i][j]);
		}

	for(i=0; i<n2; i++)
		for(j=0; j<m2; j++)
		{
			fscanf(fd, "%d", &mat2[i][j]);
		}


	struct numere linie, coloana;
	for(i=0; i<n1; i++)
		for(j=0; j<m2; j++)
		{
			linie.count = n1;
			linie.v = mat1[i];

			coloana.count = n2;
			coloana.v = mat2[j];
		}

	return 0;
}