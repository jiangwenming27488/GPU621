
// MPI MPMD
// mpi_receiver.c
#include <stdio.h>
#include <mpi.h>

#define MAX_DATA 1000
#define MAX_MSG (MAX_DATA+20)

int main(int argc, char** argv) {
   MPI_Status status;
   int ip, np;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   char msg[MAX_MSG + 1];
   for (ip = 1; ip < np; ip++) {
      MPI_Recv(msg, MAX_MSG, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
         MPI_COMM_WORLD, &status);
      int nchar;
      MPI_Get_count(&status, MPI_CHAR, &nchar);
      msg[nchar] = '\0';
      printf("%s\n", msg);
   }
   printf("You bet!!\n");
   MPI_Finalize();
   return 0;
}