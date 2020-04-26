#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <ctime>
#include <windows.h>
using namespace std;

const int MAX_CONTRIB = 10;

int Global_sum(int my_contrib, int my_rank, int p, MPI_Comm comm);

int main(int argc, char* argv[]) {
  int   p, my_rank;
  MPI_Comm comm;
  int   my_contrib;
  int   sum;

  MPI_Init(&argc, &argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &p);
  MPI_Comm_rank(comm, &my_rank);

  srand((unsigned)time(NULL) + my_rank);
  my_contrib = rand() % MAX_CONTRIB;

  if (my_rank == 0) {
    clock_t start = clock();
    double duration;
    // printf("Proc %d: contribution = %d\n", my_rank, my_contrib);
    sum = Global_sum(my_contrib, my_rank, p, comm);
    // printf("Proc %d > global sum = %d\n", my_rank, sum);
    duration = (clock()-start) / (double)CLOCKS_PER_SEC;
    cout <<"Number of nodes: " << p << "\nGlobal sum: " << sum << "\nTime: " << duration << " seconds\n";
  }
  else {
    // printf("Proc %d: contribution = %d\n", my_rank, my_contrib);
    sum = Global_sum(my_contrib, my_rank, p, comm);
  }

  MPI_Finalize();
  return 0;
} 

int Global_sum(int my_contrib, int my_rank, int p, MPI_Comm comm) {
   int     sum = my_contrib;
   int     temp;
   int     partner;
   int     done = 0;
   unsigned  bitmask = 1;
   MPI_Status status;

   while (!done && bitmask < p) {
     partner = my_rank ^ bitmask;
     if (my_rank < partner) {
      MPI_Recv(&temp, 1, MPI_INT, partner, 0, comm, &status);
      sum += temp;
      bitmask <<= 1;
     } else {
      MPI_Send(&sum, 1, MPI_INT, partner, 0, comm); 
      done = 1;
     }
   }
   return sum;
}