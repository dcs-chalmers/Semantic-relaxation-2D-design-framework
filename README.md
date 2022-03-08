# Artifact Description
Author: Adones Rukundo 
Contact: adones@chalmers.se, adones@must.ac.ug, adon.rkd@gmail.com

The artifact design is based on the adapted version of [ASCYLIB framework](https://github.com/LPD-EPFL/ASCYLIB). 
Memory is managed using [ssmem library](https://github.com/LPD-EPFL/ssmem), a simple object-based memory allocator with epoch-based garbage collection.

## Related Publication
* Monotonically Relaxing Concurrent Data-Structure Semantics for Increasing Performance: An Efficient 2D Design Framework. 
    * In proceedings of the 33rd International Symposium on Distributed Computing, {DISC} 2019, October 14-18, 2019, Budapest, Hungary   
* Brief Announcement: 2D-Stack - A Scalable Lock-Free Stack Design that Continuously Relaxes Semantics for Better Performance. 
    * In proceedings of the 2018 {ACM} Symposium on Principles of Distributed Computing, {PODC} 2018, Egham, United Kingdom, July 23-27, 2018
## 2D design framework implementations
The artifact contains two efficient, lock-free, concurrent data structure design framework for out-of-order semantic relaxation. The framework introduces a new two dimensional algorithmic design, that uses multiple instances of an implementation (sub-structures) of the given data structure. The first dimension (width) of the framework is the number of data structure instances operations are spread to, in order to achieve high parallelism through disjoint memory accesses. The second dimension (width) is the number of consecutive operations that try to stay at the same data structure instance in order to benefit from data locality. A combination of width and depth is what we refer to as the window. We implement two types of windows, Window Coupled and Window Decoupled. 
* Window Coupled, couples all data structure operations into one window from which operations can access the available sub-structures
* Window Decoupled, assigns each data structure operation an independent window from which the operation can access the available sub-structures
## 2D data structure implementations
* Window Coupled (2dc):
    * Stack -> ``multi-stack_2dc-window``
    * Counter -> ``multi-counter_2dc-window``
    * Deque -> ``deque-maged_2Dc-win``
* Window Decoupled (2dd):
    * Stack -> ``multi-stack_2dd-window``
    * FIFO Queue -> ``multi-queue_2dd-window``
    * Counter -> ``multi-counter_2dd-window``
    * Deque -> ``deque-maged_2Dd-win``
## Other data structure implementations
*Random selection: Under this form of semantic relaxation, a given operation randomly selects one or more sub-structures from which it further selects one sub-structure to operate on following a given creteria.
    * Stack -> ``multi-stack_random-relaxed``
    * FIFO Queue -> ``multi-queue_random-relaxed``
    * Counter -> ``multi-counter_random-relaxed`` 
## Getting Started Guide
### Prerequisites
The following are the hardware and software requirements to run the artifact
* Computer Architecture: 
    * The artifact is designed and tested for x86_64 computer architectures
* OS Platform: 
   * Linux OS distribution
* Compiler: 
    * gcc compiler is required to compile the artifact tests 

### Artifact directories:
In these directories you will find all the necessary files (source code) related to the implemented data structures and tests.
* ``benchmark/src``: Contains the data structures' source code
* ``benchmark/includes``: Contains support files e.g the window framework (relaxation_2dc-window.c, relaxation_2dd-window.c)
* ``benchmark/common``: Contains make definitions file
* ``benchmark/external``: Contains external libraries e.g the ssmem library
* ``benchmark/bin``: Contains the binary files for the compiled data structures


### Simple Compilation
* Step 1: Navigate to the 2D_framework directory
* Step 2: Run `make` This compiles all the implementations using the default make definitions

Binary files will be saved in `bin/`

### Simple test run
* Step 1: Navigate to the bin directory `bin/`
* Step 2: Run `./multi-ct_2dc`
 
An output of the test results will be displayed.

## Step by Step Instructions
### Compiling data structure tests:
* Step 1: Navigate to the benchmark directory
* Step 2: Run `make ` Without arguments, the make command compiles all data structure tests with the default settings. `make` can take arguments to define specific execution settings e.g `make VERSION=O3 GC=1 INIT=all`
    * make settings:
        * `VERSION` defines the optimisation levels e.g `VERSION=O3`. It takes one of the following five values:
            * `DEBUG` compile with optimisation zero and debug flags
            * `SYMBOL` compile with optimisation level three and -g
            *  `O0` compile with optimisation level zero
            *  `O1` compile with optimisation level one
            *  `O2` compile with optimisation level two
            *  `O3` compile with optimisation level three (Default)
        *  `GC` defines if deleted nodes should be recycled `GC=1` (Default) or not `GC=0`.
        *  `INIT` defines if data structure initialisation should be performed by all active threads `INIT=all` (Default) or one of the threads `INIT=one`
    *  More information about available make settings options can be found in the make file `common/Makefile.common`

### Defining machine specific thread pinning:
Different machines have different thread id layouts especially if the machine has more than one socket. For each affinity, dense or sparse, the machine thread id order must be defined e.g `0,2,4` means that pin the first thread to hardware thread id ``0``, second thread to hardware thread id ``2`` and third thread to hardware thread id ``4``. Machine affinity definitions are in file `includes/utils.h`.
*   Run `lscpu` to determine the order of hardware thread ids. 
*   Run `cat /proc/cpuinfo` to determine how the hardware thread ids are distributed to the core ids.
*   Use this information to define the order in which threads are pinned for either dense or sparse mapping. `dense` mapping should be defined to pin one thread per core close together. `sparse` pinning should be defined to pin one thread per core far apart e.g different sockets/tiles.
*   In the file `includes/utils.h`, the `DEFAULT` definition can be replaced with the specific mapping/affinity. Or the specific machine definition can be added.

### Running individual tests:
Binary files for each data structure implementation are in the directory `bin/`. The binary file name for each data structure implemetation is indicated within the individual make files located within data structure directory.
*   Within the make file look for the line similar to `BINS = $(BINDIR)/multi-st_2dc`, in this case, multi-st_2dc is the binary file name for the given data structure that will be stored within the `bin/` directory.


To run individual tests for each compiled data structure implementation navigate to the bin directory `bin/`.
* Run e.g `./multi-st_2dc -h` to get the information about the different parameters. An output similar to the following will be displayed
    ```sh
    ASCYLIB -- stress test 
    
    Usage:
      ./multi-st_2dc [options...]

    Options:
      -h, --help
            Print this message
      -d, --duration <int>
            Test duration in milliseconds
      -i, --initial-size <int>
            Number of elements to insert before test
      -n, --num-threads <int>
            Number of threads
      -r, --range <int>
            Range of integer values inserted in set
      -u, --update-rate <int>
            Percentage of update transactions
      -p, --put-rate <int>
            Percentage of put update transactions (should be less than percentage of updates)
      -b, --num-buckets <int>
            Number of initial buckets (stronger than -l)
      -v, --print-vals <int>
            When using detailed profiling, how many values to print.
      -f, --val-pf <int>
            When using detailed profiling, how many values to keep track of.
      -s, --side-work <int>
            thread work between data structure access operations.
      -k, --Relaxation-bound <int>
            Relaxation bound.
      -l, --Depth <int>
            Locality/Depth if k-mode is set to zero.
      -w, --Width <int>
            Fixed Width or Width to thread ratio depending on the k-mode.
      -m, --K Mode <int>
            0 for Fixed Width and Depth, 1 for Fixed Width, 2 for fixed Depth, 3 for fixed Width to thread ratio.
    ```
    
* Run e.g `./multi-st_2dc -n 6` to run  test with six threads. An output similar to the following will be displayed:
     ```sh 
    Initial, 1024 
    Range, 2048 
    Algorithm, OPTIK 
    Sizeof initial, 64.00 KB is 0.06 MB
    [ALLOC] initializing allocator with fs size, 10000 objects
    BEFORE size is, 1024
    AFTER size is, 2333 
    putting_count_total , 1917589    
    putting_count_total_succ , 1917589    
    putting_perc_succ ,      100.0 
    putting_perc ,       50.0 
    putting_effective ,       50.0 
    removing_count_total , 1917325    
    removing_count_total_succ , 1916280    
    removing_perc_succ ,       99.9 
    removing_perc ,       50.0 
    removing_effective ,       50.0 
    num_threads , 6 
    Mops , 3.835
    Ops , 3834914.00
    Push_CAS_fails , 1967109
    Pop_CAS_fails , 2549270
    Null_Count , 1045
    Hop_Count , 7657203
    Slide_Count , 2160012
    Slide-Fail_Count , 1212111
    Width , 1
    Depth , 1
    Relaxation_bound, 0
    K_mode , 0
    ```
The default parameters are defined in the file `includes/common.h`
