#include <fcntl.h>
void whatInc(){
    int fd = open("/home/ivan/file.txt",O_RDONLY);
    if(fd){
#define FILE = 1
    }
    else{
#define FILE = 0
    }
}
