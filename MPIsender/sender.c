
// MPI MPMD
// mpi_sender.c
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAX_DATA 1000
#define MAX_MSG (MAX_DATA+20)

int main(int argc, char** argv) {
   int rank;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   char msg[MAX_MSG + 1];
   char data[MAX_DATA + 1] = "Good Morning Sunshine!";
   sprintf_s(msg, MAX_MSG, "Process %d says %s", rank, data);
   int len = strlen(msg);
   MPI_Send(msg, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
   MPI_Finalize();
   return 0;
}