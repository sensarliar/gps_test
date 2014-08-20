#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
//#include <QWSServer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    //QWSServer::setCursorVisible(false);			// 去掉鼠标

    MainWindow w;
//    w.show();
    w.showFullScreen();
    w.initial_next();
    
    return a.exec();
}
