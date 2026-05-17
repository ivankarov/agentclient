#include <QApplication>
#include <mainwindow.h>
const char interp_section[] __attribute__((section(".interp")))  = "/usr/lib/ld-linux-x86-64.so.2";
#ifdef __GLIBC__
extern const int _IO_stdin_used;
#ifdef __cplusplus
/* Workaround for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83271 */
extern
#endif /* def __cplusplus */
    const int _IO_stdin_used __attribute__((weak)) = 131073;
#endif /* def __GLIBC__ */

#ifdef __cplusplus
extern "C" {
#endif /* def __cplusplus */

__attribute__((force_align_arg_pointer))
int main(int argc, char *argv[]){
    qDebug() << "Starting application";
    QApplication a(argc, argv);
    MainWindow w;
    qDebug()<<"window created";
    w.show();
    qDebug()<<"window shown, visible: "<< w.isVisible();
    int result = a.exec();
    qDebug()<< "app exited with code: "<<result;
    exit(0);
}
#ifdef __cplusplus
}
#endif /* def __cplusplus */
extern "C"{
    void func();
}