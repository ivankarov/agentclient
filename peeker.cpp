#include "peeker.h"
peeker::peeker() {

}
QByteArray peeker::dopeek(){
    return file->peek(1);
}
