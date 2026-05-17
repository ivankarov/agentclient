#ifndef WHATINC_HPP
#define WHATINC_HPP
#include <fcntl.h>
void whatInc(){
    int fd = open("/home/ivan/file.txt",O_RDONLY);
    if(fd){
#define FILLE = 1
    }
    else{
#define FILLE = 0
    }
}
#endif // WHATINC_HPP
