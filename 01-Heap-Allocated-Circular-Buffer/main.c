//Author: Md Marsad Zoardar
//August 23, 2026
//In this program, we will implement a heap allocated circular buffer using a pointer and not a fixed size array


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int start;
    int len;
    int cap;    //Capacity is now a runtime value, not a fixed macro
    int *data;  //a pointer, not an embedded array
}circular_buffer;

circular_buffer *cb_new(int capacity);
void cb_delete(circular_buffer *c);
void cb_append(circular_buffer *c, int value);
int cb_pop(circular_buffer *c);


int main(){
    circular_buffer *c = cb_new(5); //capacity initialized to 5
    
    cb_append(c, 10);
    cb_append(c, 20);
    cb_append(c, 30);
    
    printf("%d\n", cb_pop(c));
    printf("%d\n", cb_pop(c));
    
    cb_delete(c); //MUST do this - no automatic cleanup like this stack version had
    
    return 0;
}


//Function to decalre struct size and the array size
circular_buffer *cb_new(int capacity){
    
    circular_buffer *c = malloc(sizeof(circular_buffer)); //allocate the struct itself
    c->data = malloc(capacity * sizeof(int));             //allocate the array seperate
    c->cap = capacity;
    c->start = 0;
    c->len = 0;
    
    return c;
    
}

//Function to free the allocated memory for the struct and the array
void cb_delete(circular_buffer *c){
    free(c->data); //free the array first
    free(c);       //Then free the struct itself
}


//Function to append vale to the array
void cb_append(circular_buffer *c, int value){
    int next = (c->start + c->len) % c->cap; //This variable gets the index of the array
    c->data[next] = value;
    c->len++;
}


//Function to pop an element
int cb_pop(circular_buffer *c){
    int value = c->data[c->start];
    c->start = (c->start + 1) % c->cap;
    c->len--;
    
    return value;
}