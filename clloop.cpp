#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <filesystem>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "clloop.h"
#include "clientQ.h"
clloop* clloop::sharedClloop(){
    shmid = shmget(ftok("/",'e'),1024,0666);
    clloop* cll;
    if(shmid!=-1){
        dataptr = (int*)shmat(shmid,NULL,0); //overwriting!!!
        if(dataptr){
            cll = new (dataptr) clloop();
        }
    }
    return cll;
}
clloop::clloop() {
}
// typedef struct sharedmem{
//     int fd;
//     size_t fsize;
//     unsigned char *dataptr;
// }mem;
namespace fs = std::filesystem;
const std::string proc_path = "/proc";
void clloop::hello(){
    printf("hello\n");
}

int clloop::findpid(){//                       ЕСЛИ НЕ НАХОД, ВЫЗЫВАЕМ ХЕЛПУ С АРГОМ 0 - НУЖЕН ПИД
    cap_t caps = cap_get_proc();//              ПУСТЬ ЭТОТ ЧУВАК НАЧИНАЕТ ПИСАТЬ CLLOOP.H ДОСЛОВНО В PIPE
    perror("cap proc - ");
    cap_value_t cap_list[1] = { CAP_SYS_PTRACE };
    if(cap_set_flag(caps, CAP_EFFECTIVE, 1, cap_list, CAP_SET)!=-1){
        if(cap_set_proc(caps)==-1){
            printf("cap fault\n");
        }
    }
    else{
        printf("cap fault\n");
    }
    cap_free(caps);

    int resultpid = 0;
    std::string strpid;
    std::string fdpath;
    std::string fd;
    std::string link;
    char* chfd;
    char endp[256];
    int pid;
    struct stat st;
    struct stat thisst;
    char path[64];
    sprintf(path, "/proc/%d", getpid());
    uid_t thisuid;
    if(stat(path,&thisst)==0){
        thisuid = thisst.st_uid;
    }
    try {
        if (fs::exists(proc_path) && fs::is_directory(proc_path)) {
            for (const auto& entry : fs::directory_iterator(proc_path)) {
                strpid = entry.path().filename().string();
                if (!strpid.empty() && std::all_of(strpid.begin(), strpid.end(), ::isdigit)) {
                    pid = std::stoi(strpid);
                    if(pid!=getpid()){
                        if(pid == 851){
                            ptrace(PTRACE_ATTACH,pid);                                                  //!!!
                            printf("ptrace attach\n");
                        }
                        fdpath = std::format("/proc/{}",strpid);
                        if(stat(fdpath.c_str(),&st)==0){
                            if(thisuid==st.st_uid){
                                errno = 0;
                                if(kill(pid,SIGCONT)==0){
                                    fdpath = std::format("/proc/{}/fd",strpid);
                                    for (const auto& pidentry : fs::directory_iterator(fdpath)) {
                                        fd = pidentry.path().filename().string();
                                        link = std::format("/proc/{}/fd/{}",strpid,fd);
                                        chfd = link.data();
                                        ssize_t sl = readlink(chfd,endp,sizeof(endp)-1);
                                        if(sl!=-1){
                                            endp[sl] = '\0';
                                            printf("rand pid's fd - %d\n",endp);
                                            if(findcmpfd(endp,0)){
                                                QString str(endp);
                                                printf("CMP! ----------------- %d \n",str);
                                                resultpid = pid;
                                            }
                                        }else{
                                            printf("trying readlink pid with same uid errno - %d\n",errno);
                                        }
                                    }
                                    if(pid==851){
                                        ptrace(PTRACE_DETACH,pid);
                                        printf("ptrace detach\n");
                                    }
                                }


                                //можно отсеять все системные- НЕ серверные процессы

                                // fdpath = std::format("/proc/{}/fd",strpid);//уже не заглушка?
                                // for (const auto& pidentry : fs::directory_iterator(fdpath)) {
                                //     fd = pidentry.path().filename().string();
                                //     link = std::format("/proc/{}/fd/{}",strpid,fd);
                                //     chfd = link.data();
                                //     if(readlink(chfd,endp,sizeof(endp)-1)!=-1){
                                //         if(findcmpfd(endp)){
                                //             QString str(endp);
                                //             printf("CMP! -----------------"<<str;
                                //             resultpid = pid;
                                //         }
                                //     }else{
                                //         printf("trying readlink pid with same uid errno - "<<errno;
                                //     }
                                // }
                            }
                        }
                    }
                }
            }
        }else{
            char* argv[2];
            argv[0] = "2";
            argv[1] = "0";
            // execve("/mnt/disk_d/agentUni/helper/cl/helpa",argv);//               ВЫЗОВ ХЕЛПЫ !!!!!
            std::cerr << "Error: /proc directory not found" << std::endl;
            return -1;
        }
        caps = cap_get_proc();
        if(cap_set_flag(caps, CAP_EFFECTIVE, 1, cap_list, CAP_CLEAR)!=-1){
            if(cap_set_proc(caps)!=-1){
                cap_free(caps);
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
        return -1;
    }
    cap_free(caps);
    return resultpid;
}
//на сервере из клиента не нужно
int clloop::findcmpfd(char* srvfd, int a){
    std::string fdpath;
    std::string fd;
    std::string link;
    char* chfd;
    char lclfd[256];
    std::string thispidstr = std::to_string(getpid());
    try {
        fdpath = std::format("/proc/{}/fd",thispidstr);
        for (const auto& pidentry : fs::directory_iterator(fdpath)) {
            fd = pidentry.path().filename().string();
            link = std::format("/proc/{}/fd/{}",thispidstr,fd);
            chfd = link.data();
            ssize_t lc = readlink(chfd,lclfd,sizeof(lclfd)-1);
            if(lc!=-1){
                lclfd[lc] = '\0';
                if(a){
                    printf("client pid's fds: %d\n",lclfd);
                }
                if(strcmp(lclfd,srvfd)==0&&strncmp(lclfd,"/tmp/my_n_pipe",14)==0){
                    printf("AAAAAAAAAAAAAAAAAAAA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                    return 1;
                }
            }else{
                printf("this fd - %d trying readlink client pid errno - %d",lclfd,errno);;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }
    return 0;
}
void clloop::loopsig(std::function<int()> f/*,qint64 pid*/,std::shared_ptr<int> cont,std::mutex *mtx){
    printf("sig method\n");
    int pid1 = findpid();
    int fcode = 1;
    int x = 0;
    while(fcode){
        {
            std::lock_guard<std::mutex> lock(*mtx);
            x = *cont;
        }
        if(x){
            fcode = f();
            if(kill(pid1,SIGCONT)==0){
                printf("emitted to pid - %d",pid1);
            }
            printf("srv pid - %d",pid1);
            printf("client pid - %d",getpid());
        }
    }
}
void clloop::loopmemmap(std::function<int()> f,/* затычка*/ qint64 pid,std::shared_ptr<int> cont,std::mutex *mtx){
    char d[] = "fd";
    findcmpfd(d,1);
    printf("sig/mmap method\n");
    int pid1 = findpid();
    printf("server pid = %d",pid1);
    int fcode = 1;
    int x = 0;
    if(fdsm){
        struct stat st;
        if(fstat(fdsm,&st)==0){
            size_t fsize = (size_t)st.st_size;
            unsigned char* dataptr =(unsigned char*) mmap(NULL,fsize,PROT_READ|PROT_WRITE,MAP_SHARED,fdsm,0);//ключ общения
            if(dataptr){
                //mem* shm = (mem*)malloc(sizeof(mem));
                //if(shm){
                //shm->fd = fdsm;
                //shm->fsize= fsize;
                while(fcode){
                    {
                        std::lock_guard<std::mutex> lock(*mtx);
                        x= *cont;
                    }
                    if(x){
                        printf("cont = %d\n",x);
                        fcode = f();
                        printf("fcode -%d \n",fcode);
                        if(fcode){
                            if(kill(pid1,SIGCONT)==0){
                                printf("emitted to pid - %d\n",pid1);
                            }
                            printf("srv pid - %d\n",pid1);
                            printf("client pid - %d\n",getpid());
                        }
                        //shm->dataptr=dataptr;
                        int flag = 1;
                        while(flag){
                            if(*((int*)dataptr) ==1/**((int*)shm->dataptr)==1*/){//*
                                printf("mmup() data flag from server - %d \n",*((int*)dataptr)/**((int*)shm->dataptr)*/);
                                flag = 0;
                                // *((int*)shm->dataptr) = 0;
                                *((int*)dataptr) = 0;
                            }
                        }
                        //*((int*)shm->dataptr) = 0;
                    }else{
                        fcode = 0;
                    }
                    //}
                    //free(shm);
                }
                munmap(dataptr,fsize);
            }else{

            }
        }
    }
}
//СПОСОБЫ ВЫБИРАТЬ?
void clloop::loopmemsh(std::function<int()> f,/* затычка*/ qint64 pid,std::shared_ptr<int> cont,std::mutex *mtx/*,int flag*/){
    int pid1 = 0;
    // if (flag){ //                                       ПЕРЕДАТЬ ФУНКЦИОНАЛ РАНЬШЕ РАБОТЫ
        char d[] = "fd";
        findcmpfd(d,1);
        pid1 = findpid();
    // }
    printf("sig/shmget method\n");
    printf("server pid = %d \n",pid1);
    printf("fdsm - %d\n",fdsm);
    int fcode = 1;
    int x = 0;
    if(fdsm){
        int shmid = shmget(ftok("/",'e'),1024,0666);
        if(shmid!=-1){
        int * dataptr = (int*)shmat(shmid,NULL,0);//ключ общения
            if(dataptr){
                /*clloop* cll = new clloop();
                *dataptr = cll->findcmpfd(); //СОСТАВИТЬ СТРУКТУРУ ФУНКЦИОНАЛА
                if(flag){
                    return;
                }*/
                while(fcode){
                    {
                        std::lock_guard<std::mutex> lock(*mtx);
                        x= *cont;
                    }
                    if(x){
                        printf("cont = %d\n",x);
                        fcode = f();
                        printf("fcode - %d \n",fcode);
                        if(fcode){
                            if(kill(pid1,SIGCONT)==0){
                                printf("emitted to pid - %d \n",pid1);
                            }
                            printf("srv pid - %d \n",pid1);
                            printf("client pid - %d \n",getpid());
                        }// если не получено писать по shm 1
                        int flag = 1;
                        while(flag){
                            if(*dataptr ==1){
                                printf("shmget() data flag from server - %d \n",*dataptr);
                                flag = 0;
                                *dataptr = 0;
                            }
                        }
                    }else{
                        fcode = 0;
                    }
                }
                shmdt(dataptr);
            }else{
                printf("shmat fault\n");
            }
            shmctl(shmid,IPC_RMID,NULL);
        }else{
            printf("shmget fault\n");
        }
    }
}

clloop::~clloop(){
    close(fdsm);
}
