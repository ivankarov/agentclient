#include "client.cpp"
#include "clloop.cpp"
#include "main.cpp"
#include "mainwindow.cpp"
#include "mythread.cpp"
//инклюднуть сюда моки при их компиляции
const char interp_section[] __attribute__((section(".interp")))  = "/usr/lib/ld-linux-x86-64.so.2";