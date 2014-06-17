/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri May 23 08:33:27 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextEdit *m_sendEdit;
    QLabel *label_2;
    QLabel *m_label_N;
    QPushButton *m_connectButton;
    QPushButton *m_disconnectButton;
    QPushButton *m_sendButton;
    QCheckBox *m_enableLoggingCb;
    QLineEdit *m_logFileLe;
    QPushButton *m_logFileFileDialog;
    QTextBrowser *m_receiveEdit;
    QLabel *m_label_debug;
    QLCDNumber *m_time;
    QLCDNumber *m_location_N;
    QLCDNumber *m_location_E;
    QLabel *m_label_E;
    QLabel *label_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 480);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        m_sendEdit = new QTextEdit(centralwidget);
        m_sendEdit->setObjectName(QString::fromUtf8("m_sendEdit"));
        m_sendEdit->setGeometry(QRect(30, 190, 251, 101));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(520, 50, 91, 81));
        m_label_N = new QLabel(centralwidget);
        m_label_N->setObjectName(QString::fromUtf8("m_label_N"));
        m_label_N->setGeometry(QRect(500, 140, 31, 41));
        m_connectButton = new QPushButton(centralwidget);
        m_connectButton->setObjectName(QString::fromUtf8("m_connectButton"));
        m_connectButton->setGeometry(QRect(30, 310, 101, 27));
        m_disconnectButton = new QPushButton(centralwidget);
        m_disconnectButton->setObjectName(QString::fromUtf8("m_disconnectButton"));
        m_disconnectButton->setGeometry(QRect(150, 310, 101, 27));
        m_sendButton = new QPushButton(centralwidget);
        m_sendButton->setObjectName(QString::fromUtf8("m_sendButton"));
        m_sendButton->setGeometry(QRect(80, 360, 101, 27));
        m_enableLoggingCb = new QCheckBox(centralwidget);
        m_enableLoggingCb->setObjectName(QString::fromUtf8("m_enableLoggingCb"));
        m_enableLoggingCb->setGeometry(QRect(270, 360, 61, 21));
        m_logFileLe = new QLineEdit(centralwidget);
        m_logFileLe->setObjectName(QString::fromUtf8("m_logFileLe"));
        m_logFileLe->setGeometry(QRect(330, 350, 151, 31));
        m_logFileFileDialog = new QPushButton(centralwidget);
        m_logFileFileDialog->setObjectName(QString::fromUtf8("m_logFileFileDialog"));
        m_logFileFileDialog->setGeometry(QRect(490, 350, 101, 27));
        m_receiveEdit = new QTextBrowser(centralwidget);
        m_receiveEdit->setObjectName(QString::fromUtf8("m_receiveEdit"));
        m_receiveEdit->setGeometry(QRect(30, 30, 251, 151));
        m_label_debug = new QLabel(centralwidget);
        m_label_debug->setObjectName(QString::fromUtf8("m_label_debug"));
        m_label_debug->setGeometry(QRect(650, 10, 69, 17));
        m_time = new QLCDNumber(centralwidget);
        m_time->setObjectName(QString::fromUtf8("m_time"));
        m_time->setGeometry(QRect(310, 60, 171, 51));
        m_time->setNumDigits(10);
        m_location_N = new QLCDNumber(centralwidget);
        m_location_N->setObjectName(QString::fromUtf8("m_location_N"));
        m_location_N->setGeometry(QRect(310, 140, 171, 51));
        m_location_N->setNumDigits(10);
        m_location_E = new QLCDNumber(centralwidget);
        m_location_E->setObjectName(QString::fromUtf8("m_location_E"));
        m_location_E->setGeometry(QRect(310, 230, 171, 51));
        m_location_E->setNumDigits(10);
        m_label_E = new QLabel(centralwidget);
        m_label_E->setObjectName(QString::fromUtf8("m_label_E"));
        m_label_E->setGeometry(QRect(500, 230, 31, 41));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(520, 170, 91, 81));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Time", 0, QApplication::UnicodeUTF8));
        m_label_N->setText(QString());
        m_connectButton->setText(QApplication::translate("MainWindow", "connect", 0, QApplication::UnicodeUTF8));
        m_disconnectButton->setText(QApplication::translate("MainWindow", "disconnect", 0, QApplication::UnicodeUTF8));
        m_sendButton->setText(QApplication::translate("MainWindow", "send", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_enableLoggingCb->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        m_enableLoggingCb->setText(QApplication::translate("MainWindow", "log", 0, QApplication::UnicodeUTF8));
        m_logFileFileDialog->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        m_label_debug->setText(QString());
        m_label_E->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Location", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
