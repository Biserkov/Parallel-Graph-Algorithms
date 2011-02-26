#include <mpi.h>
#pragma once

MPI_Comm world = MPI_COMM_WORLD;
int rootID = 0;

MPI_Datatype dtype = MPI_INT;

inline int bcast(void* number, int root, MPI_Comm comm = world)
{
	return MPI_Bcast(number, 1, dtype, root, comm);
}

inline int myID(MPI_Comm comm = world)
{
	int t = -1;
	MPI_Comm_rank(comm, &t);
	return t;
}

inline int scatter(void* sendbuf, void *recvbuf, int root, MPI_Comm comm = world)
{
	return MPI_Scatter(sendbuf, 1, dtype, recvbuf, 1, MPI_INT, root, comm);
}

inline int gather(void* sendbuf, void *recvbuf, int root, MPI_Comm comm = world)
{
  return MPI_Gather( sendbuf, 1, dtype, recvbuf, 1, dtype, root, comm);
}

inline void send(void* number, int destination, int tag, MPI_Comm comm = world)
{
	MPI_Send(number, 1, dtype, destination, tag, comm);
}

inline int recv(               int source,      int tag, MPI_Comm comm = world)
{
	MPI_Status status; int number;
	MPI_Recv(&number, 1, dtype, source, tag, comm, &status);
	return number;
}