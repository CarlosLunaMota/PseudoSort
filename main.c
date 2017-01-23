#include <stdio.h>      // printf
#include <stdlib.h>     // rand, malloc, free
#include <unistd.h>     // sleep
#include <assert.h>     // assert


// PSEUDO SORTING FUNCTIONS ////////////////////////////////////////////////////

// Performs a single pass of Bubble Sort over the data (increasing by the
// number of elements that are in their final position)
//
// Returns 1 if data is already sorted, returns 0 if more work is needed:
//
int BubblePseudoSort(int *array, int length) {

    int i, aux, sorted;

    sorted = 1;
    for (i = 1; i < length; i++) { 
        if (array[i-1] > array[i]) {
            aux        = array[i-1];
            array[i-1] = array[i];
            array[i]   = aux;
            sorted = 0;
        }
    }

    return sorted;
}

// Performs a single pass of Natural Merge Sort over the data (halving the
// number of increasing runs).
//
// Returns 1 if data is already sorted, returns 0 if more work is needed:
//
// Uses and auxiliary array "aux" to avoid keep allocating and freeing memory
// but this is an optimization rather than something required by the algorithm.
//
int NaturalMergePseudoSort(int *array, int length) {

    int ini, mid, end;
    int x, y, z;
    int *aux = (int *) malloc(length * sizeof(int));
    
    // Make a single pass over the data merging pairs of adjacent runs:
    end = 0;
    while (end < length) {

        // Find new ini:
        ini = end;

        // Find new mid:
        for (mid=ini+1; mid < length && array[mid-1] <= array[mid]; mid++);

        // If we have reached the end of the array we can exit now:
        if (mid == length) { break; }
        
        // Find new end:
        for (end=mid+1; end < length && array[end-1] <= array[end]; end++);

        // Merge [ini, mid) with [mid, end) into aux:
        x = ini;
        y = mid;
        z = 0;
        while (x < mid && y < end) {
            if (array[x] <= array[y]) { aux[z++] = array[x++]; }
            else                      { aux[z++] = array[y++]; }
        }
        while (x < mid) { aux[z++] = array[x++]; }
        while (y < end) { aux[z++] = array[y++]; }

        // Copy aux into [ini, end):
        z = 0;
        x = ini;
        while (x<end) { array[x++] = aux[z++]; }
    }
    
    // Clean:
    free(aux); 

    return (ini == 0);
}

////////////////////////////////////////////////////////////////////////////////



// GENERIC SORTING FUNCTION ////////////////////////////////////////////////////

// Iterative application of a pseudosorting function to sort the data.
//
void Sort(int PseudoSortFunc (int *, int), int *array, int length) {

    int stop, iter, i;
        
    // Until done:
    stop = 0;
    iter = 0;
    while (!stop) {

        // Print current state:
        printf("Iter: %3d  ->  [", iter);
        for (i = 0; i < length-1; i++) { printf("%d, ", array[i]); }
        printf("%d]\n", array[length-1]);

        // Apply the Pseudosort:
        stop = PseudoSortFunc(array, length);
        iter++;

        sleep(1);
    }

    // Print final state:
    printf("Iter: %3d  ->  [", iter);
    for (i = 0; i < length-1; i++) { printf("%d, ", array[i]); }
    printf("%d]\n\n", array[length-1]);

    // Verify:
    for (i = 0; i < length-1; i++) { assert(array[i] <= array[i+1]); }

}
////////////////////////////////////////////////////////////////////////////////



// MAIN ////////////////////////////////////////////////////////////////////////

int main() {

    int N = 25;
    int data_size = sizeof(int);
    int *data = (int *) malloc(N * data_size);
    int i,j;

    // Sorted Data:
    for (i = 0; i < N; i++) { data[i] = i; }
    Sort(NaturalMergePseudoSort, data, N);

    // Reversed Data:
    for (i = 0; i < N; i++) { data[i] = N-i-1; }
    Sort(NaturalMergePseudoSort, data, N);

    // Random Data:
    for (j = 0; j<15; j++) {
        for (i = 0; i < N; i++) { data[i] = rand() % N; }
        Sort(NaturalMergePseudoSort, data, N);
    }

    free(data);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
