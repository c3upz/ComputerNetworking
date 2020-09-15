/*
    NAME: Chris Gerbino
    DUE-DATE: 24 April 2020
    TITLE: Lab3 (part 1 & step 2)
    DESCRIPTION: This program copies binary files or text files
        into anther file. This part of the program will use 
        system calls.
*/
#include <stdio.h>
#include <stdlib.h>
//specific libs for this lab
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int readFile, writeFile;

    int fileType = 0;
    printf("Would you like to transfer a text file (1) or a binary file (2)?");
    scanf("%d", &fileType);

    //opening .txt file if user selects '1'
    //opening .jpg file if user selects '2'

    if (fileType == 1)
    {
        readFile = open("SherlockTestDoc.txt", O_RDONLY);
        printf("\nFLAG");
        if (readFile == -1)
        { //checking if file was opened sucessfully
            printf("An error occured while opening this file");
            exit(0);
        }
        //now creating second file for the program to write to
        writeFile = open("duplicatedTextFile2.txt", O_CREAT | O_WRONLY, 0666);
        if (writeFile == -1)
        {
            printf("\n Error opening writeFile");
            exit(0);
        }
    }
    else
    {
        readFile = open("TestImage.jpg", O_RDONLY);
        //using 'rb' bc this file is a binary file

        if (readFile == -1)
        { //checking if file was opened sucessfully
            printf("An error occured while opening this file");
            exit(0);
        }
        //now creating second file for the program to write to
        writeFile = open("duplicatedBinaryFile2.jpg", O_CREAT | O_WRONLY, 0666); // using 'wb' bc binary file
        if (writeFile == -1)
        {
            printf("\n Error opening writeFile");
            exit(0);
        }
    }

    //now reading the file until EOF
    char buffer;
    int temp;
    while (read(readFile, &buffer, 1) > 0)
    {
        write(writeFile, &buffer, 1);
        //I thought i would need the code below to check if there was an error... but it was the cause of my error

        // temp = read(writeFile, &buffer, 1);
        // if(temp == -1){
        //     printf("\nAn error has occured while writing to writeFile");
        //     exit(0);
        // }
    }

    printf("\nFile should be copied");
    close(writeFile);
    close(readFile);

    return 0;
}