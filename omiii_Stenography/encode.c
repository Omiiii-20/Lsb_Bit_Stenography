//Name : Om Bidikar

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")){
        encInfo->src_image_fname=argv[2];
    }
    else{
        printf("Error : .bmp file not found\n");
        return e_failure;
    }
    if (strstr(argv[3], ".txt") || strstr(argv[3], ".py") || strstr(argv[3], ".c") || strstr(argv[3], ".csv"))
    {
        encInfo->secret_fname=argv[3];
        char *extn =  strchr(argv[3], '.');
        if (extn != NULL)
        {
            strcpy(encInfo->extn_secret_file, extn);
        }   
    }
    else{
        printf("Error: secrete  file not found in arguments.\n");
        return e_failure;
    }
    if(argv[4]!=NULL){
        if(strstr(argv[4],".bmp")){
            encInfo->stego_image_fname=argv[4];
        }
        else{
            printf("Error: provide .bmp file\n");
            return e_failure;
        }
    }
    else{
        encInfo->stego_image_fname="stego.bmp";
    }
    return e_success;
    //step1: check if argv[2] is .bmp file or not, if yes got to step2 ,  if not go to  step 3
    //step2: store the str_image in encInfo in encInfo -> src_image_fname   
    //step3: print the error msg and return e_failure

    //step4: check if argv[3] is .txt file. if yes, go to step 5,if not go to step 6
    //step5:store the secret file name in secret_fname and store the extn in extn_sec_file
    //step 6: print the error msg and return e_failure

    //optional
    //step 7:check if argv[4] is passed or not , if yes goto step 8, if no go to step 11
    //step 8: check the file is .bmp or not , if yes goto step 9,if not goto step 10
    //step 9: store the file name in stego_image_fname and return e_success
    // step 10: printf the error msg and return e_failure
    // step 11: print  the msg and store the defalut file name[stego.bmp] in stego_image_fname and return e_success
}
// Main function to drive the encoding (hiding) process
Status do_encoding(EncodeInfo *encInfo)
{
    // Open required files = source image, secret file, stego output
    if(open_files(encInfo)== e_failure){
        printf("Error : file not found\n");
        return e_failure;
    }
    else{
        printf("file open successfully\n");
    }
    //Check if the source image has enough capacity to the secret
    if(check_capacity(encInfo)== e_failure)
    {
        printf("Error: check capacity\n");
        return e_failure;

    }
    else{
        printf("check capacity done successfully\n");
    }
        //Copy BMP header from source image to stego image
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure){
        printf("Error in encoding header file\n");
        return e_failure;
    }
    else{
        printf("Encoding of header file is done\n");
    }
    //a magic string to mark the start of hidden data
    if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure){
        return e_failure;
    }
    else{
        printf("Magic string encoded successfully\n");
    }
    //encode secret file extension
    if(encode_secret_extn_file_size(strlen(encInfo->extn_secret_file), encInfo  )==e_failure){
        printf("Error in encoding file extension size\n");
        return e_failure;
    }
    else{
        printf("file Extention size is encoded successfully\n");
    }
    
    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_failure){
        printf("Error in encoding secret file extension\n");
        return e_failure;
    }
    else{
        printf("Secret file extension encoded successfully\n");
    }
    //Encode the size (in bytes) of the secret file data
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_failure){
        printf("Error in encoding secrete file size\n");
        return e_failure;
    }
    else{
        printf("Secrete file size encoded successfully\n");
    }
    //secret data byte by byte
    if(encode_secret_file_data(encInfo)==e_failure){
        printf("Error in encoding secrete file data\n");
        return e_failure;
    }
    else{
        printf("Secrete file data encoded successfully\n");
    }
        //Copy any remaining image data 
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure){
        //print error msg
        printf("Error in copying remaining bytes\n");
        return e_failure;
    }
    else{
        //print success msg
        printf("remaining byte is copying successfull\n");
        return e_success;
    }
}

uint get_file_size(FILE *fptr)
{
    //step1; move the pos to the end of file
    fseek(fptr,0,SEEK_END);
    //step2; return the pos {fell}.
    return ftell(fptr);
}
Status check_capacity(EncodeInfo *encInfo)
{
    //store image size
    uint image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    //store the size of data present in secrete file
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);

    if(image_capacity > 54 + (strlen(MAGIC_STRING) * 8) + 32 + strlen(encInfo->extn_secret_file) * 8 + 32 + (encInfo->size_secret_file * 8)){
        return e_success;
    }
    else{
        return e_failure;
    }
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    rewind(fptr_src_image);//read from starting
    char buffer[54];
    //reading 54 byte from .bmp file and store into string(buffer)
    fread(buffer,sizeof(char),54,fptr_src_image);
    //writing 54 byte into dest_image from buffer string.
    fwrite(buffer,sizeof(char),54,fptr_stego_image);
    return e_success;
}
Status encode_magic_string( char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;

}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    //1.get the msb bit "x"
    //2.for loop i=0->7
    //3. res1=(unsigned int )(data& 1<<7-i)>>7-i)
    //4.array name image_buffer ->res2= image_buffer[0]&~1                (~1)/(0xFE)
    //5. res=res1|res2
    //6. image_buffer[i]=res;
    for(int i=0;i<=7;i++){
        uint res1=(uint )(data& 1<<(7-i))>>(7-i); //get the msb bit in data
        uint res2 = (uint)image_buffer[i] & (~1) ; // clear the lsb in image_buffer
        image_buffer[i]=res1|res2;
    }
    return e_success;
}
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    //0. run a loop  for 8 times
    //1. read the 8 byte form the src_image
    //2. call function encode_byte_to_lsb(data[i],image_buffer);
    //3. write the 8 byte to the stego_image
    //4. return e_success/
    for(int i=0;i<size;i++){
        fread(image_buffer,sizeof(char),8,fptr_src_image);
        encode_byte_to_lsb(data[i],image_buffer);
        fwrite(image_buffer,sizeof(char),8,fptr_stego_image);
    }
    return e_success;
}
Status encode_size_to_lsb(int data, char *image_buffer)
{
    for(int i=0;i<=31;i++){
        uint res1=(uint )(data& 1<<(31-i))>>(31-i); //get the msb bit in data
        uint res2 = (uint)(image_buffer[i]&~1) ; // clear the lsb in image_buffer
        image_buffer[i]=res1|res2;
    }
    return e_success;
}

Status encode_secret_extn_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    //1.read 32 byte from the src;
    //2. call the size_to_lsb();
    //3.write 32 byte to the  dest
    //return e_success
    fread(image_buffer,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,image_buffer);
    fwrite(image_buffer,sizeof(char),32,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    //1. call a encode_data_to_image(extn,strlen(extn),source file pointer ,dest file pointer)
    encode_data_to_image((char *)file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
     char image_buffer[32];
    fread(image_buffer,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,image_buffer);
    fwrite(image_buffer,sizeof(char),32,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_secret);
   
    char data[encInfo->size_secret_file + 1];
    //read the secrete data and stored into the data[arrar_name];
    
    fread(data,sizeof(char),encInfo->size_secret_file,encInfo->fptr_secret);

    data[encInfo->size_secret_file]='\0';
    encode_data_to_image(data,encInfo->size_secret_file,encInfo->fptr_src_image, encInfo->fptr_stego_image);
    
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src_image,FILE *fptr_stego_image)
{
    char ch;
    while(fread(&ch,1,1,fptr_src_image)>0){
        fwrite(&ch,1,1,fptr_stego_image);
    }
    return e_success;
}