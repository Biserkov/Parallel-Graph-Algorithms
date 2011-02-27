#include <mpi.h>
#pragma once

MPI_Comm world = MPI_COMM_WORLD;
int rootID = 0;
const int message_size = 1;

MPI_Datatype dtype = MPI_INT;

inline int bcast(void* number, int root, MPI_Comm comm = world)
{
	return MPI_Bcast(number, message_size, dtype, root, comm);
}

inline int myID(MPI_Comm comm = world)
{
	int t = -1;
	MPI_Comm_rank(comm, &t);
	return t;
}

inline int scatter(void* sendbuf, void *recvbuf, int root, MPI_Comm comm = world)
{
	return MPI_Scatter(sendbuf, message_size, dtype, recvbuf, message_size, MPI_INT, root, comm);
}

inline int gather(void* sendbuf, void *recvbuf, int root, MPI_Comm comm = world)
{
  return MPI_Gather( sendbuf, message_size, dtype, recvbuf, message_size, dtype, root, comm);
}

inline void send(void* number, int destination, int tag, MPI_Comm comm = world)
{
	MPI_Send(number, message_size, dtype, destination, tag, comm);
}

inline int recv(               int source,      int tag, MPI_Comm comm = world)
{
	MPI_Status status; int number;
	MPI_Recv(&number, message_size, dtype, source, tag, comm, &status);
	return number;
}