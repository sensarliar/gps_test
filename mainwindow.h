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

private:
    Ui::MainWindow *ui;

protected:
    int openSerialPort();

protected slots:
    void sendButtonClicked();
    void remoteDataIncoming();
    void ConnectButtonClicked();
    void DisconnectButtonClicked();
    void enableLogging(bool);
    void chooseLogFile();

protected:
    int m_fd;
    QSocketNotifier *m_notifier;
    QFile m_logFile;
    QFileDialog *m_fileDlg;
    QFile m_detectUFile;
    int delayNum;

};

#endif // MAINWINDOW_H
