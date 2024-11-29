# file_based_heap_storage
File Based DB Storage indexed with Min Heap for faster access.

We assume that each data file has a limit L on the number of records that it can store. When one wants to
add a new record to a data file already storing L records, the file is split into two parts. One part consists of
the records pertaining to the smaller L/2 roll numbers, and the other part the larger L/2 roll numbers. Then,
the record for the new roll number is inserted in the appropriate part.


Although it is not very efficient, we assume that each file stores the records as a min-heap in the contiguous
representation. The heap ordering is with respect to the primary key (a number). So long as L is small,
this strategy is fine.

A heap is typically implemented in an array. Here the array resides in a file. 

Whenever a particular item in the heap is to be accessed, you can straightaway jump to that location using the call fseek() declared in stdio.h.


Min-heaps are managed in files. Binary search tree (BST) are used for indexing the files. The BST resides in the main memory. 

Given a number, the BST will guide you to a unique file which may contain the number (ideally in practical implementation the file will have additional data correcponsing to the number). 

Moreover, if you want to insert the record, the BST will identify the correct unique file where this
record is to be inserted. If that insertion attempt lets the file exceed its storage capacity of L records, the file
is split into two parts as explained above. 

The BST may be kept height-balanced (this is not a part of this
assignment). If the database stores n student records, then there will be Q(n/L) files, so the height of the
BST will be Q(log(n/L)).
