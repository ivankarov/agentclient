#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <clientQ.h>
#include <clloop.h>
#include <functional>
#include <QDebug>

class mythread: public QThread{
    Q_OBJECT
protected:
    void run() override{
        client *cli = cl;
        lop->loopmemsh([cli](){//                   СПОСОБЫ
            return cli->pipe_readReady();
        },srvpid,cont,&mtx);
    };
public:
    void setcl(client *c,clloop *l,int fdsm,qint64 pid);
    void stop();
    ~mythread(){
        delete lop;
    }
    void setsrvpid(int srvpid);
    int getsrvpid();
private:
    clloop *lop;
    client *cl;
    qint64 srvpid;
    int fdsm;
    std::shared_ptr<int> cont;
    std::mutex mtx;
};

#endif // MYTHREAD_H
