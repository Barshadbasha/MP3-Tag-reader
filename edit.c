#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "types.h"
#include "edit.h"
char str[100];
unsigned int frameSize;
char framedata[100];
char string[100];
int changepos;
status read_validation_edit(char *argv[],Mp3Info *mp3Info)
{
    if(argv[2]!=NULL)
    {
	if(strncmp(argv[2],"-t",2)==0)
	{
	    strncpy(mp3Info->tag_Id,"TIT2",5);
	    printf("INFO: Tag id is stored\n");
	}
	else if(strncmp(argv[2],"-A",2)==0)
	{
	    strncpy(mp3Info->tag_Id,"TPE1",5);
	    printf("INFO: Tag id is stored\n");
	}
	else if(strncmp(argv[2],"-a",2)==0)
	{
	    strncpy(mp3Info->tag_Id,"TALB",5);
	    printf("INFO: Tag id is stored\n");
	}
	else if(strncmp(argv[2],"-y",2)==0)
	{
	    strncpy(mp3Info->tag_Id,"TYER",5);
	    printf("INFO: Tag id is stored\n");

	}
	else if(strncmp(argv[2],"-G",2)==0)
	{
	    strncpy(mp3Info->tag_Id,"TCON",5);
	    printf("INFO: Tag id is stored\n");

	}
	else if(strncmp(argv[2],"-c",2)==0)
	{
	    strncpy(mp3Info->tag_Id,"COMM",5);
	    printf("INFO: Tag id is stored\n");
	}
	else
	{
	    printf("INFO: Edit option is invalid\n");
	    return e_failure;

	    printf("INFO: Tag Id to change:%s.\n",mp3Info->tag_Id);
	}
	if(argv[3]!=NULL)
	{
	    mp3Info->tag_Id_size=strlen(argv[3])+1;
	    strncpy(mp3Info->tag_Id_value,argv[3],mp3Info->tag_Id_size);
	    printf("Tag Id size copied\n");
	}
	if(argv[4]!=NULL)
	{
	    if(strcmp(strstr(argv[4],".mp3"),".mp3")==0)
	    {
		mp3Info->src_file=argv[4];
		printf("INFO: source file added\n");
		strcpy(str,argv[3]);
		return e_success;
	    }
	    else
	    {
		printf("INFO: Please pass .mp3 file\n");
		return e_failure;
	    }
	}
    }
}
status do_edit(Mp3Info *mp3Info)
{
    if(open_file_edit(mp3Info)!=e_success)
    {
	printf("INFO: Failed to open the file to edit\n");
	return e_failure;
    }
    if(tag_type_edit(mp3Info)!=e_success)
    {
	printf("INFO: Tag type is not ID3\n");
	return e_failure;
    }
    if(file_version_edit(mp3Info)!=e_success)
    {
	printf("INFO: File version is not 00 03\n");
	return e_failure;
    }
    if(tag_head_size_edit(mp3Info)!=e_success)
    {
	printf("INFO: File pointer is not shifted.\n");
	return e_failure;
    }
    if(tag_Id_edit(mp3Info,string)!=e_success)
    {
	printf("INFO: Tag type is not T1T2\n");
	return e_failure;
    }
    if(copy_remain_data(mp3Info)!=e_success)
    {
	printf("INFO: Failed to copy\n");
	return e_failure;
    }
    return e_success;

}

status open_file_edit(Mp3Info *mp3Info)
{
    mp3Info->fptr_src=fopen(mp3Info->src_file,"r");
    mp3Info->fptr_temp=fopen("temp.mp3","w");
    printf("INFO: Files are opened successfully\n");
    return e_success;
}
status tag_type_edit(Mp3Info *mp3Info)
{
    char buffer[3];
    fread(buffer,sizeof(char),3,mp3Info->fptr_src);
   // buffer[3]='\0';
    if(strcmp(buffer,"ID3")==0)
    {
	printf("INFO: Tag is ID3\n");
	return e_success;
    }
    return e_failure;
}
status file_version_edit(Mp3Info *mp3Info)
{
    char ver;
    fread(&ver,sizeof(char),1,mp3Info->fptr_src);
    ver+='0';
    printf("INFO: The version is ID3 v2.%c\n",ver);
    return e_success;
}
status tag_head_size_edit(Mp3Info *mp3Info)
{
    fseek(mp3Info->fptr_src,0,SEEK_END);
    int f_size=ftell(mp3Info->fptr_src);
    fseek(mp3Info->fptr_src,10,SEEK_SET);
    printf("tag head size\n");
    return e_success;
}
status tag_Id_edit(Mp3Info *mp3Info,const char *string)
{
    for(int i=0;i<6;i++)
    {
	char buffer[4];
	if(fread(buffer,sizeof(char),4,mp3Info->fptr_src)!=4)
	{
	    return e_failure;
	}
	if(strcmp(buffer,mp3Info->tag_Id)==0)
	{
	    int pos=ftell(mp3Info->fptr_src);
	    unsigned char size[4];
	    fread(size,sizeof(unsigned char),4,mp3Info->fptr_src);
	    int tagSize=(size[0]<<24)|(size[1]<<16)|(size[2]<<8)|size[3];
	    int change_pos=tagSize+4+3+pos;
	    fseek(mp3Info->fptr_src,0,SEEK_SET);
	    char *str=malloc(pos);
	    if(str==NULL)
	    {
		return e_failure;
	    }
	    fread(str,1,pos,mp3Info->fptr_src);
	    fwrite(str,1,pos,mp3Info->fptr_temp);
	    int new_size=mp3Info->tag_Id_size;
	    unsigned char* position=(unsigned char*)&new_size;
	    for(int i=0;i<sizeof(int)/2;i++)
	    {
		unsigned char temp=position[i];
		position[i]=position[sizeof(int)-i-1];
		position[sizeof(int)-i-1]=temp;
	    }
	    fwrite(&new_size,sizeof(new_size),1,mp3Info->fptr_temp);
	    fseek(mp3Info->fptr_temp,3,SEEK_CUR);
	    fwrite(mp3Info->tag_Id_value,1,mp3Info->tag_Id_size-1,mp3Info->fptr_temp);
	    free(str);
	    return e_success;
	}
	tagSize_edit(mp3Info,&frameSize,framedata);
	read_frame_data(mp3Info,&frameSize,framedata);
    
    }
    return e_success;
}
status tagSize_edit(Mp3Info *mp3Info,int *framesize,char *framedata)
{
    unsigned char size[4];
    fread(size,sizeof(char),4,mp3Info->fptr_src);
    *framesize=(size[0]<<24)|(size[1]<<16)|(size[2]<<8)|size[3];
    printf("tagsize_edit\n");
    return e_success;
}
status read_frame_data(Mp3Info *mp3Info,int *framesize,char *framedata)
{
    fseek(mp3Info->fptr_src,3,SEEK_CUR);
    framedata[*framesize];
    fread(framedata,sizeof(char),*framesize-1,mp3Info->fptr_src);
    framedata[*framesize]='\0';
    printf("framedata reading\n");
    return e_success;
}
status copy_remain_data(Mp3Info *mp3Info)
{
    unsigned char buffer[1];
    unsigned int byte_read;
    int changepos=changepos-1;
    fseek(mp3Info->fptr_src,changepos,SEEK_SET);
    while((byte_read=fread(buffer,1,sizeof(buffer),mp3Info->fptr_src))!=0)
    {
	fwrite(buffer,1,byte_read,mp3Info->fptr_temp);
    }
    printf("INFO: Remaining data copied\n");
    return e_success;
}

		    






