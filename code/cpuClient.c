/*////////////////////////////////////////////////////
 /
 /           Andrew Ybarra
 /           1081010
 /           Programming Assignment 3
 /           cpuClient.c
 /
 /           The purpose of this program is to take in
 /           burst size of cpu and IO from the client
 /           and send them to the server. The client
 /           waits to recieve the completion time
 /           and the CPU utilization and prints
 /           both results to screen.
 /
 *////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>


typedef struct values {
    char privateFIFO[14];
    int burst[5];
    int taround;
    int pointer;
    int max;
    int counter;
    float cpu;
} Input;


main (void)
{
    
    Input input;
    
    int i = 0;
    int fdIN; //to write to character server
    int fdOUT; //to read from character server
    int clientID = getpid();
    input.pointer = 0;
    input.counter = 0;
    
    printf("How many CPU/IO burst are there in total(3 or 5): ");
    scanf("%d", &input.max);
    
    for(i = 0; i < input.max; i++)
    {
        if( i%2 == 0)
            printf("Please enter the CPU-burst: " );
        else
            printf("Please enter the I/O-burst: " );
        scanf("%d", &input.burst[i]);
    }
    
    
    sprintf(input.privateFIFO, "FIFO_%d", clientID);
    
    printf("---------------------------------\n");
    printf("Private FIFO name: %s", input.privateFIFO);
    printf("\n---------------------------------\n\n");
    
    if((mkfifo(input.privateFIFO, 0666)<0 && errno != EEXIST))
    {
        perror("Can't create private FIFO\n");
        exit(-1);
    }
    
    
    
    if((fdIN=open("commFIFO", O_WRONLY))<0)  //writting into fifo
        printf("cant open fifo to write");
    
    write(fdIN, &input, sizeof(input));
    
    
    if((fdOUT=open(input.privateFIFO, O_RDONLY))<0) //reading from fifo
        printf("cant open fifo to read");
    
    
    read(fdOUT, &input, sizeof(input));
    
    printf("\nCompletion Time: %d\nCPU Utilization: %.2f\n\n\nCLIENT DONE\n", input.taround, input.cpu);
    
    
    unlink ("commFIFO");
    unlink (input.privateFIFO);
    
    close(fdIN);
    close(fdOUT);
    
}
