#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"
#include "view.h"

status read_validation_view(char *argv[],Mp3Info *mp3Info)
{
    if(strstr(argv[2],".mp3")!=NULL && strcmp(strstr(argv[2], ".mp3"),".mp3")==0)
    	
    {
	mp3Info->src_file=argv[2];
	return v_success;
    }
    else
    {
	printf("INFO : Pass MP3 file\n");
	return v_failure;
    }
}
	
status error_msg()
{
    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("USAGE : \n");
    printf("To view to pass like : ./a.out -v mp3filename\n");
    printf("To edit to pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing text mp3filename\n");
    printf("To get help pass like : ./a.out --help\n");
    printf("------------------------------------------------------------------------------------------------------\n");
}
status help_msg()
{
    printf("---------------------------------------------------------------------------------------\n");
    printf("-v -> to view mp3 file contents\n");
    printf("-e -> to edit mp3 file contents\n");
    printf("\t\t-t -> to modifies the Song title\n");
    printf("\t\t-a -> to modifies the file Artist name\n");
    printf("\t\t-A -> to modifies the Album name\n");
    printf("\t\t-y -> to modifies the Year\n");
    printf("\t\t-m -> to modifies the content\n");
    printf("\t\t-c -> to modifies the comment \n");
    printf("---------------------------------------------------------------------------------------\n");
}

status do_view(Mp3Info *mp3Info)
{
    if(open_file(mp3Info) != v_success)
    {
	printf("INFO: Failed to open the file.\n");
	return v_failure;
    }
    if(check_tag(mp3Info) != v_success)
    {
	printf("INFO: ID3 Tag not found.\n");
	return v_failure;
    }
    if(check_version(mp3Info)!=v_success)
    {
	printf("INFO: Version is not 00 03.\n");
	return v_failure;
    }
    if(skip_tag_head(mp3Info)!=v_success)
    {
	printf("INFO: Not skipped.\n");
	return v_failure;
    }
    if(tag_keys(mp3Info)!= v_success)
    {
	printf("INFO: Keys are not inserted.\n");
	return v_failure;
    }
    if(tag(mp3Info)!=v_success)
    {
	printf("INFO: Tag type is not find\n");
	return v_failure;
    }
    if(print_data(mp3Info)!=v_success)
    {
	printf("INFO: Printing not done\n");
	return v_failure;
    }
    
}
status open_file(Mp3Info *mp3Info)
{
    if((mp3Info->fptr_src=fopen(mp3Info->src_file,"r"))==NULL)
    {
	printf("INFO: File cant opening...\n");
	return v_failure;
    }
    printf("INFO: File is opened successfully\n");
    return v_success;
}
status check_tag(Mp3Info *mp3Info)
{
    char buffer[3];
    fread(buffer,sizeof(char),3,mp3Info->fptr_src);
    if(strcmp(buffer,"ID3")==0)
    {
	printf("INFO: ID3 Tag \n");
	return v_success;
    }
    printf("INFO: Tag is not ID3\n");
    return v_failure;
}

status check_version(Mp3Info *mp3Info)
{
    char var;
    fread(&var,sizeof(char),1,mp3Info->fptr_src);
    var +='0';
    printf("INFO: Version is ID3 V2.%c.\n",var);
    return v_success;

}
status skip_tag_head(Mp3Info *mp3Info)
{
    fseek(mp3Info->fptr_src,10,SEEK_SET);
    printf("INFO: Skipped 10 position.\n");
    return v_success;
}
status tag_keys(Mp3Info *mp3Info)
{
    mp3Info->key[0]="TIT2";
    mp3Info->key[1]="TPE1";
    mp3Info->key[2]="TALB";
    mp3Info->key[3]="TYER";
    mp3Info->key[4]="TCON";
    mp3Info->key[5]="COMM";
    printf("INFO: Keys are added.\n");
    return v_success;
}

	    
status tag(Mp3Info *mp3Info)
{
    for(int i=0;i<6;i++)
    {
	unsigned char buffer[4];
	unsigned char size[4];
	fread(buffer,4,sizeof(unsigned char),mp3Info->fptr_src);
	buffer[4]='\0';
	if(strcmp(buffer,mp3Info->key[i])==0)
	{
	    fread(size,sizeof(unsigned char),4,mp3Info->fptr_src);
	    //stored tag size is in big endian change that into little endian
	    int tagsize=(size[0]<<24)|(size[1]<<16)|(size[2]<<8)|size[3];
	    //skipping the flag(3bytes)
	    fseek(mp3Info->fptr_src,3,SEEK_CUR);
	    char *tag=(char *)malloc(tagsize+1);
	    //reading the tag name
	    fread(tag,sizeof(unsigned char),tagsize-1,mp3Info->fptr_src);
	    //adding null character
	    tag[tagsize-1]='\0';
	    //storing the tah name
	    mp3Info->file_data[i]=tag;
	}
    }
    printf("INFO: Tag names are readed\n");
    return v_success;
}
    
status print_data(Mp3Info *mp3Info)
{
    printf("\n\n");
    printf("========================================================\n");
    printf("File\t: %s\n",mp3Info->src_file);
    printf("========================================================\n");
    printf("Title\t: %s\n",mp3Info->file_data[0]);
    printf("Artist\t: %s\n",mp3Info->file_data[1]);
    printf("Album\t: %s\n",mp3Info->file_data[2]);
    printf("Year\t: %s\n",mp3Info->file_data[3]);
    printf("Genere\t: %s\n",mp3Info->file_data[4]);
    printf("Comment\t: %s\n",mp3Info->file_data[5]);
    printf("========================================================\n");
    return v_success;
}
 
 
    
    


