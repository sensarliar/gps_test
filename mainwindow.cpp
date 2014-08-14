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
struct tm* tmnow;
struct timeval st;

QString time_stamp;
QString time_stamp_list;
bool flag_write_ephem;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),m_notifier(0),m_notifier_com2(0),m_fileDlg(0)
{
    ui->setupUi(this);
    m_fd=-1;
    m_fd_com2=-1;
    connect(ui->m_connectButton, SIGNAL(clicked()), this, SLOT(ConnectButtonClicked()));
    connect (ui->m_disconnectButton,SIGNAL(clicked()),this,SLOT(DisconnectButtonClicked()));

    connect(ui->m_sendButton, SIGNAL(clicked()), this, SLOT(sendButtonClicked()));
    //connect (m_logFileFileDialog,SIGNAL(clicked()),this,SLOT(chooseLogFile()));
      connect(ui->m_enableLoggingCb, SIGNAL(toggled(bool)), this, SLOT(enableLogging(bool)));
      connect (ui->m_logFileFileDialog,SIGNAL(clicked()),this,SLOT(chooseLogFile()));
      ui->m_receiveEdit->setWordWrapMode(QTextOption::WrapAnywhere);
      ui->m_receiveEdit->document()->setMaximumBlockCount(500);


      gettimeofday(&st, NULL);
      time_t t_store;
      time(&t_store);
      tmnow=localtime(&t_store);
      //tmnow=localtime(NULL);

      time_stamp_list=time_stamp.setNum(tmnow->tm_year+1900);
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
      QString file_store_name("/media/sda1/cutecom");
      file_store_name+=time_stamp_list;
      file_store_name+=(".log");
        ui->m_logFileLe->setText(file_store_name);

        gps_impl_init();
        gps_nmea.pos_available = FALSE;

//       gps_nmea.msg_len = 0;
//      ui->m_logFileLe->setText("/media/mmcblk0/cutecom.log");
      enableLogging(0);
      delayNum=200;
     // enableLogging(1);
      ConnectButtonClicked();

      flag_write_ephem = 0;

      if(m_detectUFile.exists("/dev/sda1")){

                  enableLogging(1);
                  // ui->label_usbNotify->setText(tr("数据记录中！！！"));
                  ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\346\225\260\346\215\256\346\255\243\345\234\250\350\256\260\345\275\225\344\270\255\357\274\201</p></body></html>", 0, QApplication::UnicodeUTF8));
                  ui->label_usbNotify->setStyleSheet(QString::fromUtf8( "background-color: rgba(0,0,0,0);"));
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
           ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" color:#ff0000;\">\350\257\267\346\217\222\345\205\245U\347\233\230\350\256\260\345\275\225\346\225\260\346\215\256\357\274\201</span></p></body></html>", 0, QApplication::UnicodeUTF8));

       }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConnectButtonClicked()
{
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

    const char *devName = "/dev/ttyO4";
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



void MainWindow::remoteDataIncoming_com2()
{
    char buff[2*(74+48+151)+30];
    int bytesRead=read(m_fd_com2, buff, 2*(74+48+151));
    if (bytesRead<1) {
        QMessageBox::warning(this, tr("Error"), tr("Receive error!"));
        return;
    }

    if (m_logFile.isOpen())
    {
       m_logFile.write(buff, bytesRead);
    }




    if(m_detectUFile.exists("/dev/sda1")){
        delayNum--;
        if(delayNum<=30){
            if (!m_logFile.isOpen()){
                enableLogging(1);
                // ui->label_usbNotify->setText(tr("数据记录中！！！"));
                ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\346\225\260\346\215\256\346\255\243\345\234\250\350\256\260\345\275\225\344\270\255\357\274\201</p></body></html>", 0, QApplication::UnicodeUTF8));
                ui->label_usbNotify->setStyleSheet(QString::fromUtf8( "background-color: rgba(0,0,0,0);"));
/*
                QString text("log com2 rawephema\r\n");
                ::write(m_fd, text.toLatin1(), text.length());
                */
                usleep(200000);
                QString text("log com2 rawephema\r\n");
                ::write(m_fd, text.toLatin1(), text.length());
                flag_write_ephem = 1;


            }
            if(delayNum<=0){
                if(1==flag_write_ephem){
                usleep(20000);
                QString text_new("log com2 rawephema onnew\r\n");
                ::write(m_fd, text_new.toLatin1(), text_new.length());
                flag_write_ephem = 0;
                }
                delayNum=200;
            }

         }

    }
    else{
        delayNum=200;
         enableLogging(0);
     //    ui->label_usbNotify->setText(tr("请插入U盘记录数据！！！"));
         ui->label_usbNotify->setStyleSheet(QString::fromUtf8("color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.05 rgba(14, 8, 73, 255), stop:0.36 rgba(28, 17, 145, 255), stop:0.6 rgba(126, 14, 81, 255), stop:0.75 rgba(234, 11, 11, 255), stop:0.79 rgba(244, 70, 5, 255), stop:0.86 rgba(255, 136, 0, 255), stop:0.935 rgba(239, 236, 55, 255));\n"
 "background-color: qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0 rgba(35, 40, 3, 255), stop:0.16 rgba(136, 106, 22, 255), stop:0.225 rgba(166, 140, 41, 255), stop:0.285 rgba(204, 181, 74, 255), stop:0.345 rgba(235, 219, 102, 255), stop:0.415 rgba(245, 236, 112, 255), stop:0.52 rgba(209, 190, 76, 255), stop:0.57 rgba(187, 156, 51, 255), stop:0.635 rgba(168, 142, 42, 255), stop:0.695 rgba(202, 174, 68, 255), stop:0.75 rgba(218, 202, 86, 255), stop:0.815 rgba(208, 187, 73, 255), stop:0.88 rgba(187, 156, 51, 255), stop:0.935 rgba(137, 108, 26, 255), stop:1 rgba(35, 40, 3, 255));"));
         ui->label_usbNotify->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" color:#ff0000;\">\350\257\267\346\217\222\345\205\245U\347\233\230\350\256\260\345\275\225\346\225\260\346\215\256\357\274\201</span></p></body></html>", 0, QApplication::UnicodeUTF8));

     }

}



void MainWindow::remoteDataIncoming()
{
//    char buff[2*(66+49+29)+30];
    char buff[2*(74+48+151)+30];
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
    int bytesRead=read(m_fd, buff, 2*(74+48+151));
    if (bytesRead<1) {
        QMessageBox::warning(this, tr("Error"), tr("Receive error!"));
        return;
    }
/*
    if (m_logFile.isOpen())
    {
       m_logFile.write(buff, bytesRead);
    }
*/
    QString buff_qs(buff);

    ui->m_receiveEdit->append(buff_qs);
    ui->m_receiveEdit->append(QString("\n@@@@@@@@@@@@@@@@@@@@\n"));
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
    while(count_i<bytesRead)
{

// ui->m_receiveEdit->append(QString("\nstart deal with ..."));
     while (count_i<bytesRead&&!gps_nmea.msg_available)
     { nmea_parse_char(buff[count_i]);
         count_i++;
     }
// ui->m_receiveEdit->append(QString("\nread buff over ..."));
    if (gps_nmea.msg_available) {
      nmea_parse_msg();

      gps_nmea.msg_available = FALSE;
    }
//  ui->m_receiveEdit->append(QString("\nparse gps over ..."));
  }

   ui->m_receiveEdit->append(QString("\nfull loop is over ..."));
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
    temp_value+=QChar('0');
    ui->m_time_ms->display(temp_value);

  //  ui->m_time_ms->display(temp_value.setNum(fmod(gps.tow,1000)));

  //  ui->m_time->display(temp_value.setNum(gps.tow,'g',11));
   if(gps_nmea.pos_available){
       ui->m_label_available->setText(QString("OK"));
       ui->m_label_available->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));

     //  ui->m_location_N->display(temp_value.setNum(gps.lat,'g',11));
      // ui->m_location_N->display(temp_value((char *)gps.lat_ch));
       ui->m_location_N->display((char *)gps.lat_ch);
       ui->m_label_N->setText(QString(QChar(gps.NorS)));
     //  ui->m_location_E->display(temp_value.setNum(gps.lon,'g',11));
       ui->m_location_E->display((char *)gps.lon_ch);
       ui->m_label_E->setText(QString(QChar(gps.EorW)));

     //  ui->m_speed->display(temp_value.setNum(gps.pdop));
    //  ui->m_speed->display((char *)gps.speed_ch);
       ui->m_speed->display(gps.gspeed);
    //   ui->m_direction->display(temp_value.setNum(gps.num_sv));
       if (gps.gspeed>1){
          ui->m_direction->display((char *)gps.direction_ch);
       }
       else{
           ui->m_direction->display("");
       }

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

       ui->m_location_N->display("");
       ui->m_label_N->setText("");
       ui->m_location_E->display("");
       ui->m_label_E->setText("");
       ui->m_speed->display("");
       ui->m_direction->display("");
       ui->m_hight->display("");
       ui->m_speed_U->display("");
       ui->m_speed_E->display("");
       ui->m_speed_N->display("");
   }

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
   }
   else
   {
      m_logFile.close();

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
