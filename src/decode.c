#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
// Function to open the source image file and set the file pointer
Status open_file(DecodeInfo *decInfo)
{
    // Attempt to open the source image file in read mode
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");

    // Check if the file was opened successfully
    if (decInfo->fptr_src_image == NULL)
    {
        // Print the error message associated with the failure
        perror("fopen");

        // Inform the user about the inability to open the specified file
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

        // Return failure status
        return e_failure;
    }

    // Move the file pointer to offset 54 to skip the BMP header
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);

    // Return success status
    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Ensure the source image filename has a .bmp extension
    if (strstr(argv[2], ".bmp")) {
        decInfo->src_image_fname = argv[2];
    } else {
        // Fail fast if file type is incorrect
        printf("Error: .bmp file not found\n");
        return e_failure;
    }

    // If output filename is provided, use it; otherwise use default "decode"
    if (argv[3] != NULL) {
        strcpy(decInfo->secret_fname, argv[3]);
    } else {
        strcpy(decInfo->secret_fname, "decode");
    }
    return e_success;
}

// Main decoding function that orchestrates all decoding steps
Status do_decoding(DecodeInfo *decInfo)
{
    // Step 1: Open the source image file
    if (open_file(decInfo) == e_failure)
    {
        printf("Error: File not found\n");
        return e_failure;
    }
    else
    {
        printf("File opened successfully\n");
    }

    // Step 2: Decode the magic string to verify the presence of hidden data
    if (decode_magic_string(MAGIC_STRING, decInfo) == e_failure)
    {
        printf("Error in decoding magic string\n");
        return e_failure;
    }
    else
    {
        printf("Magic string decoded successfully\n");
    }

    // Step 3: Decode the size of the secret file extension (e.g., ".txt", ".c")
    if (decode_secret_extn_file_size(decInfo) == e_failure)
    {
        printf("Error in decoding secret file extension size\n");
        return e_failure;
    }
    else
    {
        printf("Secret file extension size decoded successfully\n");
    }

    // Step 4: Decode the actual secret file extension
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("Error in decoding secret file extension\n");
        return e_failure;
    }
    else
    {
        printf("Secret file extension decoded successfully\n");
    }

    // Step 5: Decode the size of the secret file content
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("Error in decoding secret file size\n");
        return e_failure;
    }
    else
    {
        printf("Secret file size decoded successfully\n");
    }

    // Step 6: Decode the actual data/content of the secret file
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("Error in decoding secret file\n");
        return e_failure;
    }
    else
    {
        printf("Secret file decoded successfully\n");
        return e_success;
    }
}

Status decode_magic_string(char *magic_string,DecodeInfo *decInfo)
{
    char image_buffer[8];
    char string[3];
    for(int i=0;i<strlen(magic_string);i++){
        fread(image_buffer,sizeof(char),8,decInfo->fptr_src_image);
        string[i]=decode_byte_to_lsb(image_buffer);
        if(string[i]!=magic_string[i]){
            printf("magic string mismatched\n");
            return e_failure;
        }
    }
    string[strlen(magic_string)]='\0';

    //printf("%s\n",string);

return e_success;
}
Status decode_secret_extn_file_size(DecodeInfo *decInfo)
{
    char image_buffer[32];
    fread(image_buffer,sizeof(char),32,decInfo->fptr_src_image);
    decInfo->size_extn = decode_size_to_lsb(image_buffer);
    //printf("%ld\n",decInfo->size_extn);
    return e_success;
}

// Function to decode the secret file extension from the image
Status decode_secret_file_extn(DecodeInfo *decInfo) {
    // Allocate memory for the extension string (+1 for null terminator)
    char string[decInfo->size_extn + 1];
    int i = 0;
    char image_buffer[8];

    // Loop to read and decode each byte of the extension
    for (i = 0; i < decInfo->size_extn; i++) {
        fread(image_buffer, sizeof(char), 8, decInfo->fptr_src_image);
        string[i] = decode_byte_to_lsb(image_buffer);
    }
    string[i] = '\0'; // Null-terminate the string

    // Concatenate the decoded extension to the secret file name
    strcat(decInfo->secret_fname, string); // Ensure secret_fname has enough allocated memory

    return e_success;
}

// Function to decode the size of the secret file from the image
Status decode_secret_file_size(DecodeInfo *decInfo) {
    char image_buffer[32];

    // Read 32 bytes from the source image
    fread(image_buffer, sizeof(char), 32, decInfo->fptr_src_image);

    // Decode the size of the secret file
    decInfo->size_secret_file = decode_size_to_lsb(image_buffer);

    return e_success;
}

// Function to decode the secret file data from the image
Status decode_secret_file_data(DecodeInfo *decInfo) {
    // Allocate buffer for secret data (+1 for null terminator)
    char string[decInfo->size_secret_file + 1];
    char image_buffer[8];

    // Open output (secret) file for writing
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_secret == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
        return e_failure;
    }

    // Read and decode each secret byte one by one
    for (int i = 0; i < decInfo->size_secret_file; i++) {
        // Read 8 bytes from the source image to decode one byte
        fread(image_buffer, sizeof(char), 8, decInfo->fptr_src_image);
        // Extract the hidden byte using LSB method
        string[i] = decode_byte_to_lsb(image_buffer);
    }
    string[decInfo->size_secret_file] = '\0'; // Null-terminate the decoded string

    // Write the decoded string to the secret file (without adding extra newline)
    fprintf(decInfo->fptr_secret, "%s", string);

    // Optionally flush or close the file here
    fclose(decInfo->fptr_secret);

    return e_success;
}

// Decode one byte from 8 LSB bits
char decode_byte_to_lsb(char *image_buffer) {
    char ch = 0;
    for (int i = 0; i < 8; i++) {
        int bit = image_buffer[i] & 1;           // Extract LSB of each scanned byte
        ch |= (bit << (7 - i));                  // Build the byte MSB-first
    }
    return ch;
}

// Decode a 32-bit size from 32 LSB bits (used for secret file size)
int decode_size_to_lsb(char *image_buffer) {
    int size = 0;
    for (int i = 0; i < 32; i++) {
        int bit = image_buffer[i] & 1;           // Extract each bit from image buffer
        size |= (bit << (31 - i));               // Build integer MSB-first
    }
    return size;
}
