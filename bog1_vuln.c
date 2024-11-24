#include "Blog1.h"

#define BUFFERSIZE 64
#define NUM_OPTIONS 4

void printArr(char** arr, int arrLen){
    for (int i = 0; i < arrLen;  i++)
    {
        printf("%s\n", arr[i]);
    }
}

void deleteElement(char*** arr, int index, int* arrLen){
    if (index >= *arrLen){
        fprintf(stderr, "Error: Invalid index given\n");
        exit(EXIT_FAILURE); 
    }

    char *deletedElement = NULL;
    char **temp = (char **)malloc((*arrLen - 1) * sizeof(char *));
    for (int i = 0, j=0; i < *arrLen; i++){
        if (i == index){
            deletedElement = (*arr)[i];
            //Free memory that need to be deleted so that it can allocate
            //to another process.
            free((*arr)[i]);
            
        }
        else
        {
            temp[j] = (*arr)[i];
            j++;
        }
    }
    free(*arr);
    *arr = temp;         // Update the pointer to the new array
    (*arrLen)--;
    //Element is printed after the memory is freed. Unexpected values
    //will be printed
    printf("**Element: %s is deleted**\n", deletedElement);
    
}

int main(){

    int arrLen = 3;
    // start with an array of 3 pointers
    char **arr = (char **)malloc(arrLen* sizeof(char *));
    for (int i = 0; i < arrLen; i++) {
        arr[i] = (char *)malloc(BUFFERSIZE);
    }
    strcpy(arr[0], "one");
    strcpy(arr[1], "two");
    strcpy(arr[2], "three");

    printf("---Old array---\n");
    printArr(arr, arrLen);

    deleteElement(&arr, 1, &arrLen);

    printf("----New array---\n");  
    printArr(arr, arrLen);

    // Free remaining memory
    for (int i = 0; i < arrLen; i++) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}