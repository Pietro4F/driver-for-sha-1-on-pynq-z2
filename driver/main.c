#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "sha1_lib.h"

int main() {

	int fd;
    int* digested_message;
    char command;
    char file_name[100], str_to_enc[100];

    
    // Open the sha1 driver
    // const int device = sha_1_open();

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
            sha_1_s(0, str_to_enc, digested_message);

            // Close the device file
            //close(device);

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

                // Perform the encryption
                sha_1(0, fd, digested_message);

                // Close the device file
                //close(device);

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

