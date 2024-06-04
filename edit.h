#ifndef EDIT_H
#define EDIT_H
#include "types.h"
status read_validation_edit(char *argv[],Mp3Info *mp3Info);
status do_edit(Mp3Info *mp3Info);
status open_file_edit(Mp3Info *mp3Info);
status tag_type_edit(Mp3Info *mp3Info);
status file_version_edit(Mp3Info *mp3Info);
status tag_head_size_edit(Mp3Info *mp3Info);
status tag_Id_edit(Mp3Info *mp3Info,const char *string);
status copy_remain_data(Mp3Info *mp3Info);
status tagSize_edit(Mp3Info *mp3Info,int *framesize,char *framedata);
status read_frame_data(Mp3Info *mp3Info,int *framesize,char *framedata);
status print_tag_data_edit(Mp3Info *mp3Info);


#endif
