

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include"decode.h"

OperationType check_operation_type(char *argv)
{
    if (strcmp(argv, "-e") == 0)
        return e_encode;
    else if (strcmp(argv, "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <-e/-d> <src_img.bmp> <secret.txt> [stego_img.bmp]\n", argv[0]);
        return 1;
    }

    int ret = check_operation_type(argv[1]);
    if (ret == e_encode)
    {
        printf("Do encoding\n");
        EncodeInfo encInfo;
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("Read and validation executed successfully\n");
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Encoding is completed");
            }
        }
        else
        {
            printf("Can't encode\n");
            return 0;
        }
    }
    else if (ret == e_decode)
    {
        printf("Do decoding\n");
        DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("Read and validation executed successfully\n");
            if (do_decoding(&decInfo) == e_success)
            {
                printf("Decoding is completed");
            }
        }
        else
        {
            printf("Can't decode\n");
            return 0;      
          }
    }
    else
    {
        printf("Unsupported\n");
    }
    return 0;
}