/*
Codigo que estima el numero utilizando el metodo montecarlo
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<mpi.h>

#define R 1000
#define IT 50

int main(int argc, char ** argv)
{
	int nproc, id, i;
	int cont = 0;
	double x, y;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	for(i=0 ; i<IT ; i++)
	{
		srand (id + (int)time(NULL)*100);
		x = rand()%R + 1;
		usleep(0.03 * 10e6);
		srand (id + (int)time(NULL)*100);
		y = rand()%R + 1;
		x /= R;
		y /= R;
		if(x*x + y*y <= 1.0)
		{
			cont++;
		}	
	}
	for(i = 1; i<nproc; i++){
	
		MPI_Send(&cont, 1, MPI_INT, 0, i, MPI_COMM_WORLD);
	}
	if(id != 0)
	{
		double pi;
		int total;
		int aux, j;
		for(j = 1; j<nproc ; j++)
		{
			MPI_Recv(&aux, 1, MPI_INT, j, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total += aux;
		}
		total += cont;
		pi = ((double)total/(nproc*IT))*4;
		printf("El numero de puntos es: %d", total);
		printf("El valor de PI estimado es: %f", pi);
	}
	else
	{
		for(i = 1; i<nproc; i++)
		{
			printf("Procesador %d reporta: %d", i, cont);
		}
	}
	
	MPI_Finalize();
	return 0;
}
