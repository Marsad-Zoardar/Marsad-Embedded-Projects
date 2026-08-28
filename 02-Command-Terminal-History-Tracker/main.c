//Author: Md Marsad Zoardar
//August 27, 2026
/*
 * **********************************The Project: Command History Tracker****************************************************
----------------------------------------------------------------------------------------------------------------------------------

 * Build a program that simulates a simplified command history buffer — like what a shell keeps track of when you press the up-arrow 
 * to see previous commands.
 ***** Core requirements:
 * ->Store a history of commands (strings) the user types, one at a time, in a loop, until they type "exit".

 * ->The history has a starting capacity (say, 5 commands) but must grow dynamically once it fills up — you can't just reject 
 *   new commands once full. When it's full and a new command comes in, resize the storage to hold more (doubling capacity is 
 *   a common, reasonable strategy).
 
 * ->Each stored command must be its own independently allocated string — not a fixed-size char[100] array per slot, but memory
 *   sized to fit whatever the user actually typed (so a 3-character command and a 200-character command shouldn't waste the same 
 *   fixed space).
 
 * ->Track, using a function-persistent counter, the total number of commands ever entered across the program's run — even ones 
 *   that got evicted or overwritten — and print this total when the program exits. (Yes — this is deliberately testing whether 
 *   static inside a function actually stuck with you.)

 * ->Use at least two macros, both genuinely justified by function, not just decorative:
 * --->One for debug/logging output that can be compiled in or out (#ifdef DEBUG style) — think back to why this matters for embedded work.
 
 * --->One that wraps memory allocation with a check — if malloc/realloc ever returns NULL, print an error and exit gracefully rather than 
 *     crashing with a segfault later

 * ->Print the full command history, in order, when the user exits.
 * 
 * ->Free every single allocation before the program ends — every command string, and the array holding them.
 * 
 * ->Run it through valgrind and confirm zero leaks, zero errors — paste me the summary output when you're done, not just "it worked."

 * *******************************Things I'm deliberately NOT teaching you first, that you'll need to research yourself********************
-----------------------------------------------------------------------------------------------------------------------------------------
 
 * ->How to safely store and grow an array of independently-allocated strings (this means an array of pointers — you'll bump into 
 *   "pointer to pointer" naturally here, which is actually Week 3 material; figuring out how far you can reason about it on your
 *    own first is part of the point)
 * 
 * ->How to duplicate a string into freshly allocated memory of the right size (there's a standard function for this — look it up 
 *   rather than hand-rolling it, but understand what it's doing under the hood once you find it)
 * 
 * ->Exactly how realloc behaves — what happens to your old pointer, what you must do with its return value, and what mistake would 
 *   cause a leak or crash if you handle it carelessly

 * */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 5
void commandTerminal();
void clearAllStrings(char **arr, size_t count);
void totalCommands();


//defining the debug macro
#define DEBUG

#ifdef DEBUG
    #define LOG(msg) \
    do{\
        printf("[DEBUG] %s (%s, line %d)\n", msg, __func__, __LINE__);\
    }while(0)
#else
    #define LOG(msg) //expands to nothing
#endif

//defining the memory allocation macro failure detector
#define SAFE_ALLOC(ptr, size)\
    do{\
        (ptr) = malloc(size);\
        if(ptr == NULL){\
            printf("Memory allocation failed!");\
            return;\
        }\
    }while(0)
   
 
int main(int argc, char **argv){
     
     
     commandTerminal();
     
     
     return 0;
}


//This function is used to free the memories after 
void clearAllStrings(char **arr, size_t count){
    for(size_t k = 0; k < count; k++){
        free(arr[k]);
    } 
    free(arr);
}
//Function to count the number of total commands entered
void trackCommandEntry(){
    static int totalCommands = 0;
    totalCommands++;
    printf("Total commands entered so far: %d\n", totalCommands);
}
//This is the function that the main operation of a command terminal
void commandTerminal(){
    
    char **pUserInput; //char --> one character, char* --> one string, char** --> array of strings
    int i = 0; //variable to keep track of the array index

    
    size_t commandCapacity = INITIAL_CAPACITY; //initialize the capacity for total number of bytes for commands
    size_t stringCapacity = INITIAL_CAPACITY; //initialize the capacity for total number of bytes in a string
    size_t count = 0; //variable to count the total number of commands already entered
    
//    pUserInput = malloc(commandCapacity*sizeof(*pUserInput)); //allocated memory for the address of the commands
//    
//    //check if memory allocation for the string array were successful or not
//        if(pUserInput == NULL){
//            printf("Memory allocation failed!");
//            //no need to free(), as it is already NULL
//            return;
//        }

    SAFE_ALLOC(pUserInput, commandCapacity*sizeof(*pUserInput));

    while(1){ //removed the initial for loop to avoid unnecessary loop count
        printf("Enter your command\n> ");
        
        
        pUserInput[i] = malloc(stringCapacity); //allocate memory for the string array
        //Check if malloc were successful or not
            if(pUserInput[i] == NULL){
                printf("Memory allocation failed!");
                clearAllStrings(pUserInput, i);
                return;
            }
        
        size_t length = 0; //variable to check the entered string length in fgets

        while(1){ //This loop is to continue taking input from the user until it finds a end of line and keep allocating memory 
            if(fgets(pUserInput[i] + length, stringCapacity - length, stdin) == NULL){
                printf("String read error!");
                free(pUserInput[i]);
                clearAllStrings(pUserInput, i);
                
                return;
            }
        
                if(strchr(pUserInput[i] + length, '\n') != NULL){ //check if there is a end of line
                    break; //breaks out of the while loop as soon as it finds a line
                }
            length = strlen(pUserInput[i]);
            
            stringCapacity *= 2;//if the input is longer than the INITIAL_CAPACITY, it doubles the string capacity
            char *temp; //temporary memory to realloc
            temp = realloc(pUserInput[i], stringCapacity);
                if(temp == NULL){
                    printf("Memory allocation failed!");
                    //free(pUserInput[i]); //commented out as assigning 2 frees causes serious problem
                    clearAllStrings(pUserInput, i);
                    return;
                }
            pUserInput[i] = temp; //assign the allocated memory from temp to string array
            LOG("String array resized\n");
        }
        stringCapacity = INITIAL_CAPACITY; //resets the memory capacity to avoid unnecessary memory usage
        
        
            pUserInput[i][strcspn(pUserInput[i], "\n")] = '\0'; //
        
                if(strcmp(pUserInput[i], "exit") == 0){
                    free(pUserInput[i]);
                    break;
                }else{
                    trackCommandEntry();
                }
        
        count++;
        i++;
        
        if(count == commandCapacity){
            char **temp2;
            commandCapacity *= 2;
            
            temp2 = realloc(pUserInput, commandCapacity*sizeof(*pUserInput));
            if(temp2 == NULL){
                printf("Memory allocation failed!");
                clearAllStrings(pUserInput, i);
                return;
            }
            
            pUserInput = temp2;
            LOG("Command array resized\n");
 
        }
 
    }
    
    printf("\tYou entered\n");
    printf("---------------------------------\n");
    
    for(size_t j = 0; j < count; j++){
        printf("%s\n", pUserInput[j]);
        free(pUserInput[j]);
        
    }
    
    
    free(pUserInput);
    
} 
