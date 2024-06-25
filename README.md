# Memory-Management-Simulator

This project simulates how memory is managed in a computer, using 4 different algorithms: **first** fit, **best** fit, **worst** fit, and **next** fit.  
The simulator will take processes of varying sizes, load them into memory according to the chosen algorithm (see Algorithms below) and swap process out as needed to create larger holes.
Written in C.

## Compilation Instructions
- Navigate to the root directory  
- Run `make`

## Running the simulation
- Navigate to the root directory  
- Create an input file that matches the format provided below
    - Input file is validated on program launch
    - A test file `testfile.txt` is provided in the root directory  
- run `./holes [inputfile] <first|best|worst|next>`, where
    - `[inputfile]` is the input file created in the previous step
    - `<first|best|worst|next>` corresponds to the four algorithms
        - Please use one of the four full words.
    - input parameters are validated on program launch

## Rounding of Float Values
For `%memusage` and `cumulative %mem`, float values are cast as int. This will round down to the nearest integer.

## Input File format
The "process file" will be in the following format:
`[Process id (int)] [memory chunk size (int)]`  
Here is a sample process file:  
> 1 130  
> 2 99  
> 3 200  
> 4 512  
> 5 440  
> 6 320  
> 7 200  
> 8 90  

PLEASE NOTE: there cannot be a new line at the end of the file or else the program will report an error during ingesting. This is by design. The input file should follow the format below

GOOD INPUT FILE 
> 1 100  
> 2 500  
> 3 350     

BAD INPUT FILE 
> 1 100  
> 2 500  
> 3 350  
> (new line)

## Data Structures Used
A linked list was used to create both the waiting queue and the memory usage.

### Waiting Queue
Uses a FIFO linked list to determine which process is next in queue

### Memory Usage
Uses a linked list sorted by placement in the address space to determine where the processes are located  
i.e. [block located at mem addr 0, size 10] -> [block located at mem addr 50, size 50] -> [block located at mem addr 400, size 20]
- When calculating holes, this list is iterated through and the end position of the current node is compared against the start position of the next node to determine if they are side-by-side
- When placing a new process, hole size is calculated using the method above, and the process is inserted into the linked list between the nodes that surround the hole.

## Algorithms
### First
First space large enough for a block of memory.  
### Best
Most ideal space for a block of memory, minimizing leftover space before or after.  
### Worst
Least ideal space for a block of memory, maximizing leftover space before or after.  
### Next
Next available spot after a selected block of memory.  
