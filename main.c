#include <stdio.h>      // printf
#include <stdlib.h>     // rand, malloc, free
#include <unistd.h>     // sleep
#include <assert.h>     // assert
#include <string.h>     // memcpy, memmove
#include <time.h>       // clock, CLOCKS_PER_SEC



// PRESORTEDNESS MEASURES //////////////////////////////////////////////////////

// Counts the number of inversions of a given array.
//
int Inversions(int *array, int length) {

    int inversions = 0;

    for (int i=0; i<length-1; i++) {
        for (int j=i+1; j<length; j++) {
            if (array[i] > array[j]) { inversions++; }
        }
    }

    return inversions;
}

// Counts the number of runs of a given array.
//
int Runs(int *array, int length) {

    int runs = 1;

    for (int i=0; i<length-1; i++) {
        if (array[i] > array[i+1]) { runs++; }
    }

    return runs;
}

// Computes the length of the longest run.
//
int MaxRun(int *array, int length) {

    int max = 0;
    int run = 1;

    for (int i=0; i<length-1; i++) {
        if (array[i] <= array[i+1]) { run++; }
        else {
            max = (max < run) ? run : max;
            run = 1;
        }
    }
    
    return (max < run) ? run : max;
}

// Computes the length of the first run.
//
int FirstRun(int *array, int length) {

    int run = 1;

    for (int i=0; i<length-1; i++) {
        if (array[i] <= array[i+1]) { run++; }
        else                        { break; }
    }
    
    return run;
}

////////////////////////////////////////////////////////////////////////////////



// PSEUDO SORTING FUNCTIONS ////////////////////////////////////////////////////

// Iterative application of a pseudosorting function to sort the array.
//
// Returns the number of iterations performed.
//
void Sort(int PseudoSortFunc(int *array, int length), int *array, int length) {

    int DEBUG = 0;  // Boolean
    int stop = 0;   // Boolean
    int iter = 0;   // Integer
    int i;          // Index variable

    // Print Initial State:
    if (DEBUG) {
        printf("%4d) Runs:%6d    MaxRun:%6d    FirstRun:%6d    Invs:%10d",
               iter++, Runs(array, length), MaxRun(array, length),
               FirstRun(array, length), Inversions(array, length));
        if (length <= 25) {
            printf("  ->  [");
            for (i = 0; i < length-1; i++) { printf("%d, ", array[i]); }
            printf("%d]", array[length-1]);
        }
        printf("\n");
    }

    // Until done:
    while (!stop) {

        // Apply the Pseudosort:
        stop = PseudoSortFunc(array, length);

        // Print Current State:
        if (DEBUG) {
            printf("%4d) Runs:%6d    MaxRun:%6d    FirstRun:%6d    Invs:%10d",
                   iter++, Runs(array, length), MaxRun(array, length),
                   FirstRun(array, length), Inversions(array, length));
            if (length <= 25) {
                printf("  ->  [");
                for (i = 0; i < length-1; i++) { printf("%d, ", array[i]); }
                printf("%d]", array[length-1]);
            }
            printf("\n");
        }       
    }
    
    // Verify:
    for (i = 0; i < length-1; i++) { assert(array[i] <= array[i+1]); }
}

// Performs a single pass of Bubble Sort over the data (increasing the number
// of elements that are in their final position).
//
// Returns 1 if data is already sorted, returns 0 otherwise.
//
int BubblePseudoSort(int *array, int length) {

    int sorted = 1;     // Boolean
    int i;              // Index variable
    int aux;            // Same type of array content
    
    // Bubble Up:
    for (i = 1; i < length; i++) {
        if (array[i-1] > array[i]) {
            aux        = array[i-1];
            array[i-1] = array[i];
            array[i]   = aux;
            sorted     = 0;
        }
    }

    return sorted;
}

// Performs a single pass of Heapify over the data (decreasing the number of
// inversions in the array).
//
// Returns 1 if data is already sorted, returns 0 otherwise.
//
int HeapifyPseudoSort(int *array, int length) {

    int root, parent, child, ind;   // Index variables
    int aux;                        // Same type of array content
    
    // Find the end of the first run:
    for (ind = 1; ind < length && array[ind-1] <= array[ind]; ind++);

    // If we are done return:
    if (ind == length) { return 1; }

    // Now find the correct place to insert this element: 
    aux = array[ind];
    for (ind = 0; ind < length && aux >= array[ind]; ind++);
    
    // Min-Heapifiy [ind,length-1] so array[ind] == min(array[ind,length-1])
    for (root = length-1; root >= ind; root--) {
        parent = root;
        while (1) {
            child = ind + ((parent-ind)*2 + 1);
            if (child >= length) { break; }
            if (child+1 < length && array[child] > array[child+1]) { child++; }
            if (array[parent] <= array[child]) { break; }
            else {
                aux           = array[parent];
                array[parent] = array[child];
                array[child]  = aux;
                parent        = child;
            }
        }
    }

    return 0;
}

// Performs a single pass of Natural Merge Sort over the data (halving the
// number of increasing runs).
//
// Returns 1 if data is already sorted, returns 0 otherwise.
//
int NaturalMergePseudoSort(int *array, int length) {

    int x,   y,   z   = 0;                  // Index variables: indices
    int ini, mid, end = 0;                  // Index variables: limits
    size_t size = sizeof(int);              // Size of content type
    int *aux = (int *) malloc(length*size); // Same type of array content

    // Make a single pass over the data merging pairs of adjacent runs:
    while (end < length) {

        // Find new ini:
        ini = end;

        // Find new mid:
        for (mid = ini+1; mid < length && array[mid-1] <= array[mid]; mid++);

        // If we have reached the end of the array we can exit now:
        if (mid == length) { break; }

        // Find new end:
        for (end = mid+1; end < length && array[end-1] <= array[end]; end++);

        // Merge array[ini, mid) with array[mid, end) into aux[z, z+end-ini):
        x = ini;
        y = mid;
        while (x < mid && y < end) {
            if (array[x] <= array[y]) { aux[z++] = array[x++]; }
            else                      { aux[z++] = array[y++]; }
        }
        if (x < mid) { memcpy(&aux[z], &array[x], (mid-x)*size); z += mid-x; }
        if (y < end) { memcpy(&aux[z], &array[y], (end-y)*size); z += end-y; }
    }

    // Copy aux back into array:
    memcpy(array, aux, end*size);

    // Clean & return:
    free(aux);
    return (ini == 0);
}

////////////////////////////////////////////////////////////////////////////////



// SORTING FUNCTIONS ///////////////////////////////////////////////////////////

// Performs a complete Binary Insertion Sort over the data.
//
// O(N^2) time, O(1) memory, Stable, Fast for small or almost sorted arrays.
//
void BinaryInsertionSort(int *array, int length) {

    int i, j, left, right;  // Index variables
    int aux;                // Same type of array content

    for (i = 1; i < length; i++) {

        // Binary search of the insertion point: O(log(i))
        aux   = array[i];
        left  = 0;
        right = i;
        j     = i/2;
        while (left < right) {
            if (aux < array[j]) { right = j;   }
            else                { left  = j+1; }
            j = left + (right-left)/2;
        } 

        // Insertion: O(i), but at least as optimized as possible.
        if (j < i) {
            memmove(&array[j+1], &array[j], (i-j)*sizeof(int));
            array[j] = aux;
        }
    }
    
    // Verify:
    for (i = 0; i < length-1; i++) { assert(array[i] <= array[i+1]); }
}

// Performs a complete Heap Sort over the data.
//
// O(N*log(N)) time, O(1) memory, Unstable.
//
void HeapSort(int *array, int length) {

    int parent, child, i;   // Index variables
    int aux;                // Same type of array content

    // Heapify the array:
    for (i = (length-2)/2; i >= 0; i--) {
        parent = i;
        while (1) {
            child = parent*2 + 1;
            if (child > length-1) { break; }
            if (child+1 <= length-1 && array[child] < array[child+1]) { child++; }
            if (array[parent] >= array[child]) { break; }
            else {
                aux           = array[parent];
                array[parent] = array[child];
                array[child]  = aux;
                parent        = child;
            }
        }
    }

    // Selection Sort phase:
    for (i = length-1; i > 0; i--) {
        aux      = array[i];
        array[i] = array[0];
        array[0] = aux;
        parent   = 0;
        while (1) {
            child = parent*2 + 1;
            if (child > i-1) { break; }
            if (child+1 <= i-1 && array[child] < array[child+1]) { child++; }
            if (array[parent] >= array[child]) { break; }
            else {
                aux           = array[parent];
                array[parent] = array[child];
                array[child]  = aux;
                parent        = child;
            }
        }
    }
    
    // Verify:
    for (i = 0; i < length-1; i++) { assert(array[i] <= array[i+1]); }
}

// Performs a complete Natural Merge Sort over the data.
//
// O(N*log(N)) time, O(N) memory, Stable.
//
// It is equivalent but more efficient than:
//
//      for (stop = 0; !stop; stop = NaturalMergePseudoSort(array, length);
//
void NaturalMergeSort(int *array, int length) {

    int x, y, z;                            // Index variables: indices
    int end, mid, ini = length;             // Index variables: limits
    size_t size = sizeof(int);              // Size of content type
    int *aux = (int *) malloc(length*size); // Same type of array content

    // While data is not fully sorted:
    while (ini > 0) {

        // Make a single pass over the data merging pairs of adjacent runs:
        z = end = 0;
        while (end < length) {

            // Find new ini, mid & end:
            ini = end;
            for (mid = ini+1; mid < length && array[mid-1] <= array[mid]; mid++);
            for (end = mid+1; end < length && array[end-1] <= array[end]; end++);
            if (end > length) { end = length; }
            
            // Merge array[ini, mid) with array[mid, end) into aux[z, z+end-ini):
            x = ini;
            y = mid;
            while (x < mid && y < end) {
                if (array[x] <= array[y]) { aux[z++] = array[x++]; }
                else                      { aux[z++] = array[y++]; }
            }
            if (x < mid) { memcpy(&aux[z], &array[x], (mid-x)*size); z += mid-x; }
            if (y < end) { memcpy(&aux[z], &array[y], (end-y)*size); z += end-y; }
        }

        // If it is already sorted: Copy aux back into array & break
        if (ini == 0) { memcpy(array, aux, length*size); break; }

        // Make a single pass over the data merging pairs of adjacent runs:
        z = end = 0;
        while (end < length) {

            // Find new ini, mid & end:
            ini = end;
            for (mid = ini+1; mid < length && aux[mid-1] <= aux[mid]; mid++);
            for (end = mid+1; end < length && aux[end-1] <= aux[end]; end++);
            if (end > length) { end = length; }
            
            // Merge aux[ini, mid) with aux[mid, end) into array[z, z+end-ini):
            x = ini;
            y = mid;
            while (x < mid && y < end) {
                if (aux[x] <= aux[y]) { array[z++] = aux[x++]; }
                else                  { array[z++] = aux[y++]; }
            }
            if (x < mid) { memcpy(&array[z], &aux[x], (mid-x)*size); z += mid-x; }
            if (y < end) { memcpy(&array[z], &aux[y], (end-y)*size); z += end-y; }
        }
    }

    // De-allocate aux:
    free(aux);

    // Verify:
    for (x = 0; x < length-1; x++) { assert(array[x] <= array[x+1]); }
}

// Performs a complete (Bottom Up) Merge Sort over the data.
//
// O(N*log(N)) time, O(N) memory, Stable.
//
void MergeSort(int *array, int length) {

    int x, y, z, step = 1;                  // Index variables: indices
    int end, mid, ini = length;             // Index variables: limits
    size_t size = sizeof(int);              // Size of content type
    int *aux = (int *) malloc(length*size); // Same type of array content

    // While data is not fully sorted:
    while (ini > 0) {

        // Make a single pass over the data merging pairs of adjacent runs:
        z = end = 0;
        while (end < length) {

            // Find new ini, mid & end:
            ini = end;
            mid = (ini+step > length) ? length : ini+step;
            end = (mid+step > length) ? length : mid+step;
            
            // Merge array[ini, mid) with array[mid, end) into aux[z, z+end-ini):
            x = ini;
            y = mid;
            while (x < mid && y < end) {
                if (array[x] <= array[y]) { aux[z++] = array[x++]; }
                else                      { aux[z++] = array[y++]; }
            }
            if (x < mid) { memcpy(&aux[z], &array[x], (mid-x)*size); z += mid-x; }
            if (y < end) { memcpy(&aux[z], &array[y], (end-y)*size); z += end-y; }
        }
        
        // Double the step size:
        step *= 2;

        // Make a single pass over the data merging pairs of adjacent runs:
        z = end = 0;
        while (end < length) {

            // Find new ini, mid & end:
            ini = end;
            mid = (ini+step > length) ? length : ini+step;
            end = (mid+step > length) ? length : mid+step;

            // Merge aux[ini, mid) with aux[mid, end) into array[z, z+end-ini):
            x = ini;
            y = mid;
            while (x < mid && y < end) {
                if (aux[x] <= aux[y]) { array[z++] = aux[x++]; }
                else                  { array[z++] = aux[y++]; }
            }
            if (x < mid) { memcpy(&array[z], &aux[x], (mid-x)*size); z += mid-x; }
            if (y < end) { memcpy(&array[z], &aux[y], (end-y)*size); z += end-y; }
            
        }

        // Double the step size:
        step *= 2;
    }

    // De-allocate aux:
    free(aux);

    // Verify:
    for (x = 0; x < length-1; x++) { assert(array[x] <= array[x+1]); }
}

////////////////////////////////////////////////////////////////////////////////



// MAIN ////////////////////////////////////////////////////////////////////////

int main() {

    int i, j;
    int length = 100000;
    int trials = 10;
    int *array = (int *) malloc(length*sizeof(int));
    clock_t start;
    int cmp(const void * a, const void * b) { return (*(int*)a - *(int*)b); }

    printf("\nSorting %d random arrays of %d integers:\n", trials, length);


    // Bubble Pseudo-Sort O(n^2)
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        Sort(BubblePseudoSort, array, length);        
    }
    printf("\tBubblePseudoSort:\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);


    // Heapify Pseudo-Sort O(n^2)
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        Sort(HeapifyPseudoSort, array, length);        
    }
    printf("\tHeapifyPseudoSort:\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);


    // Binary Insertion Sort O(n^2)
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        BinaryInsertionSort(array, length);
    }
    printf("\tBinaryInsertionSort:\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);


    // Heapify + Insertion Sort O(n^2)
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        HeapifyPseudoSort(array, length);
        BinaryInsertionSort(array, length);
    }
    printf("\tHeapify+InsertionSort:\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);
        

    // Heap Sort O(n*log(n))
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        HeapSort(array, length);
    }
    printf("\tHeapSort:\t\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);

    // Natural Merge Pseudo-Sort O(n*log(n))
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        Sort(NaturalMergePseudoSort, array, length);        
    }
    printf("\tNaturalMergePseudoSort:\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);


    // Natural Merge Sort O(n*log(n))
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        NaturalMergeSort(array, length);
    }
    printf("\tNaturalMergeSort:\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);


    // Merge Sort O(n*log(n))
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        MergeSort(array, length);
    }
    printf("\tMergeSort:\t\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);


    // Quick Sort O(n*log(n))
    start = clock();
    for (j = 0; j<trials; j++) {
        for (i = 0; i < length; i++) { array[i] = rand() % length; }
        qsort(array, length, sizeof(int), cmp);
    }
    printf("\tC qsort:\t\t%6.2f s\n", (double)(clock()-start)/CLOCKS_PER_SEC);


    // Clean & Return:
    free(array);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
