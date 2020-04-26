#!/bin/bash
# used this bash script to record Haskell quicksort runtimes
for i in {2..7}
do
	for j in {1..4}
	do
		echo "using $j cores:"
		./ParallelQuickSort +RTS -N$j -RTS $((10**i))
		echo ""
	done
done	
