#ifndef TYPES_H
#define TYPES_H


typedef enum
{
    v_success,
    v_failure,
    e_success,
    e_failure
}status;

typedef enum
{
    view,
    edit,
    unsupport,
    help
}OperationType;

typedef struct mp3Info 
{
    char *src_file;
    FILE *fptr_src;
    char *key[6];
    char *file_data[5];

    char tag_Id[5];
    int tag_Id_size;
    char tag_Id_value[100];
    FILE *fptr_temp;

  
}Mp3Info;

#endif
