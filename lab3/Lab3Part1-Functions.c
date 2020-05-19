/*
    NAME: Chris Gerbino
    DUE-DATE: 24 April 2020
    TITLE: Lab3 (part 1 & step 1)
    DESCRIPTION: This program copies binary files or text files
        into anther file. This part of the program will use 
        functions.
*/

#include <stdio.h>
#include <stdlib.h>
//specific libs for this lab
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    FILE *readFile, *writeFile; //creating file pointers needed
    //readFile = file w/ data we want to transfer to writeFile

    int fileType = 0;
    printf("Would you like to transfer a text file (1) or a binary file (2)?");
    scanf("%d", &fileType);

    //opening .txt file if user selects '1'
    //opening .jpg file if user selects '2'

    if(fileType == 1){
        readFile = fopen("SherlockTestDoc.txt", "r"); 
        //using 'r' bc this file is a textfile so there is not translation needed
        printf("FLAG");
        if(readFile == NULL){ //checking if file was opened sucessfully
            printf("An error occured while opening this file");
            exit(0);
        }
        //now creating second file for the program to write to
        writeFile = fopen("duplicatedTextFile.txt", "w");
        if(writeFile == NULL){
            printf("\n Error opening writeFile");
            exit(0);
        }

    }else{
        readFile = fopen("TestImage.jpg", "rb"); 
        //using 'rb' bc this file is a binary file 

        if(readFile == NULL){ //checking if file was opened sucessfully
            printf("An error occured while opening this file");
            exit(0);
        }
        //now creating second file for the program to write to
        writeFile = fopen("duplicatedBinaryFile.jpg", "wb"); // using 'wb' bc binary file
        if(writeFile == NULL){
            printf("\n Error opening writeFile");
            exit(0);
        }
    }
    printf("FLAG: PAST FILE SELECTION");

    //Here I need to read & write from/to the files
    //char temp = fgetc(readFile);
    fseek(readFile, 0 , SEEK_END);
    int lengthOfFile = ftell(readFile);
    fseek(readFile, 0, SEEK_SET);
    int i;
    while(i < lengthOfFile){
        fputc(fgetc(readFile), writeFile); //using fgetsc so i can use it as a parameter
        i++;
    }
    //Now checking to make sure it actually hit the end of the file... if it didnt get to 
        //end of file and it returned EOF then an error occured
    // if (feof(readFile)){ 
    //     printf("\n End of file reached."); 
    // }else{ 
    //     printf("\n Error occured.");
    //     exit(0); 
    // }

    printf("File Sucessfully Copied!");
    fclose(readFile);
    fclose(writeFile);


    return 0;
}