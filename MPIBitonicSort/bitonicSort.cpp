#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include <string>
#define MCW MPI_COMM_WORLD

using namespace std;

void print(int rank, int value, string msg, int size) {
	if (rank != 0) {
    	MPI_Send(&value,1,MPI_INT,0,1,MCW);
    }
    Sleep(1000);
    if (rank == 0) {
    	cout << msg << ": " << value << " ";
    	int data;
    	for (int i = 1; i < size; i++) {
    		MPI_Recv(&data,1,MPI_INT,i,1,MCW,MPI_STATUS_IGNORE);
    		cout << data << " ";
    	}
    	cout << endl;
    }
}

int main(int argc, char **argv){
	int rank, size;
    int data;
    int value;
    int dest;
    bool descending;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank); 
    MPI_Comm_size(MCW, &size);
    srand(time(NULL)+rank);
    int mask;
    int steps = int(log2(size));

    if(!rank) {
    	value = rand()%50+1;
    	for (int i = 1; i < size; i++) {
    		data = rand()%50+1;
    		MPI_Send(&data,1,MPI_INT,i,2,MCW);
    	}
    	Sleep(1000);
    } else {
    	MPI_Recv(&data,1,MPI_INT,0,2,MCW,MPI_STATUS_IGNORE);
    	value = data;
    	Sleep(1000);
    }
    
    string msg = "Unsorted Array";
    print(rank, value, msg, size);

    for (int step = 1; step <= steps; step++) {
    	mask = 1 << step;
    	//if (!rank) cout << "Mask: " << mask << endl;
    	descending = (mask & rank) >> step;
    	//cout << "Step: " << step << "   Rank: " << rank << "   Descending: " << descending << endl;

    	while(mask) {
    		mask >>=1;
    		dest = rank ^ mask;
    		MPI_Send(&value,1,MPI_INT,dest,0,MCW);
    		MPI_Recv(&data,1,MPI_INT,dest,0,MCW,MPI_STATUS_IGNORE);
    		if(!descending) {
		    	if((rank & mask) == 0) {
		    		if(data < value) value = data;
		    	} else {
		    		if(data > value) value = data;
		    	}
		    } else {
		    	if((rank & mask) == 0) {
		    		if(data > value) value = data;
		    	} else {
		    		if(data < value) value = data;
		    	}
		    }
    		
    	}
    	string s = to_string(step);
    	msg = "After Step " + s + " ";
    	print(rank, value, msg, size);
    }

    MPI_Finalize();
	return 0;
}