// Workshop 9 - Discretize the Elements of an Array
// Chris Szalwinski
// 2020/11/27
// w9.mpi.c

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "mpi.h"
#define TIME 1

// report processor time
//
void reportTime(const char* msg, int i, double span) {
   printf("Process %d: %-12s took %7.4lf seconds\n", i, msg, span);
}

// discretize data[n] into 0s and 1s
//
void discretize(float* data, int n) {
   for (int i = 0; i < n; i++)
      data[i] = (pow(sin(data[i]), cos(data[i])) +
         pow(cos(data[i]), sin(data[i]))) / 2.0f;
}

void err(MPI_Comm* comm, int* err) {
   printf("Error code %d\n", *err);
}

int main(int argc, char** argv) {
   if (argc != 2) {
      fprintf(stderr, "%s : invalid number of arguments\n"
         "Usage: %s no_of_elements\n", argv[0], argv[0]);
      return 1;
   }

   // retrieve number of elements
   int n = atoi(argv[1]);

   int rank, np;
   //
   // initialize MPI
   //
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Status status;
   MPI_Errhandler error;

   int nPerProcess = n / np;
   n = nPerProcess * np;
   int zeroes = 0;
   float* data = NULL;
   float* result = NULL;

   // get MPI time start
   double begin, end;

   if (rank == 0) {
      // initialization

      // get MPI time start
      begin = MPI_Wtime();
      data = (float*)malloc(2 * n * sizeof(float));
      result = (float*)malloc(n * sizeof(float));
      // set original data
      for (int i = 0; i < n; i++)
         data[i] = (float)rand() / RAND_MAX;

      // get MPI time end
      end = MPI_Wtime();
      reportTime("Allocation", 0, end - begin);

      // conversion

      // get MPI time start
      begin = MPI_Wtime();
      // scatter original data to all workers including master
      MPI_Scatter(data, nPerProcess, MPI_FLOAT, MPI_IN_PLACE, 0, MPI_FLOAT, 0, MPI_COMM_WORLD); 
      discretize(data, nPerProcess);

      // gather converted data from all workers
      MPI_Gather(data, nPerProcess, MPI_FLOAT, result, nPerProcess, MPI_FLOAT, 0, MPI_COMM_WORLD); 
      // get MPI time end
      end = MPI_Wtime();
      // identify 0s and 1s
      for (int i = 0; i < nPerProcess; i++)
         if (data[i] < 0.707f)
            zeroes++;
      for (int i = nPerProcess; i < n; i++)
         if (result[i] < 0.707f)
            zeroes++;

      // report timing statistics

      // error handler
      MPI_Comm_create_errhandler(err, &error);
      MPI_Comm_set_errhandler(MPI_COMM_WORLD, error);

      double* time = (double*)malloc(nPerProcess * sizeof(double));

      double totalTime = 0;
      reportTime("Conversion", 0, end - begin);
      for (int i = 1; i < np; i++) {
         
         // collect time for process i 
         MPI_Recv(time, nPerProcess, MPI_DOUBLE, i, TIME, MPI_COMM_WORLD, &status); 
       
         reportTime("Conversion", i, *time);
         totalTime += *time;
         
      }
      reportTime("Sum of All", 0, totalTime);
      free(time);
   }
   else {
      // worker process
      float* data = (float*)malloc(nPerProcess * sizeof(float));
      double* time = (double*)malloc(nPerProcess * sizeof(double));

      // get MPI time
      begin = MPI_Wtime();
      // scatter receiver of original data
      MPI_Scatter(NULL, nPerProcess, MPI_FLOAT, data, nPerProcess, MPI_FLOAT, 0, MPI_COMM_WORLD);
      discretize(data, nPerProcess);

      // gather converted data and send to master
      MPI_Gather(data, nPerProcess, MPI_FLOAT, NULL, nPerProcess, MPI_FLOAT, 0, MPI_COMM_WORLD);
      // get MPI time
      end = MPI_Wtime();

      *time = end - begin;

      free(data);

      // report time
      reportTime("Conversion", 0, end - begin);
      // send time to master
      MPI_Send(time, nPerProcess, MPI_DOUBLE, 0, TIME, MPI_COMM_WORLD);
      free(time);
   }

   // get MPI time end

   if (rank == 0) {
      reportTime("Wall clock", 0, end - begin);

      // get MPI time start
      begin = MPI_Wtime();
      free(data);
      free(result);

      // get MPI time end
      end = MPI_Wtime();
      reportTime("Deallocation", 0, end - begin);
      printf("Result: %d = %d (0s) + %d (1s)\n", n, zeroes, n - zeroes);
   }

   // close MPI
   MPI_Finalize();
   return 0;
}