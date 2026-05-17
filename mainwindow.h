#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <clientQ.h>
#include <QProcess>
#include "mythread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getClient();
private slots:
    void button_clicked();
    void button_Stop_clicked();
    void onDataUpd(QString msg);
    // void getClient();
private:
    client *cl;
    Ui::MainWindow *ui;
    QProcess *srv;
    int fdsm;
    clloop* a;
    mythread *mt;
};
#endif // MAINWINDOW_H
