#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5

typedef struct _decodeInfo
{
    /* Source Image info */
    char *src_image_fname;   //store the name of source image
    FILE *fptr_src_image;   //address of file 
    // uint image_capacity;
    // uint bits_per_pixel;
    // char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char secret_fname[50];  //store the name of secret txt file
    FILE *fptr_secret;  //store the address of secret txt file
    char extn_secret_file[MAX_FILE_SUFFIX];  //store the extension of secret txt file//.txt/.c/.sh
    // char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
    long size_extn; //size of secrete file


} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv);//comp

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);//comp

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);//comp

/* Get File pointers for i/p and o/p files */
Status open_file(DecodeInfo *decInfo);//comp

/* Store Magic String */
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo);//comp

/* decode secret  file extension size */
Status decode_secret_extn_file_size( DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode function, which does the real encoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* decode a byte into LSB of image data array */
char decode_byte_to_lsb(char *image_buffer);

/* decode a size into LSB of image data array */
int decode_size_to_lsb(char *image_buffer);


#endif