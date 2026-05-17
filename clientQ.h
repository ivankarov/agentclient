#ifndef CLIENTQ_H
#define CLIENTQ_H
#include <QtNetwork/QLocalSocket>
#include <QLocalSocket>
#include <QLocalServer>
#include <QTextStream>
#include <qdatastream.h>
#include <QFile>


#include <QDataStream>
#include <QThread>
#include <stdlib.h>//                                   ЗАГИТЬ!!
#include <qthread.h>
// #include <peeker.h>
#include <poll.h>
#include <sys/types.h>

#include "clloop.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <csignal>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <QDebug>
#include <functional>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/capability.h>
#include <sys/ptrace.h>
class client : public QObject
{
    Q_OBJECT
public:
    client(QString remoteServername, QObject *parent = nullptr);
    ~client();
    QString Read();
    int clopening();
    void pipeWritting(char* buf);
    int pipe_readReady();
    void addbond();
signals:
    void serverResponse(QString);

public slots:
    void send_StopToServer();
    void pipe_disconnected();
    void pipe_error(QLocalSocket::LocalSocketError);

private:
    // QLocalSocket*   m_socket;
    QString m_message;
    QString m_serverName;
    QFile* pipec;
    int fd;
};


#endif // CLIENTQ_H
