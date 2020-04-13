#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

int main(void){
  int sum, comm_sz, my_rank, i, next, value;
  int divisor = 2;
  int core_difference = 1;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  srandom((unsigned)time(NULL) + my_rank);
  value = random() % 10;

      //process should recieve and add
      if (my_rank % divisor == 0){

          printf("IF----");

          printf("Process %d generates: %d\n", my_rank, value);

          for (i = 0; i < comm_sz; i++)
          {
              MPI_Recv(&value, 1, MPI_INT, i, my_rank , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               sum += value;  
               printf("Current Sum=: %d\n", sum);

          }

          printf("The NEW divisor is:%d\n", divisor);
          divisor *= 2;
          core_difference *= 2;

      }

      //sending the random value - no calculation
      else if (my_rank % divisor == core_difference){
          printf("ELSE----");
          printf("Process %d generates: %d\n", my_rank, value);
          MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
      else
         if (my_rank==0)
            printf("Sum=: %d\n", sum);

  MPI_Finalize();

  return 0;
}