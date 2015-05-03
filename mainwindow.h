#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "main_form_base.h"
#include <qsocketnotifier.h>
#include <qfile.h>
#include <qfiledialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initial_next();

private:
    Ui::MainWindow *ui;

protected:
    int openSerialPort();
    int openSerialPort_com2();
    int openSerialPort_com4();

protected slots:
    void sendButtonClicked();
    void remoteDataIncoming();
    void remoteDataIncoming_com2();
    void remoteDataIncoming_com4();
    void ConnectButtonClicked();
    void DisconnectButtonClicked();
    void enableLogging(bool);
    void chooseLogFile();
    void serial_wr_func();


protected:
    int m_fd;
    QSocketNotifier *m_notifier;
    int m_fd_com2;
    QSocketNotifier *m_notifier_com2;
    int m_fd_com4;
    QSocketNotifier *m_notifier_com4;
    QFile m_logFile;
    QFileDialog *m_fileDlg;
    QFile m_detectUFile;
    int delayNum;


};

#endif // MAINWINDOW_H
