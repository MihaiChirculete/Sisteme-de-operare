#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

struct param{
	int** matrice1;
	int** matrice2;
	int n1;
	int linie;
	int coloana;
};

void* calcElement(void* parametreAux)
{
	param* parametre = (param*)parametreAux;
	int* produs = (int*)malloc (sizeof(int));

	*produs = 0;

	for(int i = 0; i < parametre -> n1; i++)
		*produs += (parametre -> matrice1[parametre -> linie][i])*(parametre -> matrice2[i][parametre -> coloana]);


	return (void*)produs;
}

int main(int argc, char** argv)
{
	pthread_t thread;
	void* result;

	FILE *input = fopen("input.in", "r+");

	if(input == NULL)
	{
		perror("Eroare la incarcarea fisierului \"input.in\"!\n");
		return errno;
	}

	int n1, m1, n2, m2;

	fscanf(input, "%d", &n1);
	fscanf(input, "%d", &m1);

	int** matrice1 = (int**)malloc (sizeof(int*)*n1);

	for(int i = 0; i < n1; i++)
	{
		matrice1[i] = (int*)malloc (sizeof(int)*m1);
		for(int j = 0; j < m1; j++)
		{
			fscanf(input, "%d", &matrice1[i][j]);
		}
	}

	fscanf(input, "%d", &n2);
	fscanf(input, "%d", &m2);

	int** matrice2 = (int**)malloc (sizeof(int*)*n2);

	for(int i = 0; i < n2; i++)
	{
		matrice2[i] = (int*)malloc (sizeof(int)*m2);
		for(int j = 0; j < m2; j++)
		{
			fscanf(input, "%d", &matrice2[i][j]);
		}
	}

	if(n1 != m2)
	{
		perror("Matrici incompatibile cu inmultirea!\n");
		return errno;
	}

	int** matrice3 = (int**)malloc (sizeof(int*)*n1);

	param* parametre = (param*)malloc (sizeof(param));

	parametre -> matrice1 = matrice1;
	parametre -> matrice2 = matrice2;
	parametre -> n1 = n1;	

	for(int i = 0; i < n1; i++)
	{
		matrice3[i] = (int*)malloc (sizeof(int)*n1);
		for(int j = 0; j < n1; j++)
		{
			parametre -> linie = i;
			parametre -> coloana = j;
			if(pthread_create(&thread, NULL, calcElement, parametre))
			{
				perror(NULL);
				return errno;
			}

		}
		printf("\n");
	}

	for(int i = 0; i < n1; i++)
	{
			if(pthread_join(thread, &result))
			{
				perror(NULL);
				return errno;
			}
			printf("%d ", *(int*)result);
	}

	return 0;
}
