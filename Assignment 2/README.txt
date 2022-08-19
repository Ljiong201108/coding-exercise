shared memory protocol:
An instruction occupies 3 ints: operator, operand1, operand2
A return value occupies 2 ints: status(OK/BAD), return_value

I allocate one page of shared memory(4096 bytes). A queue with a head and a tail pointer is maintained in this page: the position of head is stored in the second last 4 bytes and the position of tail is stored in the last 4 bytes. The size of queue is by default 200. So we can get a following overview of this page:

(number of int as unit, a page of 4096 bytes has 1024 ints)
[0, 600)        200 * 3 bytes of instructions block
[600, 1000)     200 * 2 bytes of return value block
[1022, 1023)    head pointer
[1023, 1024)    tail pointer

Thus, this part of problem is an instantation of producer-comsumer model. I have to use three semaphores to control this progress: ctl(if the queue is blocked), empty(number of empty blocks in the queue), filled(number of filled blocks in the queue). The producer is the client and the consumer is the server. To quit this model safely, I defined a special operator "KILL". Once the thread gets it, it terminates. So I only have to push the same number of KILLs in the queue in order terminate all threads.

For the return value part, I also used semaphore to synchronize the process: only after the client read the return value, the server can then write new return value in it; only after the server write the return value in it, the client can then read it.
