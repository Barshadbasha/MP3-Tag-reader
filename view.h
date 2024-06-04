#ifndef VIEW_H
#define VIEW_H
#include "types.h"


OperationType check_operation(char *argv[]);
status read_validation_view(char *argv[],Mp3Info *mp3Info);
status do_view(Mp3Info *mp3Info);
status open_file(Mp3Info *mp3Info);
status check_tag(Mp3Info *mp3Info);
status check_version(Mp3Info *mp3Info);
status skip_tag_head(Mp3Info *mp3Info);
status tag_keys(Mp3Info *mp3Info);
status tag(Mp3Info *mp3Info);
status print_data(Mp3Info *mp3Info);
status error_msg();
status help_msg();

#endif
