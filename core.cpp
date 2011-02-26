#include <iostream>
#include <fstream>
#include <mpi.h>
#include "readability.h"

using namespace std;

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv); const bool root = (myID(world) == rootID);
	//Read N from command line	
	int N = atoi(argv[1]);
	const int N2 = N * N;
	int m = 2;
	//Create mesh & init i, j = coords in mesh.
	MPI_Comm mesh, row; 
	int dims[]= {N, N}, periods[]= {0, 0}, reorder=0, coords[2] , freecoords[]={0,1}; 
	MPI_Cart_create(world, m, dims, periods, reorder, &mesh); int id = myID(mesh);	    	
	MPI_Cart_sub(mesh, freecoords, &row);
	MPI_Cart_coords(mesh, id, m, coords); int i = coords[0]; int j = coords[1];
	
	int Above = 0, Local = 0, Bcast = 0, temp = 0;
	
	//communication tags
	int vertical = 1;	
	
	int *line;
	if (i == 0 || i == N - 1)
		line = new int[N];
		
	ifstream infile;
	ofstream outfile;
	if (root)
		infile.open(argv[2], ios::in);
	if (id == N2 - 1)
		outfile.open(argv[3], ios::trunc);

	int totalSteps = 3*N;

for (int step = 0; step < totalSteps; step++)
{
	//Crazy dataflow
	if (i == 0)		//First Row
	{
		if (step < N)
		{
			if (root)			//Read a row from input file
				for(int f = 0; f < N; f++)
					infile >> line[f];	
			if (step == 0)
				scatter(line, &Local, 0, row);
			else
			{
				scatter(line, &Above, 0, row);		
				if (i == j)
					Bcast = Above;
				bcast(&Bcast, i, row);
				
			}
		}
		if (step == N)
				Above = Local;
		if (i < step && step < i + N + 1 )
			send(&Above, id + N, vertical, mesh);
	}
	else	//  0 < i 
	{
		if (2*i-1 < step)
		{
			if (step < 2*i + N)
				temp = recv(id - N, vertical, mesh);
			if (i == j)
				Bcast = temp;
			bcast(&Bcast, i, row);
				
			if (2*i == step)
				Local = temp;
			if (2*i+1 == step)
				Above = temp;
			if (2*i+1  < step && step <= 2*i+1 + N)
				if (i < N - 1)				
					send(&Above, id + N, vertical, mesh);			
				else // i == N, last row 
				{
					gather(&Above, line, N-1, row);
					if (id == N2 - 1)			
						{
							for(int f = 0; f < N; f++)
								outfile << line[f] << " ";	
							outfile << endl;	
						}						
				}
			if (2*i+1  < step && step < 2*i+1 + N - 1)
				Above = temp;
			if (step == 2*i + N)
				Above = Local;				
		}
	}
	//Actual computation :)
	Above = Above || (Bcast && Local);
}
	MPI_Finalize();
	return 0;
}