#include "mythread.h"
#include <QDebug>
#include "clloop.h"
#include <iostream>
void mythread::setcl(client *c,clloop *l,int fdsm,qint64 pid){
    this->cl= c;
    this->lop = l;
    this->fdsm = fdsm;
    this->srvpid = pid;
    cont = std::make_shared<int>(1);
    l->fdsm = fdsm;
};
void mythread::stop(){
    std::lock_guard<std::mutex> lock(mtx);
    *cont = 0;
}
void mythread::setsrvpid(int srvpid){
    std::cout<<"setsrvpid";
    // this->srvpid = srvpid;
}
int mythread::getsrvpid(){
    return srvpid;
}