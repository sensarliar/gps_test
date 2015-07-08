#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define QT3_SUPPORT
//#include "main_form.h"
//#include <qmultilineedit.h>
#include <qtextedit.h>
//#include <qprogressbar.h>
//#include <qtimer.h>
//#include <qapplication.h>
#include <qmessagebox.h>
//#include <qstringlist.h>

//#include <stdio.h>
#include <unistd.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/ioctl.h>
#include <fcntl.h>
//#include <linux/fs.h>
//#include <errno.h>
//#include <string.h>
#include <termio.h>
#include <qlcdnumber.h>
//#include <cctype>
#include <qlabel.h>
#include <cstring>
#include <stdio.h>

#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

#include <qfileinfo.h>

#include <qdir.h>
#include <qfile.h>

#include <time.h>
#include <sys/time.h>

#include "gps_nmea.h"
//#include "gps.h"
#include <math.h>
#include <stdlib.h>

#include "group_broadcast.h"
#include "calc_rel_pos.h"

extern int utc2bj_time(int utc_hour, struct GpsState* gps_p);

//struct GpsState gps;

class QFileDialog;
/*QPushButton *m_connectButton;
QPushButton *m_disconnectButton;
QPushButton *m_sendButton;
QCheckBox *m_enableLoggingCb;
QPushButton *m_logFileFileDialog;
QLineEdit *m_logFileLe;
QTextEdit *m_sendEdit;
QTextEdit *m_receiveEdit;
*/
time_t t_store;
struct tm* tmnow=NULL,*tmrtc=NULL;
struct timeval st;
bool uart4_message_ok;


QString time_stamp;
QString time_stamp_list;
bool flag_write_ephem;
//int file_name_flag;

//void initial_next();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),m_notifier(0),m_notifier_com2(0),m_notifier_com4(0),m_fileDlg(0)
{
    ui->setupUi(this);
    m_fd=-1;
    m_fd_com2=-1;
    m_fd_com4=-1;
    connect(ui->m_connectButton, SIGNAL(clicked()), this, SLOT(ConnectButtonClicked()));
    connect (ui->m_disconnectButton,SIGNAL(clicked()),this,SLOT(DisconnectButtonClicked()));

    connect(ui->m_sendButton, SIGNAL(clicked()), this, SLOT(sendButtonClicked()));
    //connect (m_logFileFileDialog,SIGNAL(clicked()),this,SLOT(chooseLogFile()));
      connect(ui->m_enableLoggingCb, SIGNAL(toggled(bool)), this, SLOT(enableLogging(bool)));
      connect (ui->m_logFileFileDialog,SIGNAL(clicked()),this,SLOT(chooseLogFile()));
      ui->m_receiveEdit->setWordWrapMode(QTextOption::WrapAnywhere);
      ui->m_receiveEdit->document()->setMaximumBlockCount(500);
      //initial_next();
  /*    QFont font;
      font.setPointSize(28);
      ui->label->setFont(font);
      ui->label->resize(320,50);
*/
      init_group_broadcast();
}

void MainWindow::initial_next(){
      uart4_message_ok=0;
      tmnow=(struct tm *)malloc(sizeof(struct tm));
      tmrtc=(struct tm *)malloc(sizeof(struct tm));

      gps.time_zda_ok=0;
      gps.time_zda=0;
      gps.day_zda=0;
      gps.month_zda=0;
      gps.year_zda=0;

      gps.file_name_flag=0;

      gps.time_ch[0]='1';
      gps.time_ch[1]='6';
      gps.time_ch[2]='8';
      gps.time_ch[3]='8';
      gps.time_ch[4]='8';
      gps.time_ch[5]='8';
      gps.time_ch[6]='.';
      gps.time_ch[7]='8';
      gps.time_ch[8]='8';
      gps.time_ch[9]='\0';

      gps.num_sats = 0;
      gps.num_gps = 0;
      gps.num_beidou = 0;

      gps.speed_E = 0;
      gps.speed_N = 0;
      gps.speed_U = 0;
      gps.speed_H = 0;

      gps.align_pos_av_ch[0]='\0';
      gps.align_pos_av = 0;

      gps.rel_pos_E_ch[0]='1';
      gps.rel_pos_E_ch[1]='0';
      gps.rel_pos_E_ch[2]='0';
      gps.rel_pos_E_ch[3]='0';
      gps.rel_pos_E_ch[4]='0';
      gps.rel_pos_E_ch[5]='\0';


      gps.rel_pos_N_ch[0]='1';
      gps.rel_pos_N_ch[1]='0';
      gps.rel_pos_N_ch[2]='0';
      gps.rel_pos_N_ch[3]='0';
      gps.rel_pos_N_ch[4]='0';
      gps.rel_pos_N_ch[5]='\0';

      gps.rel_pos_U_ch[0]='1';
      gps.rel_pos_U_ch[1]='0';
      gps.rel_pos_U_ch[2]='0';
      gps.rel_pos_U_ch[3]='0';
      gps.rel_pos_U_ch[4]='0';
      gps.rel_pos_U_ch[5]='\0';

      gps.bestvela_parse_ok =0;

      gps.speed_angle = 0;

      gps.rel_ant_pos = {0,0,0};
      gps.rel_ant2plane_pos = gps.rel_ant_pos;
      gps.rel_tail2plane_pos = gps.rel_ant_pos;
      gps.rel_tail2head_pos = gps.rel_ant_pos;
      gps.rel_ant_last_pos = gps.rel_ant_pos;
      gps.rel_speed_enu = gps.rel_ant_pos;
      gps.rel_speed_xyz = gps.rel_ant_pos;

      gps.no_align_count = 40;
      gps.align_flag = 0;

      m_fd_com4 = openSerialPort_com4();
      if (m_fd_com4 < 0) {
          QMessageBox::warning(this, tr("Error"), tr("Fail to open serial port!"));
          return ;
      }
      tcflush(m_fd_com4,TCIOFLUSH);
      m_notifier_com4 = new QSocketNotifier(m_fd_com4, QSocketNotifier::Read, this);
      connect (m_notifier_com4, SIGNAL(activated(int)), this, SLOT(remoteDataIncoming_com4()));


      gps_nmea.msg_len = 0;

      if (m_fd >= 0)
        return;
      m_fd = openSerialPort();
      if (m_fd < 0) {
          QMessageBox::warning(this, tr("Error"), tr("Fail to open serial port!"));
          return ;
      }
      tcflush(m_fd,TCIOFLUSH);
      m_notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
      connect (m_notifier, SIGNAL(activated(int)), this, SLOT(remoteDataIncoming()));


      //sleep(10);
  /*        if(uart4_message_ok)
              break;
              */
        //  usleep(500000);

 /*     if(uart4_message_ok)
      {
          if (m_notifier_com4) {
              delete m_notifier_com4;
              m_notifier_com4 = 0;
          }

          if (m_fd_com4 >= 0) {
              tcflush(m_fd_com4,TCIOFLUSH);
              ::close(m_fd_com4);
              m_fd_com4 = -1;
          }
      }
      */


      gps_impl_init();
      gps_nmea.pos_available = FALSE;

//       gps_nmea.msg_len = 0;
//      ui->m_logFileLe->setText("/media/mmcblk0/cutecom.log");

    delayNum=200;
   // enableLogging(1);
    ConnectButtonClicked();

    flag_write_ephem = 0;


/*
      gettimeofday(&st, NULL);

      time(&t_store);
      tmnow=localtime(&t_store);
*/
/*
      if(uart4_message_ok)
    {
      tmnow->tm_hour=tmrtc->tm_hour;
      tmnow->tm_min=tmrtc->tm_min;
      tmnow->tm_sec=tmrtc->tm_sec;
      t_store=mktime(tmnow);
      stime(&t_store);  //set system time
    }
*/
      //tmnow=localtime(NULL);

/*      time_stamp_list=time_stamp.setNum(tmnow->tm_year+1900);
      time_stamp_list+=("_");
      time_stamp_list+=time_stamp.setNum(tmnow->tm_mon+1);
      time_stamp_list+=("_");
      time_stamp_list+=time_stamp.setNum(tmnow->tm_mday);
      time_stamp_list+=("_");
      time_stamp_list+=time_stamp.setNum(tmnow->tm_hour);
      time_stamp_list+=("_");
      time_stamp_list+=time_stamp.setNum(tmnow->tm_min);
      time_stamp_list+=("_");
      time_stamp_list+=time_stamp.setNum(tmnow->tm_sec);
      time_stamp_list+=("_");
      time_stamp_list+=time_stamp.setNum(st.tv_usec);
      QString file_store_name("/media/sda1/range");
      file_store_name+=time_stamp_list;
      file_store_name+=(".log");
        ui->m_logFileLe->setText(file_store_name);
        */
/*

        enableLogging(0);
        delayNum=200;

      if(m_detectUFile.exists("/dev/sda1")){

                  enableLogging(1);
                  // ui->label_usbNotify->setText(tr("数据记录中！！！"));
                //  ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\346\225\260\346\215\256\346\255\243\345\234\250\350\256\260\345\275\225\344\270\255\357\274\201</p></body></html>", 0, QApplication::UnicodeUTF8));
                  ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">\350\256\260\345\275\225\344\270\255\357\274\201</span></p></body></html>", 0, QApplication::UnicodeUTF8));
                 // ui->label_usbNotify->setStyleSheet(QString::fromUtf8( "background-color: rgba(0,0,0,0);"));
                  ui->label_usbNotify->setStyleSheet(QString::fromUtf8( "background-color: rgb(0, 255, 0);"));
                  usleep(200000);
                  QString text("log com2 rawephema\r\n");
                  ::write(m_fd, text.toLatin1(), text.length());
                  sleep(1);
                  text="log com2 rawephema onnew\r\n";
                  ::write(m_fd, text.toLatin1(), text.length());

      }
      else{

           enableLogging(0);
       //    ui->label_usbNotify->setText(tr("请插入U盘记录数据！！！"));
           ui->label_usbNotify->setStyleSheet(QString::fromUtf8("color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.05 rgba(14, 8, 73, 255), stop:0.36 rgba(28, 17, 145, 255), stop:0.6 rgba(126, 14, 81, 255), stop:0.75 rgba(234, 11, 11, 255), stop:0.79 rgba(244, 70, 5, 255), stop:0.86 rgba(255, 136, 0, 255), stop:0.935 rgba(239, 236, 55, 255));\n"
   "background-color: qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0 rgba(35, 40, 3, 255), stop:0.16 rgba(136, 106, 22, 255), stop:0.225 rgba(166, 140, 41, 255), stop:0.285 rgba(204, 181, 74, 255), stop:0.345 rgba(235, 219, 102, 255), stop:0.415 rgba(245, 236, 112, 255), stop:0.52 rgba(209, 190, 76, 255), stop:0.57 rgba(187, 156, 51, 255), stop:0.635 rgba(168, 142, 42, 255), stop:0.695 rgba(202, 174, 68, 255), stop:0.75 rgba(218, 202, 86, 255), stop:0.815 rgba(208, 187, 73, 255), stop:0.88 rgba(187, 156, 51, 255), stop:0.935 rgba(137, 108, 26, 255), stop:1 rgba(35, 40, 3, 255));"));
        //   ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" color:#ff0000;\">\350\257\267\346\217\222\345\205\245U\347\233\230\350\256\260\345\275\225\346\225\260\346\215\256\357\274\201</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\350\257\267\346\217\222\345\205\245U\347\233\230\357\274\201\357\274\201</p></body></html>", 0, QApplication::UnicodeUTF8));
       }
       */

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConnectButtonClicked()
{

    //connect (TMainForm->TMainFormBase->m_enableLoggingCb,SIGNAL(toggled(bool)),this,SLOT(remoteDataIncoming()));
    //connect (this->m_enableLoggingCb,SIGNAL(toggled(bool)),this,SLOT(remoteDataIncoming()));


    if (m_fd_com2 >= 0)
      return;
    m_fd_com2 = openSerialPort_com2();
    if (m_fd_com2 < 0) {
        QMessageBox::warning(this, tr("Error"), tr("Fail to open serial port!"));
        return ;
    }
    tcflush(m_fd_com2,TCIOFLUSH);
    m_notifier_com2 = new QSocketNotifier(m_fd_com2, QSocketNotifier::Read, this);
    connect (m_notifier_com2, SIGNAL(activated(int)), this, SLOT(remoteDataIncoming_com2()));



}

void MainWindow::DisconnectButtonClicked()
{
    if (m_notifier) {
        delete m_notifier;
        m_notifier = 0;
    }

    if (m_fd >= 0) {
      tcflush(m_fd,TCIOFLUSH);
        ::close(m_fd);
        m_fd = -1;
    }

    if (m_notifier_com2) {
        delete m_notifier_com2;
        m_notifier_com2 = 0;
    }

    if (m_fd_com2 >= 0) {
      tcflush(m_fd_com2,TCIOFLUSH);
        ::close(m_fd_com2);
        m_fd_com2 = -1;
    }

    if (m_notifier_com4) {
        delete m_notifier_com4;
        m_notifier_com4 = 0;
    }

    if (m_fd_com4 >= 0) {
        tcflush(m_fd_com4,TCIOFLUSH);
        ::close(m_fd_com4);
        m_fd_com4 = -1;
    }
}

void MainWindow::sendButtonClicked()
{
  // QString text( m_sendEdit->text() );
 //   QString text("nihao\nls\nroot\n");
    QString text("nihao\n");
    if (text.isEmpty()) {
        return ;
    }
    ::write(m_fd, text.toLatin1(), text.length());
    ui->m_sendEdit->setText("");
}


int MainWindow::openSerialPort()
{
    int fd = -1;

    const char *devName = "/dev/ttyO1";
//    const char *devName = "/dev/ttyUSB0";
    fd = ::open(devName, O_RDWR&~O_NONBLOCK);
    //fd = ::open(devName, O_RDWR|O_NONBLOCK);
    if (fd < 0) {
        return -1;
    }

    termios serialAttr;
    memset(&serialAttr, 0, sizeof serialAttr);
    serialAttr.c_iflag = IGNPAR;
    serialAttr.c_cflag = B115200 | HUPCL | CS8 | CREAD | CLOCAL;
//    serialAttr.c_cc[VMIN] = 273;//144
    serialAttr.c_cc[VMIN] = 255;//144
    if (tcsetattr(fd, TCSANOW, &serialAttr) != 0) {
        return -1;
    }
    //tcflush(fd,TCIOFLUSH);
    return fd;
}




int MainWindow::openSerialPort_com2()
{
    int fd = -1;

    const char *devName = "/dev/ttyO2";
//    const char *devName = "/dev/ttyUSB0";
    fd = ::open(devName, O_RDWR&~O_NONBLOCK);
    //fd = ::open(devName, O_RDWR|O_NONBLOCK);
    if (fd < 0) {
        return -1;
    }

    termios serialAttr;
    memset(&serialAttr, 0, sizeof serialAttr);
    serialAttr.c_iflag = IGNPAR;
//    serialAttr.c_cflag = B115200 | HUPCL | CS8 | CREAD | CLOCAL;
    serialAttr.c_cflag = B921600 | HUPCL | CS8 | CREAD | CLOCAL;
//    serialAttr.c_cc[VMIN] = 273;//144
    serialAttr.c_cc[VMIN] = 255;//144
    if (tcsetattr(fd, TCSANOW, &serialAttr) != 0) {
        return -1;
    }
    //tcflush(fd,TCIOFLUSH);
    return fd;
}

int MainWindow::openSerialPort_com4()
{
    int fd = -1;

    const char *devName = "/dev/ttyO4";
    fd = ::open(devName, O_RDWR&~O_NONBLOCK);
    if (fd < 0) {
        return -1;
    }

    termios serialAttr;
    memset(&serialAttr, 0, sizeof serialAttr);
    serialAttr.c_iflag = IGNPAR;
    serialAttr.c_cflag = B115200 | HUPCL | CS8 | CREAD | CLOCAL;
//    serialAttr.c_cc[VMIN] = 273;//144
    serialAttr.c_cc[VMIN] = 20;
    if (tcsetattr(fd, TCSANOW, &serialAttr) != 0) {
        return -1;
    }
    //tcflush(fd,TCIOFLUSH);
    return fd;
}


void MainWindow::remoteDataIncoming_com2()
{
    char buff_com2[2*(74+48+151)+30];
    int bytesRead=read(m_fd_com2, buff_com2, 2*(74+48+151));
    if (bytesRead<1) {
        QMessageBox::warning(this, tr("Error"), tr("Receive error!"));
        return;
    }

    if (m_logFile.isOpen())
    {
       m_logFile.write(buff_com2, bytesRead);
    }




    if(m_detectUFile.exists("/dev/sda1")&&gps.file_name_flag){
        delayNum--;
        if(delayNum<=25){
            if (!m_logFile.isOpen()){
                enableLogging(1);
                // ui->label_usbNotify->setText(tr("数据记录中！！！"));
              //  ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\346\225\260\346\215\256\346\255\243\345\234\250\350\256\260\345\275\225\344\270\255\357\274\201</p></body></html>", 0, QApplication::UnicodeUTF8));
              //  ui->label_usbNotify->setStyleSheet(QString::fromUtf8( "background-color: rgba(0,0,0,0);"));
                ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">\350\256\260\345\275\225\344\270\255\357\274\201</span></p></body></html>", 0, QApplication::UnicodeUTF8));
               // ui->label_usbNotify->setStyleSheet(QString::fromUtf8( "background-color: rgba(0,0,0,0);"));
                ui->label_usbNotify->setStyleSheet(QString::fromUtf8( "background-color: rgb(0, 255, 0);"));


/*
                QString text("log com2 rawephema\r\nbdsephemerisa");
                ::write(m_fd, text.toLatin1(), text.length());
                */
 /*               usleep(200000);
//               QString text("log com2 rawephema\r\nlog com2 bdsephemerisa once\r\n");
             QString text("log com2 rawephemb\r\n");
 //               QString text("log com2 gpzda ontime 1 \r\n");
             ::write(m_fd, text.toLatin1(), text.length());
             usleep(200000);
//               QString text("log com2 rawephema\r\nlog com2 bdsephemerisa once\r\n");
          text="log com2 bdsephemerisb\r\n";
//               QString text("log com2 gpzda ontime 1 \r\n");
          ::write(m_fd, text.toLatin1(), text.length());
*/                flag_write_ephem = 1;


            }
            if(delayNum<=0){
                if(1==flag_write_ephem){
                usleep(20000);
//                QString text_new("log com2 rawephema onnew\r\nlog com2 bdsephemerisa onchanged\r\n");
/*            QString text_new("log com2 rawephemb ontime 300\r\nlog com2 bdsephemerisb ontime 302\r\n");
                //QString text_new("log com2 gpzda ontime 1 \r\n");
           ::write(m_fd, text_new.toLatin1(), text_new.length());
  */              flag_write_ephem = 0;
                }
                delayNum=200;
            }

         }

    }
    else{
        delayNum=100;
         enableLogging(0);
     //    ui->label_usbNotify->setText(tr("请插入U盘记录数据！！！"));
         ui->label_usbNotify->setStyleSheet(QString::fromUtf8("color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.05 rgba(14, 8, 73, 255), stop:0.36 rgba(28, 17, 145, 255), stop:0.6 rgba(126, 14, 81, 255), stop:0.75 rgba(234, 11, 11, 255), stop:0.79 rgba(244, 70, 5, 255), stop:0.86 rgba(255, 136, 0, 255), stop:0.935 rgba(239, 236, 55, 255));\n"
 "background-color: qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0 rgba(35, 40, 3, 255), stop:0.16 rgba(136, 106, 22, 255), stop:0.225 rgba(166, 140, 41, 255), stop:0.285 rgba(204, 181, 74, 255), stop:0.345 rgba(235, 219, 102, 255), stop:0.415 rgba(245, 236, 112, 255), stop:0.52 rgba(209, 190, 76, 255), stop:0.57 rgba(187, 156, 51, 255), stop:0.635 rgba(168, 142, 42, 255), stop:0.695 rgba(202, 174, 68, 255), stop:0.75 rgba(218, 202, 86, 255), stop:0.815 rgba(208, 187, 73, 255), stop:0.88 rgba(187, 156, 51, 255), stop:0.935 rgba(137, 108, 26, 255), stop:1 rgba(35, 40, 3, 255));"));
    //     ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" color:#ff0000;\">\350\257\267\346\217\222\345\205\245U\347\233\230\350\256\260\345\275\225\346\225\260\346\215\256\357\274\201</span></p></body></html>", 0, QApplication::UnicodeUTF8));
         ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\350\257\267\346\217\222\345\205\245U\347\233\230\357\274\201\357\274\201</p></body></html>", 0, QApplication::UnicodeUTF8));
         if (m_fd_com2 >= 0) {
           tcflush(m_fd_com2,TCIOFLUSH);
         }

     }

}



void MainWindow::remoteDataIncoming_com4()
{
    static bool set_time_flag=0;
    unsigned int fpga_state=0;
    unsigned int fpga_temp;
    unsigned char buff_com4[40];
    int bytesRead=read(m_fd_com4, buff_com4, 20);
    if (bytesRead<1) {
        QMessageBox::warning(this, tr("Error"), tr("Receive error!"));
        return;
    }

    int i;
    int index_f5af=0;
    for(i=0;(i<10)&&(i<bytesRead);i++)
    {
        if((buff_com4[i]==0xF5)&&(buff_com4[i+1]==0xAF))
        {
            if(bytesRead-i>=10)
            {
                uart4_message_ok=1;
                index_f5af=i;
          /*      if(!set_time_flag&&(buff[index_f5af+4]||buff[index_f5af+5]||buff[index_f5af+6])){
                    tmrtc->tm_sec=((buff[index_f5af+4]&0xF0)>>4)*10+(buff[index_f5af+4]&0x0F);
                    tmrtc->tm_min=((buff[index_f5af+5]&0xF0)>>4)*10+(buff[index_f5af+5]&0x0F);
                    tmrtc->tm_hour=((buff[index_f5af+6]&0xF0)>>4)*10+(buff[index_f5af+6]&0x0F);

                    if(uart4_message_ok)
                  {
                    tmnow->tm_hour=tmrtc->tm_hour;
                    tmnow->tm_min=tmrtc->tm_min;
                    tmnow->tm_sec=tmrtc->tm_sec;
                    t_store=mktime(tmnow);
                    stime(&t_store);  //set system time
                    set_time_flag=1;
                  }
                }
                */
                  //  fpga_state=(unsigned int)buff[index_f5af+9];
                    fpga_state=buff_com4[index_f5af+9];
                    if(fpga_state&1)
                    ui->lineEdit_bdc->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
                    else
                    ui->lineEdit_bdc->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
                        fpga_temp=fpga_state&(unsigned int)(0x01<<1);
                    if(fpga_temp)
                    ui->lineEdit_bac->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
                    else
                    ui->lineEdit_bac->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

                    if(fpga_state&1<<2)
                    ui->lineEdit_1588->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
                    else
                    ui->lineEdit_1588->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

                    if(fpga_state&1<<3)
                    ui->lineEdit_ntp->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
                    else
                    ui->lineEdit_ntp->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));


            }
        }
    }





}



void MainWindow::serial_wr_func()
{

    char buff_wr[2048];
    char* buff_wr_p=buff_wr;



buff_wr_p = strcpy(buff_wr,"$GAOMING,");
buff_wr_p += 9;
buff_wr_p = strcpy(buff_wr_p,gps.time_ch);
buff_wr_p += strlen(gps.time_ch);
*buff_wr_p = ',';
buff_wr_p++;
buff_wr_p = strcpy(buff_wr_p,gps.year_zda_ch);
buff_wr_p += strlen(gps.year_zda_ch);
*buff_wr_p = ',';
buff_wr_p++;
buff_wr_p = strcpy(buff_wr_p,gps.month_zda_ch);
buff_wr_p += strlen(gps.month_zda_ch);
*buff_wr_p = ',';
buff_wr_p++;
buff_wr_p = strcpy(buff_wr_p,gps.day_zda_ch);
buff_wr_p += strlen(gps.day_zda_ch);
*buff_wr_p = ',';
buff_wr_p++;
if(gps_nmea.pos_available)
{
    if(gps.align_pos_av){
            *buff_wr_p = '2';
    }else{
            *buff_wr_p = '1';
    }


}else
{
    *buff_wr_p = '0';
}
buff_wr_p++;
*buff_wr_p = ',';
buff_wr_p++;
buff_wr_p = strcpy(buff_wr_p,gps.lat_ch);
buff_wr_p += strlen(gps.lat_ch);
*buff_wr_p = gps.NorS;
buff_wr_p++;
*buff_wr_p = ',';
buff_wr_p++;
buff_wr_p = strcpy(buff_wr_p,gps.lon_ch);
buff_wr_p += strlen(gps.lon_ch);
*buff_wr_p = gps.EorW;
buff_wr_p++;
*buff_wr_p = ',';
buff_wr_p++;
buff_wr_p = strcpy(buff_wr_p,gps.alt_ch);
buff_wr_p += strlen(gps.alt_ch);
*buff_wr_p = ',';
buff_wr_p++;
char speed_buff[20];
gcvt(gps.speed_E,8,speed_buff);
buff_wr_p = strcpy(buff_wr_p,speed_buff);
buff_wr_p += strlen(speed_buff);
*buff_wr_p = ',';
buff_wr_p++;
gcvt(gps.speed_N,8,speed_buff);
buff_wr_p = strcpy(buff_wr_p,speed_buff);
buff_wr_p += strlen(speed_buff);
*buff_wr_p = ',';
buff_wr_p++;
gcvt(gps.speed_U,8,speed_buff);
buff_wr_p = strcpy(buff_wr_p,speed_buff);
buff_wr_p += strlen(speed_buff);
*buff_wr_p = ',';
buff_wr_p++;
/*-------------ant jiayou  to ant shouyou -----------------------*/
buff_wr_p = strcpy(buff_wr_p,gps.rel_pos_E_ch);
buff_wr_p += strlen(gps.rel_pos_E_ch);
*buff_wr_p = ',';
buff_wr_p++;

buff_wr_p = strcpy(buff_wr_p,gps.rel_pos_N_ch);
buff_wr_p += strlen(gps.rel_pos_N_ch);
*buff_wr_p = ',';
buff_wr_p++;

buff_wr_p = strcpy(buff_wr_p,gps.rel_pos_U_ch);
buff_wr_p += strlen(gps.rel_pos_U_ch);
*buff_wr_p = ',';
buff_wr_p++;

/* calc the pos of tail of jiayou to the head of shouyou plane*/
buff_wr_p = strcpy(buff_wr_p,gps.speed_angle_ch);
buff_wr_p += strlen(gps.speed_angle_ch);
*buff_wr_p = ',';
buff_wr_p++;



char rel_pos_buff[20];
gcvt(gps.rel_ant_last_pos.x,8,rel_pos_buff);
buff_wr_p = strcpy(buff_wr_p,rel_pos_buff);
buff_wr_p += strlen(rel_pos_buff);
*buff_wr_p = ',';
buff_wr_p++;

gcvt(gps.rel_ant_last_pos.y,8,rel_pos_buff);
buff_wr_p = strcpy(buff_wr_p,rel_pos_buff);
buff_wr_p += strlen(rel_pos_buff);
*buff_wr_p = ',';
buff_wr_p++;

gcvt(gps.rel_ant_last_pos.z,8,rel_pos_buff);
buff_wr_p = strcpy(buff_wr_p,rel_pos_buff);
buff_wr_p += strlen(rel_pos_buff);
*buff_wr_p = ',';
buff_wr_p++;

/*
calc_enu2xyz_plane_ordinator(&(gps.rel_ant2plane_pos),&(gps.rel_ant_pos));
//calc_xyz_plane_ordinator();
calc_tail2plane_pos();
calc_tail2head_pos();
char rel_pos_buff[20];
gcvt(gps.rel_tail2head_pos.x,8,rel_pos_buff);
buff_wr_p = strcpy(buff_wr_p,rel_pos_buff);
buff_wr_p += strlen(rel_pos_buff);
*buff_wr_p = ',';
buff_wr_p++;

gcvt(gps.rel_tail2head_pos.y,8,rel_pos_buff);
buff_wr_p = strcpy(buff_wr_p,rel_pos_buff);
buff_wr_p += strlen(rel_pos_buff);
*buff_wr_p = ',';
buff_wr_p++;

gcvt(gps.rel_tail2head_pos.z,8,rel_pos_buff);
buff_wr_p = strcpy(buff_wr_p,rel_pos_buff);
buff_wr_p += strlen(rel_pos_buff);
*buff_wr_p = ',';
buff_wr_p++;

calc_rel_speed_method1();
char rel_speed_buff[20];
gcvt(gps.rel_speed_xyz.x,8,rel_speed_buff);
buff_wr_p = strcpy(buff_wr_p,rel_speed_buff);
buff_wr_p += strlen(rel_speed_buff);
*buff_wr_p = ',';
buff_wr_p++;

gcvt(gps.rel_speed_xyz.y,8,rel_speed_buff);
buff_wr_p = strcpy(buff_wr_p,rel_speed_buff);
buff_wr_p += strlen(rel_speed_buff);
*buff_wr_p = ',';
buff_wr_p++;

gcvt(gps.rel_speed_xyz.z,8,rel_speed_buff);
buff_wr_p = strcpy(buff_wr_p,rel_speed_buff);
buff_wr_p += strlen(rel_speed_buff);
*buff_wr_p = ',';
buff_wr_p++;
*/
strcpy(buff_wr_p,"\r\n");

//  int bytesWrite=write(m_fd, buff_wr, (strlen(buff_wr)<1023)? strlen(buff_wr):1023);
// int bytesWrite=write(m_fd, buff_wr, strlen(buff_wr));
//-----------to 422 port-------------------------------
//write(m_fd, buff_wr, strlen(buff_wr));

/*
 int wbuff_len = strlen(buff_wr);
 wbuff_len--;
  while (wbuff_len>=0)
  {
      buff_wr[wbuff_len--]=0;
  }
  //clear all data
*/

 /*------------------clear all buff_wr--------------*/

/*
  int time_length;
  time_length=strlen(gps.time_ch);

 //binary output
  char convert_ch[30];
  int convert_int;
 //  short convert_16bit;
  short convert_16bit;
  int utc_hour_temp;
  buff_wr[0]=0xAA;
  buff_wr[1]=0x55;
  convert_ch[0]=gps.time_ch[0];
  convert_ch[1]=gps.time_ch[1];
  convert_ch[2]='\0';
                                    //hour
  utc_hour_temp = atoi(convert_ch);
  int bj_time_temp;
  bj_time_temp = utc2bj_time(utc_hour_temp,&gps);
  buff_wr[2]=char(bj_time_temp);




  convert_ch[0]=gps.time_ch[2];
  convert_ch[1]=gps.time_ch[3];
  convert_ch[2]='\0';
                                        //minute
  buff_wr[3]=char(atoi(convert_ch));

  if(time_length<9)
  {
      convert_ch[1]=gps.time_ch[4];
      convert_ch[2]='\0';
  }
  else{
      convert_ch[0]=gps.time_ch[4];
      convert_ch[1]=gps.time_ch[5];
      convert_ch[2]='\0';
   }
    buff_wr[4]=char(atoi(convert_ch));//second


    convert_ch[0]=gps.time_ch[time_length-2];
    convert_ch[1]=gps.time_ch[time_length-1];
    convert_ch[2]='\0';
    buff_wr[5]=char(atoi(convert_ch));//mili-second*10

    if(gps_nmea.pos_available)      //Position state
    {
        buff_wr[6]='A';
    }else
    {
        buff_wr[6]='V';
    }

    buff_wr[7]=char(gps.year_zda);
    buff_wr[8]=char(gps.year_zda>>8); //year ,2B

    buff_wr[9]=char(gps.month_zda);//month

    buff_wr[10]=char(gps.day_zda);//day

    convert_ch[0]=gps.lat_ch[0];
    convert_ch[1]=gps.lat_ch[1];  //
    convert_ch[2]=gps.lat_ch[2];
    convert_ch[3]=gps.lat_ch[3];  //
    convert_ch[4]='\0';
    convert_int = atoi(convert_ch);//lat
    buff_wr[11]=char(convert_int);//du*100+min
    buff_wr[12]=char(convert_int>>8);

    convert_ch[0]=gps.lat_ch[5];
    convert_ch[1]=gps.lat_ch[6];  //
    convert_ch[2]=gps.lat_ch[7];
    convert_ch[3]=gps.lat_ch[8];  //
    convert_ch[4]='\0';
    convert_int = atoi(convert_ch);//lat
    buff_wr[13]=char(convert_int);//min(<1)*10000
    buff_wr[14]=char(convert_int>>8);

    convert_ch[0]=gps.lon_ch[0];
    convert_ch[1]=gps.lon_ch[1];  //
    convert_ch[2]=gps.lon_ch[2];
    convert_ch[3]=gps.lon_ch[3];  //
    convert_ch[4]=gps.lon_ch[4];
    convert_ch[5]='\0';
    convert_int = atoi(convert_ch);//lon
    buff_wr[15]=char(convert_int);//du*100+min
    buff_wr[16]=char(convert_int>>8);

    convert_ch[0]=gps.lon_ch[6];
    convert_ch[1]=gps.lon_ch[7];  //
    convert_ch[2]=gps.lon_ch[8];
    convert_ch[3]=gps.lon_ch[9];  //
    convert_ch[4]='\0';
    convert_int = atoi(convert_ch);//lon
    buff_wr[17]=char(convert_int);//min(<1)*10000
    buff_wr[18]=char(convert_int>>8);


    convert_int = (int)(gps.alt*100);//alt*100,4B
    buff_wr[19]=char(convert_int);//
    buff_wr[20]=char(convert_int>>8);
    buff_wr[21]=char(convert_int>>16);
    buff_wr[22]=char(convert_int>>24);

    convert_16bit = (short)(gps.speed_E*10);//speed_E*10,2B with sign
    buff_wr[23]=char(convert_16bit);//
    buff_wr[24]=char(convert_16bit>>8);

    convert_16bit = (short)(gps.speed_N*10);//speed_N*10,2B
    buff_wr[25]=char(convert_16bit);//
    buff_wr[26]=char(convert_16bit>>8);

    convert_16bit = (short)(gps.speed_U*10);//speed_U*10,2B
    buff_wr[27]=char(convert_16bit);//
    buff_wr[28]=char(convert_16bit>>8);

    double gps_total_square=gps.speed_U*gps.speed_U+gps.speed_H*gps.speed_H;
    double gps_total_speed = 0;
    if(gps_total_square>0)
    {
        gps_total_speed =sqrt(gps_total_square);
    }
    else
    {
        gps_total_speed = 0;
    }

    convert_16bit = (short)(gps_total_speed*10);//total speed*10,2B
    buff_wr[29]=char(convert_16bit);//
    buff_wr[30]=char(convert_16bit>>8);

    buff_wr[31]='\r';
    buff_wr[32]='\n';
    buff_wr[33]='\0';

 //  int bytesWrite=write(m_fd, buff_wr, (strlen(buff_wr)<1023)? strlen(buff_wr):1023);
  // bytesWrite = write(m_fd, buff_wr, 33);///do not use strlen
    write(m_fd, buff_wr, 33);///do not use strlen
*/
    int len_buff_wr = strlen(buff_wr);
    int n =0;
 //   n=sendto(soketfd,buff_wr,sizeof(buff_wr),0,(struct sockaddr *)&mcast_addr,sizeof(mcast_addr));
    n=sendto(soketfd,buff_wr,len_buff_wr,0,(struct sockaddr *)&mcast_addr,sizeof(mcast_addr));
    if(n<0)
    {
        perror("sendto()");
       // return -2;
        return;
    }

    if (m_logFile2.isOpen())
    {
       m_logFile2.write(buff_wr, len_buff_wr);
    }

 }






void MainWindow::remoteDataIncoming()
{
//    char buff[2*(66+49+29)+30];
    static char buff[2*(74+48+151+569)+30];
    char* buff_cont_p = buff;
    static int buff_cont_len=0;

    //char buff_array[2*(66+49+29)+30];
    //char *buff=buff_array;
    QString buff_time;
    const QString STR_GGA("NVGGA,");
   //-gm-d const QString STR_GGA("u");
    const char DOUHAO(',');
    //QString test_str("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    //QString s2;
    QString buff_location_N;
    QString buff_location_E;
    //int index_t;
    int index_start;
    int index_end;
    //int time_adjust;
    //unsigned char time_hour;
    int time_hour;
    QString adjust_hour;
    int bytesRead = 0;

//    char* time_stamp;


/*    if (NULL != localtime_r(&st.tv_sec, &tmnow))
    {
            printf(time_stamp,
                    "current time = %04d/%02d/%02d, %02d:%02d:%02d,(microsec: %06d)\n",
                    tmnow.tm_year+1900,tmnow.tm_mday,tmnow.tm_mon+1,
                    tmnow.tm_hour,tmnow.tm_min,tmnow.tm_sec,
                    st.tv_usec
            );
    }
*/
    if((buff_cont_len>74+48+151+569)||(buff_cont_len<0))
    {
        QMessageBox::warning(this, tr("Error"), tr("Receive error2!"));
        buff_cont_len = 0;
        return;
    }
    buff_cont_p=&buff[buff_cont_len];
    if(buff_cont_p==NULL)
    {
        QMessageBox::warning(this, tr("Error"), tr("Receive error1!"));
        return;
    }
        bytesRead=read(m_fd, buff_cont_p, 2*(74+48+151+569)-buff_cont_len);
        if (bytesRead<1) {
            QMessageBox::warning(this, tr("Error"), tr("Receive error3!"));
            return;
        }

    bytesRead += buff_cont_len;
/*
    if (m_logFile.isOpen())
    {
       m_logFile.write(buff, bytesRead);
    }
*/
/*    QString buff_qs(buff);

    ui->m_receiveEdit->append(buff_qs);
    ui->m_receiveEdit->append(QString("\n@@@@@@@@@@@@@@@@@@@@\n"));
*/

/*
    char* src_p_w=buff;


    buff_wr_p = strcpy(buff_wr,"$GAOMING,");
    buff_wr_p += 9;
    buff_wr_p = strcpy(buff_wr_p,gps.time_ch);
    buff_wr_p += strlen(gps.time_ch);
    *buff_wr_p = ',';
    buff_wr_p++;
    buff_wr_p = strcpy(buff_wr_p,gps.day_zda_ch);
    buff_wr_p += strlen(gps.day_zda_ch);
    *buff_wr_p = '%';
    buff_wr_p++;
    if(bytesRead>1800){
        strcpy(buff_wr_p,"out of service!");
        buff_wr_p += strlen("out of service!");

    }else
    {
        int buff_cont_len_temp2=buff_cont_len;

        while(buff_cont_len_temp2>0)
        {
            *buff_wr_p++ = *src_p_w++;
            buff_cont_len_temp2--;
        }

        *buff_wr_p = '%';
        buff_wr_p++;
        int buff_cont_len_temp3=bytesRead-buff_cont_len;
        while(buff_cont_len_temp3>0)
        {
            *buff_wr_p++ = *src_p_w++;
            buff_cont_len_temp3--;
        }
    }

    strcpy(buff_wr_p,"\r\n");

  //  int bytesWrite=write(m_fd, buff_wr, (strlen(buff_wr)<1023)? strlen(buff_wr):1023);
     int bytesWrite=write(m_fd, buff_wr, strlen(buff_wr));

*/

/*
    gettimeofday(&st, NULL);

    time_stamp_list=time_stamp.setNum(tmnow.tm_year+1900);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(tmnow.tm_mon+1);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(tmnow.tm_mday);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(tmnow.tm_hour);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(tmnow.tm_min);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(tmnow.tm_sec);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(st.tv_usec);

    ui->m_receiveEdit->append(time_stamp_list);
  ui->m_receiveEdit->append(QString("\n**********************\n"));
*/
    int count_i=0;

    gps_impl_init();
    int temp_count_i = 0;
    char* dest_p =buff;
    char* src_p = buff;

 while(count_i<bytesRead)
{
     // buff_cont_p = &nmea_parse_char(buff[count_i];
     temp_count_i = count_i;

     src_p = &buff[count_i];
// ui->m_receiveEdit->append(QString("\nstart deal with ..."));
     while (count_i<bytesRead&&!gps_nmea.msg_available)
     { nmea_parse_char(buff[count_i]);
         count_i++;
     }
// ui->m_receiveEdit->append(QString("\nread buff over ..."));
    if (gps_nmea.msg_available) {
      nmea_parse_msg();

      gps_nmea.msg_available = FALSE;
      if(gps.bestvela_parse_ok)
      {
          serial_wr_func();

          gps.bestvela_parse_ok =0;
          if(gps.align_flag == 1)
          {
              gps.align_flag == 0;
              gps.no_align_count = 40;

          }else{
                gps.no_align_count--;
          }

      }

      //buff_cont_len = 0;
    }
    else
      {

        if(temp_count_i>0)
        {
            buff_cont_len = bytesRead - temp_count_i;
           int buff_cont_len_temp = buff_cont_len;
            if(src_p>dest_p)
            {

                   if (*src_p == '\r'||*src_p == '\n') {
                       src_p++;
                       buff_cont_len_temp--;
                       buff_cont_len--;
                   }
                while(buff_cont_len_temp>0)
                {
                    *dest_p++ = *src_p++;
                    buff_cont_len_temp--;
                }


            }
            gps_nmea.msg_len = 0;
        }
        else if (temp_count_i==0)
        {
            buff_cont_len = bytesRead;
            gps_nmea.msg_len = 0;
            return;
        }
        else
        {

            buff_cont_len =0;
            gps_nmea.msg_len = 0;
             return;

        }

    }//else
//  ui->m_receiveEdit->append(QString("\nparse gps over ..."));
  }//while(count_i<bytesRead)



 //char* src_p_w=buff;//error

 if(gps.no_align_count < 2)
 {
     gps.align_pos_av = 0;
 }
/*if there is no align message output for long time, set align pos flag to unlock for rel_pos; ie. the electronic telecom equipment is not working */

//   ui->m_receiveEdit->append(QString("\nfull loop is over ..."));


 //   if((gps.time_ch[time_length-2]=='3'||gps.time_ch[time_length-2]=='8')&&(gps.time_ch[time_length-1]!='0'))
    if(1)
    {
//int test_vl=gps.hmsl;
   QString temp_value;



 //  buff_time(gps.time_ch);
    QChar qc_hour_h,qc_hour_l;
  //  qc_hour_h=QChar(buff_qs[index_start+7]);
   // qc_hour_l=QChar(buff_qs[index_start+6]);
    qc_hour_h=QChar(gps.time_ch[0]);
    qc_hour_l=QChar(gps.time_ch[1]);

   //time_hour=uchar(QChar(buff_qs[index_start+7]))+uchar(QChar(buff_qs[index_start+6]))*10+8;
   time_hour=qc_hour_l.digitValue()+(qc_hour_h.digitValue())*10+8;
   //time_adjust+=int(buff_qs[index_start+6])*10+8;
   time_hour%=24;
   ui->m_time_h->display(temp_value.setNum(time_hour));
   qc_hour_h=QChar(gps.time_ch[2]);
   qc_hour_l=QChar(gps.time_ch[3]);
   temp_value=qc_hour_h;
   temp_value+=qc_hour_l;
  //time_hour=uchar(QChar(buff_qs[index_start+7]))+uchar(QChar(buff_qs[index_start+6]))*10+8;
//  time_hour=qc_hour_l.digitValue()+(qc_hour_h.digitValue())*10;

//   ui->m_time_m->display(temp_value.setNum(time_hour));
     ui->m_time_m->display(temp_value);

     int time_length;
     time_length=strlen(gps.time_ch);

   if(time_length<9)
   {
       qc_hour_h=QChar('0');
       qc_hour_l=QChar(gps.time_ch[4]);
   }
   else{
       qc_hour_h=QChar(gps.time_ch[4]);
       qc_hour_l=QChar(gps.time_ch[5]);
    }
   //   time_hour=qc_hour_l.digitValue()+(qc_hour_h.digitValue())*10;
    temp_value=qc_hour_h;
    temp_value+=qc_hour_l;
   // ui->m_time_s->display(temp_value.setNum(time_hour));
    ui->m_time_s->display(temp_value);
    //temp_value=QChar(gps.time_ch[time_length-3]);
    temp_value=QChar(gps.time_ch[time_length-2]);
    temp_value+=QChar(gps.time_ch[time_length-1]);
//    temp_value+=QChar('0');
    ui->m_time_ms->display(temp_value);

if((gps.time_ch[time_length-2]=='3'||gps.time_ch[time_length-2]=='8')&&(gps.time_ch[time_length-1]!='0'))
{

  //  ui->m_time_ms->display(temp_value.setNum(fmod(gps.tow,1000)));

  //  ui->m_time->display(temp_value.setNum(gps.tow,'g',11));
    ui->m_num_gps->display(gps.num_gps);
    ui->m_num_beidou->display(gps.num_beidou);

   if(gps_nmea.pos_available){
       if(gps.align_pos_av){
        ui->m_label_available->setText(QString("REL_OK"));
        ui->m_label_available->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
       }else{
           ui->m_label_available->setText(QString("SIG_OK"));
           ui->m_label_available->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 180, 0);"));
       }

     //  ui->m_location_N->display(temp_value.setNum(gps.lat,'g',11));
      // ui->m_location_N->display(temp_value((char *)gps.lat_ch));
   //    gps.lat_ch[4]='\0';
       ui->m_location_N_fen->display((char *)(&gps.lat_ch[2]));
//       gps.lat_ch[2]='\0';
       ui->m_location_N_du->display((char *)gps.lat_du_ch);
  //     ui->m_location_N_miao->display(gps.lat);
//       ui->m_location_N->display((char *)gps.lat_ch);
       ui->m_label_N->setText(QString(QChar(gps.NorS)));
     //  ui->m_location_E->display(temp_value.setNum(gps.lon,'g',11));
  //     ui->m_location_E->display((char *)gps.lon_ch);
       ui->m_location_E_fen->display((char *)(&gps.lon_ch[3]));
//       gps.lat_ch[2]='\0';
       ui->m_location_E_du->display((char *)gps.lon_du_ch);
       ui->m_label_E->setText(QString(QChar(gps.EorW)));

     //  ui->m_speed->display(temp_value.setNum(gps.pdop));
    //  ui->m_speed->display((char *)gps.speed_ch);

    //   ui->m_direction->display(temp_value.setNum(gps.num_sv));
 /*      if (gps.gspeed>1){
          ui->m_direction->display((char *)gps.direction_ch);
       }
       else{
           ui->m_direction->display("");
       }
       */
       ui->m_speed->display(gps.gspeed);
       //ui->m_speed->display(gps.num_sats);



       //ui->m_hight->display(temp_value.setNum(gps.alt));
       ui->m_hight->display((char *)gps.alt_ch);

        //ui->m_speed_U->display((char *)gps.speed_U_ch);
        ui->m_speed_U->display(gps.speed_U);
        ui->m_speed_E->display(gps.speed_E);
        ui->m_speed_N->display(gps.speed_N);
    }
   else{
       ui->m_label_available->setText(QString("N/A"));
       ui->m_label_available->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

       ui->m_location_N_du->display("");
       ui->m_location_N_fen->display("");
//       ui->m_location_N_miao->display("");
       ui->m_label_N->setText("");
//       ui->m_location_E->display("");
       ui->m_location_E_du->display("");
       ui->m_location_E_fen->display("");
       ui->m_label_E->setText("");
       ui->m_speed->display("");
//       ui->m_direction->display("");

       ui->m_hight->display("");
       ui->m_speed_U->display("");
       ui->m_speed_E->display("");
       ui->m_speed_N->display("");
   }
   if(1==gps.time_zda_ok)
   {
    time_stamp_list=time_stamp.setNum(gps.year_zda);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(gps.month_zda);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(gps.day_zda);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(gps.time_zda);
    time_stamp_list+=("_");
    time_stamp_list+=time_stamp.setNum(st.tv_usec);
    QString file_store_name("/media/sda1/range");
    file_store_name+=time_stamp_list;
    file_store_name+=(".bin");

    file_store_name2 = "/media/sda1/message" ;
    file_store_name2+=time_stamp_list;
    file_store_name2+=(".txt");

      ui->m_logFileLe->setText(file_store_name);
      gps.file_name_flag=1;
     }
   }

  }//if(gps.time_ch[4]=='1'||gps.time_ch[4]=='6')

//QString disp_hight;
//QString disp_num_sv;
//disp_hight.setNum(temp_value.setNum(gps.hmsl));
//ui->m_label_tt->setText(disp_hight);

 //   disp_num_sv.setNum(gps.num_sv);

 /*
 ui->m_receiveEdit->append(QString("\n"));
  ui->m_receiveEdit->append(disp_hight);
   ui->m_receiveEdit->append(QString("\n"));
  ui->m_receiveEdit->append(disp_num_sv);
  */
/*
  index_start=buff_qs.indexOf(STR_GGA);
    if(((buff_qs.length()-42)>=index_start)&&(index_start>=0)&&(buff_qs.length()>=42)){   //num=15
 //gm-d   if(buff_qs.length()>=1){
      index_end=buff_qs.indexOf(DOUHAO,index_start+6);
      buff_time=buff_qs.mid(index_start+6,index_end-index_start-6);
       QChar qc_hour_h,qc_hour_l;
       qc_hour_h=QChar(buff_qs[index_start+7]);
       qc_hour_l=QChar(buff_qs[index_start+6]);

      //time_hour=uchar(QChar(buff_qs[index_start+7]))+uchar(QChar(buff_qs[index_start+6]))*10+8;
      time_hour=qc_hour_h.digitValue()+(qc_hour_l.digitValue())*10+8;
      //time_adjust+=int(buff_qs[index_start+6])*10+8;
      time_hour%=24;
      adjust_hour.setNum(time_hour);
      buff_time.remove(0,2);
      buff_time.insert(0,adjust_hour);
      //time_hour=time_adjust;
      //buff_time[0]=char(time_adjust/10);
      //buff_time[1]=char(time_adjust%10);
      //buff_time.insert(0,'4');
     // buff_time.insert(0,time_hour);
      //buff_time.insert(0,char(time_adjust%10));
      //buff_time.insert(0,char(time_adjust/10));
        //buff_time.insert(0,time_hour%10);
        //buff_time.insert(0,time_hour/10);
      index_start=index_end+1;
      index_end=buff_qs.indexOf(DOUHAO,index_start);
      buff_location_N=buff_qs.mid(index_start,index_end-index_start);
     // buff_location_N+=char(QChar(buff_qs[index_end+1]));
      ui->m_label_N->setText(QString(QChar(buff_qs[index_end+1])));
      index_start=index_end+3;
      index_end=buff_qs.indexOf(DOUHAO,index_start);
      buff_location_E=buff_qs.mid(index_start,index_end-index_start);
      //buff_location_E+=char(QChar(buff_qs[index_end+1]));
      ui->m_label_E->setText(QString(QChar(buff_qs[index_end+1])));

      ui->m_time->display(buff_time);
      ui->m_location_N->display(buff_location_N);
      ui->m_location_E->display(buff_location_E);


      // m_time->display(index_t);
     //  m_location_N->display(buff_time);
     //  m_location_E->display(int(buff_qs.length()));
      //m_debug->display(time_hour);
    }
    else{
     //  m_time->display(QString("N/A"));
     // m_debug->display(QString("N/A"));
     //  m_location->display(buff_time);
    // m_label_debug->setText(QString("N/A"));
    ui->m_label_debug->setText(QString("N/A"));

    }
*/




    /*  s2=QString(QChar(index_t));
      test_str+=s2;
      s2=QString("\n^^^^^^^^^^^^^\n");
      test_str+=s2;
      s2=QString(QChar(buff_qs.length()));
      test_str+=s2;
      s2=QString("\n^^^^^^@@^^^^^^^\n");
      test_str+=s2;
      m_receiveEdit->insert(test_str);
    */
    //if(isdigit(c))
      //m_time->display(QString(QChar(c)));

}

void MainWindow::enableLogging(bool on)
{
   if (m_logFile.isOpen()==on)
   {
      return;
   }

   if (on)
   {
    //   sleep(2);
      m_logFile.setFileName(ui->m_logFileLe->text());
      //m_logFile.setFileName(m_logFileLe->text());
      //QIODevice::OpenMode mode=QIODevice::ReadWrite;
      //int mode=IO_ReadWrite | IO_Truncate;
      QIODevice::OpenMode mode=QIODevice::WriteOnly;

         mode=mode | QIODevice::Truncate;

       //  mode=mode | QIODevice::Truncate;

       //  mode=mode | QIODevice::Append;

         m_logFile2.setFileName(file_store_name2);


      if (!m_logFile.open(mode))
      {
         QMessageBox::information(this, tr("Opening file failed"), tr("Could not open file %1 for writing").arg(ui->m_logFileLe->text()));
         ui->m_enableLoggingCb->setChecked(false);
      }
      else
      {
       //  m_logAppendCb->setEnabled(false);
         ui->m_logFileLe->setEnabled(false);
         ui->m_logFileFileDialog->setEnabled(false);
      //   saveSettings();
      }

      if (!m_logFile2.open(mode))
      {
         QMessageBox::information(this, tr("Opening file failed"), tr("Could not open file %1 for writing").arg(file_store_name2));
      }

   }
   else
   {
      m_logFile.close();
      m_logFile2.close();

  //    m_logAppendCb->setEnabled(true);
      ui->m_logFileLe->setEnabled(true);
      ui->m_logFileFileDialog->setEnabled(true);

   }

}


void MainWindow::chooseLogFile()
{
  //static QString * workingDirectory = 0;
 // QFileDialog *dlg = new QFileDialog( *workingDirectory, QString::null, parent, name, TRUE );
 // QFileDialog *dlg = new QFileDialog( *workingDirectory, QString::null, this,m_logFileLe->text(), TRUE );
  // QFileDialog qfile_diag(QString("/sdcard"),this);
 //  QString logFile=qfile_diag->getSaveFileName( tr("Save log file ..."),tr("Save log file ..."), this,m_logFileLe->text());
//QString logFile=QFileDialog::getSaveFileName(tr("Save log file ...") ,tr("Save log file ...") , this,m_logFileLe->text(),tr("Save log file ..."));
 //  QString logFile=QFileDialog::getSaveFileName(this, tr("Save log file ..."), m_logFileLe->text());
// QString logFile = QFileDialog::getOpenFileName( QString::null, "*.cpp", this );
//QString logFile = m_fileDlg->getSaveFileName( QString::null, "*.cpp", this );
    QString logFile=QFileDialog::getSaveFileName(this, tr("Save log file ..."), ui->m_logFileLe->text());
    if (!logFile.isEmpty())
    {
       ui->m_logFileLe->setText(logFile);
    }
}
