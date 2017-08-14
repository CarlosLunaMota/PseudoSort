# PseudoSorting algorithms

A **pseudosorting algorithm** takes a sequence and returns a permutation of that sequence that is _more sorted_ than the previous one. The precise definition of _more sorted_ it is not important as long as we guarantee that:

1. Repeated aplications of the same pseudosorting algorithm will eventually sort the sequence.
2. Applying a pseudosorting algorithm to a sorted sequence will leave it sorted.

It is also necessary to impose that each application of a pseudosorting algorithm requires (strictly) less than N·log(N) steps to avoid including all sorting algorithms into the pseudosorting category.

Finally, we will say that a pseudosorting algorithm is **optimal** if the following generic sorting algorithm is able to sort any sequence in O(N·log(N)) steps:

```c
void Sort(int PseudoSortFunc (int *, int), int *array, int length) {
    while (is_not_sorted(array)) {
        PseudoSortFunc(array, length);
    }
}
```

Lets assume for simplicity that ```is_not_sorted(array)``` can evaluate if the array is sorted in constant time.

## Simple PseudoSorting algorithms

Any _memoryless_ sorting algorithm that performs several iterations over the data may be regarded as the repeated application of a pseudosorting algorithm. By _memoryless_ I mean that it is not necessary to store any additional information (apart from the array itself) from one iteration to the next.

For example, we can use a _single pass_ of **BubbleSort** as a pseudosorting algorithm and call it **BubblePseudoSort**:

```c
void BubblePseudoSort(int *array, size_t length) {
    size_t i;       // Index variable
    int    aux;     // Same type of array content
    
    // Bubble-up
    for (i = 1; i < length; i++) {
        if (array[i-1] > array[i]) {
            aux        = array[i-1];
            array[i-1] = array[i];
            array[i]   = aux;
        }
    }
}
```

We can safely break BubbleSort into a series of independent applications of BubblePseudoSort because it is a very naive sorting algorithm that keeps comparing elements that have been previously compared. Other sorting algorithms (like Quicksort, Heapsort or MergeSort) exploit previous comparisons but do so by storing some information (sometimes implicitly) and thus can not be broken into independent applications of a pseudosorting algorithm as easily as BubbleSort.

For example, QuickSort keeps in memory the position of the previous _pivots_, HeapSort remembers the amount of elements already in their final position and (bottom-up) MergeSort only needs to remember how many iterations has been performed so far.

At this point it seems that it is impossible to have an optimal pseudosorting algorithm and that we need to pay the price of storing some bits between iterations to achieve the O(N·log(N)) performance. Fortunately, it is not the case.

## Optimal PseudoSorting Algorithms

**Natural MergeSort** is a bottom-up variant of MergeSort that does not require us to store information between iterations. It sorts the data by merging pairs of consecutive runs (incresing subsequences). Therefore, we can easily define **NaturalMergePseudoSort** as:

```c
void NaturalMergePseudoSort(int *array, size_t length) {

    size_t x,   y,   z   = 0;                   // Index variables: indices
    size_t ini, mid, end = 0;                   // Index variables: limits
    size_t size = sizeof(int);                  // Size of content type
    int   *aux = (int *) malloc(length*size);   // Same type of array content

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

    // Copy aux[0, end) back into array:
    memcpy(array, aux, end*size);

    // De-allocate aux:
    free(aux);
}
```

Since each application of NaturalMergePseudoSort will halve the number of runs in the sequence it is guaranteed that we will perform at most log(N) iterations before the sequence gets sorted and it is equally easy to see that each aplication of NaturalMergePseudoSort requires O(N) steps, so we can conclude that **NaturalMergePseudoSort is an optimal pseudosorting algorithm**.

As usual with other variants of MergeSort, NaturalMergePseudoSort is stable and requires O(N) aditional (temporary) space. As happens to the standar Natural MergeSort, the NaturalMergePseudoSort shows adaptability to the number of runs of the sequence (as defined by Heikki Mannila in _Measures of presortedness and optimal sorting algorithms, IEEE Trans. Comput. 34 (1985) 318-325_).

## Open Questions:

* Are there other optimal pseudosorting algorithms?
* Can we transform quicksort or heapsort in the repeated aplication of a pseudosorting algorithm?
