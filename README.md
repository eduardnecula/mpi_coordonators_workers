Topic 3 APD

Necula Eduard-Ionut 332CA
Automation and Calculators, CTI
Email: ne_eduard_ionut@yahoo.com


Run theme: mpirun -np 9 ./theme3 12 0 --oversubscribe
    9 - number of workers
    ./theme3 executable after running make
    12 - the size of the vector to be doubled
    0 - if this is the case, I have connections between the coordinators
    (if it was 1 instead of 0, a link was cut)
    --oversubscribe: flag for not commenting on the program
                      when using multiple workers

Theme implementation steps:

                Stage 1 - Topology implementation:

1. Coordinator Processes: 0, 1, 2, will read each from the file.
2. Each coordinating process sends a message to the workers, telling them
who is their coordinator, and the workers receive the messages.
3. Coordinator 0, gives messages to coordinators 1 and 2, and asks for their vector
    of workers, and they are responsible.
4. Coordinator 1, gives messages to coordinators 0 and 2, and asks them for their vector
    of workers, and they are responsible.
5. Coordinator 2, gives messages to coordinators 0 and 1, and asks them for their vector
    of workers, and they are responsible.
6. After each coordinator has received the desired messages, they will display them
acquired topology.
7. After displaying, each coordinator sends to each worker the data that,
he has the data about the other coordinators, but also his own. The workers don't talk
between them, only the coordinators communicate their messages.
8. After a worker receives the topology from each coordinator, displays it.

Remarks:
        After each message sent, a message of the form M (rank, id_sent) is displayed.
        Rank is the id of the coordinator or worker, and id_sent is the id
    the worker / coordinator who will receive the message.
        After each MPI_Send message, an MPI_Recv message will be expected.
        If you do not write the program correctly, it will hang on to an MPI_Recv because of
    experience with the program means that you send too few messages.
        Before sending each vector with the topology, I also sent a message before with
    the number of elements of the vector.
        The whole theme is a collection of (send message - receive message), it is nothing
    abstract by code.

                Stage 2 - Performing Calculations

Implementation steps:

1. Coordinator 0 will create the element vector (line 330 in the main, this is where the stage begins)
2. You will see how many processes the worker has
3. Using start and end, from the first apd lab, divide the number by
items for each worker.
4. Coordinator 0, sends messages to his workers, a message with start, one with end and one with the vector to modify
5. They send back the modified vector.
6. Coordinator 0, sends messages to coordinators 1 and 2, and tells them what the start would be
and the end of each worker (because coordinator 0 knows how many workers each coordinator has).
7. The coordinators send messages to each worker they have with the information
received from coordinator 0.
8. The workers then calculate the results, and send them back to coordinators 1 and 2, each
work sends messages only to the coordinator
9. Coordinators 1 and 2, then send the results received from the workers to coordinator 0
10. Finally to Coordinator 0, all messages will be returned, and then the vector
it will be formatted in a string, so as not to have problems with the display, which sometimes spoils the tests

Remarks:
        At this stage, the large printfs were modified, the messages were then formatted into one
    single string, because otherwise it tells you that you are not sending the correct messages, but in fact
    a printf spoils something, then sticks to the display the checker is checking.
        The length of the vector size is transmitted from coordinator 0, to its workers, then to
    the rest of the coordinators, who also pass on the size of the vector to their workers.