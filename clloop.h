#ifndef CLLOOP_H
#define CLLOOP_H
#include <QObject>
class clloop: public QObject
{
    Q_OBJECT
public:
    clloop();
    void hello();
    clloop* sharedClloop();
    ~clloop();
    int findpid();
    int findcmpfd(char* srvfd,int a);
    void loopsig(std::function<int()> f/*,qint64 pid*/,std::shared_ptr<int> cont,std::mutex *mtx);
    void loopmemmap(std::function<int()> f,/* затычка*/qint64 pid,std::shared_ptr<int> cont,std::mutex *mtx);
    void loopmemsh(std::function<int()> f,/* затычка*/ qint64 pid,std::shared_ptr<int> cont,std::mutex *mtx);
    int fdsm;
private:
    int shmid;
    int * dataptr;
};
#endif // CLLOOP_H
