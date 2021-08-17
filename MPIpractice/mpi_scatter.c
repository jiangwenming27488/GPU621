#include <stdio.h>
#include <mpi.h>
#define D_SIZE 6

double avg(int num, double** data) {
   double sum = 0;
   double avg = 0;
   for (int i = 0; i < 1; i++) {
      for (int j = 0; j < num; j++) {
         sum += data[1][j];
      }
   }
   return avg = num / sum;
}

int main(int argc, char** argv) {
   int rank, np, i, j, ip;

   double numbs[4][D_SIZE] = {
         {23.2, 21.6, 16.7, 13.9},
         {16.7, 14.5, 12.7},
         {11.3, 12.9, 19.4},
         {56.3, 62.3, 54.1, 32.1, 53.7, 61.2}
   };

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   if (rank == 0) {
      double data[4][D_SIZE];
      for (j = 0; j < 4; j++) {
         for (i = 0; i < D_SIZE; i++)
            data[j][i] = numbs[j][i];
      }

      MPI_Bcast(&data[0], D_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&data[1], D_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&data[2], D_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&data[3], D_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

   }
   else {
      double data[4][D_SIZE];
      MPI_Bcast(data, D_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&data[1], D_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&data[2], D_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&data[3], D_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      if (rank == 1) {
         double a = avg(4, data[0]);
         printf("Toronto avg %f:", &a);
         for (i = 0; i < D_SIZE; i++)
            if (data[0][i] != 0.00)
               printf("%.2lf ", data[0][i]);
         MPI_Send(data, D_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
         printf("\n");
      }
      else if (rank == 2) {
         float a = avg(3, data[1]);
         printf("Ottawa avg %f: ", &a);
         for (i = 0; i < D_SIZE; i++)
            if (data[1][i] != 0.00)
               printf("%.2lf ", data[1][i]);
         printf("\n");
      }
      else if (rank == 3) {
         float a = avg(3, data[2]);
         printf("Hamiltion avg %f: ", &a);
         for (i = 0; i < D_SIZE; i++)
            if (data[2][i] != 0.00)
               printf("%.2lf ", data[2][i]);
         printf("\n");
      }
      else if (rank == 4) {
         float a = avg(5, data[3]);
         printf("Oshawa avg %f: ", &a);
         for (i = 0; i < D_SIZE; i++)
            if (data[3][i] != 0.00)
               printf("%.2lf ", data[3][i]);
         printf("\n");
      }
   }

   MPI_Finalize();
   return 0;
}
