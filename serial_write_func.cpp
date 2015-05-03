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
//#include "gps.h"
#include <math.h>
#include <stdlib.h>
#include "mainwindow.h"

#include "gps_nmea.h"


void MainWindow::write_to_serial()
{
//char* src_p_w=buff;
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
    *buff_wr_p = '1';
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

strcpy(buff_wr_p,"\r\n");

//  int bytesWrite=write(m_fd, buff_wr, (strlen(buff_wr)<1023)? strlen(buff_wr):1023);
 int bytesWrite=write(m_fd, buff_wr, strlen(buff_wr));

 int wbuff_len = strlen(buff_wr);
 while (wbuff_len>0)
 {
     buff_wr[wbuff_len--]=0;
 }
/*------------------clear all buff_wr--------------*/

/*
 int time_length;
 time_length=strlen(gps.time_ch);

//binary output
 char convert_ch[30];
 int convert_int;
//  short convert_16bit;
 short convert_16bit;
 buff_wr[0]=0xAA;
 buff_wr[1]=0x55;
 convert_ch[0]=gps.time_ch[0];
 convert_ch[1]=gps.time_ch[1];
 convert_ch[2]='\0';
 buff_wr[2]=char(atoi(convert_ch));//hour

 convert_ch[0]=gps.time_ch[2];
 convert_ch[1]=gps.time_ch[3];
 convert_ch[2]='\0';
 buff_wr[3]=char(atoi(convert_ch));//minute

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

   double gps_total_speed =sqrt(gps.speed_U*gps.speed_U+gps.speed_H*gps.speed_H);
   convert_16bit = (short)(gps_total_speed*10);//total speed*10,2B
   buff_wr[23]=char(convert_16bit);//
   buff_wr[24]=char(convert_16bit>>8);

   buff_wr[31]='\r';
   buff_wr[32]='\n';
   buff_wr[33]='\0';

//  int bytesWrite=write(m_fd, buff_wr, (strlen(buff_wr)<1023)? strlen(buff_wr):1023);
  bytesWrite = write(m_fd, buff_wr, 33);///do not use strlen
  */
}
