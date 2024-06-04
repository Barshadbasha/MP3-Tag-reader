#include<stdio.h>
#include<string.h>
#include "types.h"
#include "view.h"
#include "edit.h"
OperationType check_operation(char *argv[]);
Mp3Info mp3Info;
int main(int argc,char *argv[])
{
    if(argc<=2)
    {
	error_msg();
	return v_failure;

    }
    else
    {
	int res=check_operation(argv);
	switch(res)
	{
	    case view:
		if(read_validation_view(argv,&mp3Info)==v_success)
		{
		    printf("INFO: Validation successfull\n");
		    do_view(&mp3Info);
		}
		
		else
		{
		    printf("INFO: Validation failed\n");
		    error_msg();
		}
		break;
	    case edit:
		printf("INFO: editing........\n");
		if(argc<5)
		{
		    printf("INFO: Please pass the option,new data to replace,and .mp3 file\n");
		    error_msg();
		    return e_failure;
		}
		else
		{
		    if(read_validation_edit(argv,&mp3Info)==e_success)
		    {
			printf("INFO: Read and validation is successfully executed\n");
			do_edit(&mp3Info);
		    }
		    else
		    {
			printf("INFO: Read and validation failed\n");
			error_msg();
			return e_failure;
		    }
		}
			    
		break;
	    case unsupport:
		printf("INFO: Unsupport operation.....\n");
		error_msg();
		break;
	    case help:
		help_msg();
		break;
	}
    }
}
OperationType check_operation(char *argv[])
{
    if(strcmp(argv[1],"-v")==0)
    {
	printf("INFO: VIEW OPERATION\n");
	return view;
    }
    else if(strcmp(argv[1],"-e")==0)
    {
	printf("edit operation\n");
	return edit;
    }
    else
    {
	printf("INFO: Enter valid option...v for view or e for edit\n");
	return unsupport;
    }
}

