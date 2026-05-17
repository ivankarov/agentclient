#include "mainwindow.h"
#include <mythread.h>
#include "ui_mainwindow.h"
#include "clientQ.h"
#include <QProcess>
#include <QDebug>
#include <unistd.h>
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    getClient();
    // /*УСТАРЕЛО!!*/srv = new QProcess(this);
    //     //srv->setWorkingDirectory();
    // connect(this->srv, &QProcess::started,this,&MainWindow::getClient);
    // srv->start("/mnt/disk_d/agentUni/agent");
    // qDebug()<<srv->state();
}
void MainWindow::getClient(){
    cl = new client("/tmp/my_n_pipe",this);
    connect(cl,&client::serverResponse,this,&MainWindow::onDataUpd);
    connect(this->ui->pushButton,SIGNAL(clicked()),this,SLOT(button_clicked()));
    connect(this->ui->pushButtonStop,SIGNAL(clicked()),this,SLOT(button_Stop_clicked()));
    fdsm= cl->clopening();
    if (fdsm<0){
        return ;
    }
    qDebug()<<"fdsm - "<<fdsm;
    a = new clloop();
    clloop* l = a->sharedClloop();
    client* cli = cl;
    cli->addbond();
    mt = new mythread();
    mt->setcl(cli,a,fdsm,2/*srv->processId()*/);
    mt->start();
}
MainWindow::~MainWindow(){
    if (mt) {
        mt->stop();
        mt->wait();
        delete mt;
    }
    delete ui;
}
void MainWindow::onDataUpd(QString msg){
    qDebug() << "from on data upd";
    qDebug()<<msg;
}

void MainWindow::button_clicked(){
    qDebug()<<"from button_clckd";
    ui->TextEdit->appendPlainText(cl->Read()+QString("\n"));
}
void MainWindow::button_Stop_clicked(){
    cl->send_StopToServer();
}

