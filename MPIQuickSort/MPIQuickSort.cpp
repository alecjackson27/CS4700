#include <mpi.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <fstream>
#include <Windows.h>

const int MAX_SIZE = 500000;

int read_file(std::ifstream& input, int *result){
	int i = 0;
	while (input >> result[i] && i < MAX_SIZE)  
	{
		i++;
	}
	return i;
}

int partition(int *A, int lo, int hi){
	int pivot = A[lo];
	int i = lo - 1;
	int j = hi + 1;
	while (true) {
		do
			i = i + 1;
		while (A[i] < pivot);

		do
			j = j - 1;
		while (A[j] > pivot);

		if (i >= j)
			return j;

		int temp = A[i];
		A[i] = A[j];
		A[j] = temp;
	}
}

void quicksort(int *A, int lo, int hi) {
	if (lo < hi) {
		int p = partition(A, lo, hi);
		quicksort(A, lo, p);
		quicksort(A, p + 1, hi);
	}
}

int lomuto_partition(int * A, int lo, int hi) {
	int pivot = A[hi];
	int i = lo - 1;
	for (int j = lo; j < hi; j++) {
		if (A[j] <= pivot) {
			i++;
			int temp = A[i];
			A[i] = A[j];
			A[j] = temp;
		}
	}
	int temp = A[i + 1];
	A[i + 1] = A[hi];
	A[hi] = temp;
	return i + 1;
}

int sort_recursive(int* arr, int arrSize, int currProcRank, int maxRank, int rankIndex) {
	MPI_Status status;

	// Calculate the rank of sharing process
	int shareProc = currProcRank + pow(2, rankIndex);
	rankIndex++;

	// If no process is available, sort sequentially and return
	if (shareProc > maxRank) {
		quicksort(arr, 0, arrSize - 1);
		return 0;
	}

	// Divide array in two parts with the pivot in between		
	int j = 0;
	int pivotIndex;
	do {
		pivotIndex = lomuto_partition(arr, j, arrSize - 1);
		j++;
	} while (pivotIndex == j - 1);

	// Send partition based on size, sort the remaining partitions, receive sorted partition	
	if (pivotIndex <= arrSize - pivotIndex) {
		MPI_Send(arr, pivotIndex - 1, MPI_INT, shareProc, pivotIndex, MPI_COMM_WORLD);
		sort_recursive((arr + pivotIndex + 1), (arrSize - pivotIndex - 1), currProcRank, maxRank, rankIndex);
		MPI_Recv(arr, pivotIndex - 1, MPI_INT, shareProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
	else {
		MPI_Send((arr + pivotIndex + 1), arrSize - pivotIndex - 1, MPI_INT, shareProc, pivotIndex + 1, MPI_COMM_WORLD);
		sort_recursive(arr, (pivotIndex + 1), currProcRank, maxRank, rankIndex);
		MPI_Recv((arr + pivotIndex + 1), arrSize - pivotIndex - 1, MPI_INT, shareProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
}

void write_file(std::ofstream& output, int arrSize, int *arr){
	if (output.is_open()) {
		for (int i = 0; i < arrSize; i++)
			output << arr[i] << " ";
		output.close();
	}
	else
		std::cout << "Unable to open an output file.\n";
}

int main(int argc, char *argv[]) {	
	
	int size, rank;
	bool ouu = false;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int iter_count = 10;
	// This is a lame means to iterate through the program; 
	// better to use a script (.bat, python, bash, whichever you like) to gather statistics
	for (int iter = 0; iter < iter_count; iter++) {
		// calculating index for a future search of a neighbor process (see README)
		int rankPower = 0;
		while (pow(2, rankPower) <= rank)
			rankPower++;

		if (rank == 0) {
			auto start = std::chrono::high_resolution_clock::now();
			
			std::ifstream input("1m_nums.txt");
			if (!input) { std::cout << "Input file not found.\n"; return 1; }

			int *numbers = (int *)malloc(MAX_SIZE * sizeof(int));
			int arraySize = read_file(input, numbers);

			sort_recursive(numbers, arraySize, rank, size - 1, rankPower);
			// if (!ouu){
				ouu = true;
				std::ofstream output("output.txt");
				write_file(output, arraySize, numbers);
				auto finish = std::chrono::high_resolution_clock::now();
				std::cout << "Sorted an array of size " << arraySize << " in " << double(std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count()) / 1000 
					<< " seconds with "<< size << " processes." << std::endl;
			// }
		}
		else {
			MPI_Status status;
			int subarray_size;
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			// Capturing size of the array to receive
			MPI_Get_count(&status, MPI_INT, &subarray_size);
			
			int source_process = status.MPI_SOURCE;
			int *subarray = (int*)malloc(subarray_size * sizeof(int));

			MPI_Recv(subarray, subarray_size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sort_recursive(subarray, subarray_size, rank, size - 1, rankPower);
			MPI_Send(subarray, subarray_size, MPI_INT, source_process, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
    return 0;
}
