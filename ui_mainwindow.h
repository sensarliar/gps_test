/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Jun 23 03:28:10 2014
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
    QLabel *label_4;
    QLabel *label_5;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 480);
        QFont font;
        font.setFamily(QString::fromUtf8("\346\226\207\346\263\211\351\251\277\345\276\256\347\261\263\351\273\221"));
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("henben5.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        m_sendEdit = new QTextEdit(centralwidget);
        m_sendEdit->setObjectName(QString::fromUtf8("m_sendEdit"));
        m_sendEdit->setGeometry(QRect(30, 220, 251, 101));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(520, 70, 91, 41));
        QPalette palette;
        QBrush brush(QColor(80, 221, 40, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(159, 158, 158, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_2->setPalette(palette);
        label_2->setFont(font);
        label_2->setText(QString::fromUtf8("<html><head/><body><p>\345\214\227\344\272\254\346\227\266\351\227\264</p></body></html>"));
        m_label_N = new QLabel(centralwidget);
        m_label_N->setObjectName(QString::fromUtf8("m_label_N"));
        m_label_N->setGeometry(QRect(500, 140, 31, 41));
        m_connectButton = new QPushButton(centralwidget);
        m_connectButton->setObjectName(QString::fromUtf8("m_connectButton"));
        m_connectButton->setGeometry(QRect(30, 330, 101, 27));
        m_disconnectButton = new QPushButton(centralwidget);
        m_disconnectButton->setObjectName(QString::fromUtf8("m_disconnectButton"));
        m_disconnectButton->setGeometry(QRect(150, 330, 101, 27));
        m_sendButton = new QPushButton(centralwidget);
        m_sendButton->setObjectName(QString::fromUtf8("m_sendButton"));
        m_sendButton->setGeometry(QRect(80, 380, 101, 27));
        m_enableLoggingCb = new QCheckBox(centralwidget);
        m_enableLoggingCb->setObjectName(QString::fromUtf8("m_enableLoggingCb"));
        m_enableLoggingCb->setGeometry(QRect(270, 380, 61, 21));
        m_logFileLe = new QLineEdit(centralwidget);
        m_logFileLe->setObjectName(QString::fromUtf8("m_logFileLe"));
        m_logFileLe->setGeometry(QRect(330, 370, 151, 31));
        m_logFileFileDialog = new QPushButton(centralwidget);
        m_logFileFileDialog->setObjectName(QString::fromUtf8("m_logFileFileDialog"));
        m_logFileFileDialog->setGeometry(QRect(490, 370, 101, 27));
        m_receiveEdit = new QTextBrowser(centralwidget);
        m_receiveEdit->setObjectName(QString::fromUtf8("m_receiveEdit"));
        m_receiveEdit->setGeometry(QRect(30, 50, 251, 151));
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
        QPalette palette1;
        QBrush brush2(QColor(227, 73, 25, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        m_location_N->setPalette(palette1);
        m_location_N->setNumDigits(10);
        m_location_E = new QLCDNumber(centralwidget);
        m_location_E->setObjectName(QString::fromUtf8("m_location_E"));
        m_location_E->setGeometry(QRect(310, 230, 171, 51));
        QPalette palette2;
        QBrush brush3(QColor(70, 54, 242, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        m_location_E->setPalette(palette2);
        m_location_E->setNumDigits(10);
        m_label_E = new QLabel(centralwidget);
        m_label_E->setObjectName(QString::fromUtf8("m_label_E"));
        m_label_E->setGeometry(QRect(500, 230, 31, 41));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(520, 170, 91, 41));
        QPalette palette3;
        QBrush brush4(QColor(33, 76, 18, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        QBrush brush5(QColor(208, 242, 54, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush5);
        QBrush brush6(QColor(255, 249, 73, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush6);
        QBrush brush7(QColor(255, 255, 0, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush7);
        QBrush brush8(QColor(255, 255, 127, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush8);
        label_3->setPalette(palette3);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(520, 240, 91, 41));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(200, 0, 211, 41));
        label_5->setFont(font);
        label_5->setText(QString::fromUtf8("<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">\345\214\227\346\226\227/GPS\345\256\232\344\275\215\346\216\210\346\227\266\350\256\276\345\244\207</span></p></body></html>"));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\345\214\227\346\226\227/GPS\345\256\232\344\275\215\346\216\210\346\227\266\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
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
        label_3->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ffaa00;\">\347\272\254\345\272\246</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "<html><head/><body><p>\347\273\217\345\272\246</p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
