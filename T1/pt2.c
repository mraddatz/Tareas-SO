#include <stdio.h>
#include <stdlib.h> // For exit() function

int main(int argc, char **argv)
{

    char c[1000];
    FILE *fptr;

    if ((fptr = fopen(argv[1], "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }

    // reads text until newline 
    fscanf(fptr,"%[^\n]", c);

    printf("Data from the file:\n%s", c);
    fscanf(fptr,"%[^\n]", c);

    printf("Data from the file:\n%s", c);
    fclose(fptr);
    
    return 0;
}