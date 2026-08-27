//Author: Md Marsad Zoardar
//August 27, 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 5
void commandTerminal();
void clearAllStrings(char **arr, size_t count);

static int totalCommand = 0;

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
        }
        stringCapacity = INITIAL_CAPACITY; //resets the memory capacity to avoid unnecessary memory usage
        
        
            pUserInput[i][strcspn(pUserInput[i], "\n")] = '\0'; //
        
                if(strcmp(pUserInput[i], "exit") == 0){
                    free(pUserInput[i]);
                    break;
                }else{
                    totalCommand += 1;
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
 
        }
 
    }
    
    printf("\tYou entered\n");
    printf("---------------------------------\n");
    
    for(size_t j = 0; j < count; j++){
        printf("%s\n", pUserInput[j]);
        free(pUserInput[j]);
        
    }
    
    printf("Number of total commands entered are: %d \n\n", totalCommand);
    
    
    free(pUserInput);
    
} 
