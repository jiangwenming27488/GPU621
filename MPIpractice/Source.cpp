// An MPI Program - Hello World
// mpi.cpp

#include <cstdio>
#include <mpi.h> // header file

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv); // initializing MPI
   // MPI statements here ... parallel processing 
   std::printf("Hello from an MPI process\n");
   MPI_Finalize(); // closing MPI
}