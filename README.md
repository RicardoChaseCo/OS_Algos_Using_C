# OperatingSystem_C
OS Algos Using C

1.PCB

PCB is used to represent the whole process entity, keyboard control method is used to simulate the event generated in process execution, or mouse or keyboard operation based on graphical interface is used to control process management content. Set up three basic queues: waiting, executing and blocking for simulation. The process management of operating system, the scheduling of simulation process, the creation, execution, blocking, suspending and wakening of users are simulated.

2.FCFS

First come, first served (FCFS) algorithm is to select the process that enters the ready queue first from the ready queue and assign the processor to it to put it into operation. The processor will not be abandoned until the execution is finished or an event causes it to interrupt and block.

3.SPF

Short job priority (SPF) is to select a process with the shortest estimated running time from the ready queue, assign the processor to it, and make it execute until it is completed, or when an event occurs and is blocked, and then reschedule when the processor is abandoned.

4.FIFO

FIFO page replacement algorithm always eliminates the pages that enter the memory first, that is, the pages that stay in memory for the longest time are selected to be eliminated. The algorithm is simple to implement. It only needs to connect the pages that have been transferred into memory into a queue according to the sequence, and set a replacement pointer to make it always point to the oldest page.

5.LRU Page Replacement

LRU page replacement algorithm is based on the usage of the page after being transferred into memory. This algorithm endows each page with an access field to record the time that a page has experienced since it was last visited. When a page needs to be eliminated, the page with the largest time value is selected for elimination.
