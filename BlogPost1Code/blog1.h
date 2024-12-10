#define VULN_H

#include <stdlib.h> // For malloc, free
#include <stdio.h>  // For printf, scanf
#include <string.h> // For strcpy, strcmp

#define BUFFERSIZE 64
#define NUM_OPTIONS 4

// Function prototypes
void populateAndDisplayQuestion(char* question, char** options);
void displayQuestion(char* question, char** options);
int answerQuestion(char** options);