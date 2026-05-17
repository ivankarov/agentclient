#ifndef PEEKER_H
#define PEEKER_H
#include <QObject>
#include <QFile>
class peeker: public QObject
{
public:
    peeker();
    QByteArray dopeek();
};

#endif // PEEKER_H
