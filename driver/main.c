/*

This code is licensed under a Creative Commons Attribution-NonCommercial 4.0 International License.
https://creativecommons.org/licenses/by-nc/4.0/

Developed by:
- Pietro Fagnani	Politecnico di Torino, Italy
- Luca D'Elia		Politecnico di Torino, Italy
- Marco Massetti	Politecnico di Torino, Italy
- Jacopo Serra		Politecnico di Torino, Italy
- Massimo Emiliano	Politecnico di Torino, Italy
- Gioele Sineo		Politecnico di Torino, Italy

There is no warranty

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "sha1_lib.h"

void print_result(int *digested_message);

int main() {

	int fd;
    int digested_message[5];
    char command;
    char file_name[100], str_to_enc[100];

    
    // Open the sha1 driver
    const int device = sha_1_open();

    //Print wellcome message
    printf("Enter s/S to execute the SHA1 algorithm on a string, enter f/F to perform the SHA1 algorithm on an file\n");

    // Get the command
    command = tolower(getchar());

    // Execute the command
    switch (command)
    {
    case 's':
        // SHA1 on a string
        printf("Execute SHA1 on a string\n");

        // Get the string
        printf("Enter a string:");
        scanf("%s", str_to_enc);

        if(errorHandler.code >= 0){

            // Perform the encryption
            sha_1_s(device, str_to_enc, digested_message);

            print_result(digested_message);

            // Close the device file
            close(device);

        }

        break;
    case 'f':

        // SHA1 in a file
        printf("Execute SHA1 on a file\n");

        // Get file name
        printf("File name:");
        scanf("%s", file_name);

        // Open the file
        fd = open(file_name, O_RDONLY );
    
        if(fd < 0){
            // Handle error in file opening
            errorHandler.code = ERROR_FILE;
            sprintf(errorHandler.args, file_name);

        } else
        {
            if(errorHandler.code >= 0){

                printf("File correctly opened\n");

                printf("Start SHA1\n");
                // Perform the encryption
                sha_1(device, fd, digested_message);

                print_result(digested_message);

                // Close the device file
                close(device);

            }

            //Close message file
            close(fd);
        }   
        break;
    
    default:
        printf("Error!\n");
        break;
    }

    // Execute error function
    error();

	return 0;
}

void print_result(int *digested_message){

    printf("Result:");
    for(int i = 4; i >= 0; i--){
        printf("%08x ", digested_message[i]);
    }
    printf("\n");
}