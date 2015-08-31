#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qlist.h>


/** @file gps_nmea.h
 * NMEA protocol specific code.
 *
*/

//#include <unistd.h>
//#include <cstring>
//#include <stdio.h>

#ifndef GPS_NMEA_H
#define GPS_NMEA_H

//#include "mcu_periph/uart.h"

//#define DEBUG_NMEA

#define GPS_NB_CHANNELS 16

#ifdef DEBUG_NMEA
//#define NMEA_PRINT(...) {  printf( __VA_ARGS__);};
#define NMEA_PRINT(...) {ui->m_receiveEdit->append( __VA_ARGS__);};
#else
#define NMEA_PRINT(...) {};
#endif

//#define NMEA_MAXLEN 255
#define NMEA_MAXLEN 1024

struct GpsNmea {
  bool msg_available;
  bool pos_available;
//  int gps_nb_ovrn;        // number if incomplete nmea-messages
  char msg_buf[NMEA_MAXLEN];  ///< buffer for storing one nmea-line
  int msg_len;
};

struct point_3d {
    double x;
    double y;
    double z;
};



/** data structure for GPS information */
struct GpsState {
//  int hmsl;                  ///< height above mean sea level in mm



  double tow;                  ///< GPS time of week in ms
  char time_ch[12];
  double lat;
  char lat_ch[12];
  char lat_du_ch[12];
  char NorS;
  double lon;
  char lon_ch[12];
  char lon_du_ch[12];
  char EorW;

  int num_sv;                ///< number of sat in fix
  float pdop;                 ///< position dilution of precision scaled by 100

  float alt;
  char alt_ch[12];

  char direction_ch[12];
  char speed_ch[12];
  double gspeed;
  double speed_angle;

  char speed_horizon_ch[12];
  char speed_angle_ch[12];
  char speed_E_ch[12];
  char speed_N_ch[12];
  char speed_U_ch[12];

  char time_zda_ch[12];
  char day_zda_ch[12];
  char month_zda_ch[12];
  char year_zda_ch[12];
  int time_zda;
  int day_zda;
  int month_zda;
  int year_zda;

 double speed_H;
  double speed_E;
  double speed_N;
  double speed_U;
  bool time_zda_ok;
  bool file_name_flag;

  char num_sats_ch[12];
  int num_sats;
  int num_gps;
  int num_beidou;

  bool bestvela_parse_ok;

  char align_pos_av_ch[16];
  bool align_pos_av;
  char rel_pos_E_ch[16];
  char rel_pos_N_ch[16];
  char rel_pos_U_ch[16];

  struct point_3d rel_ant_last_pos;

  struct point_3d rel_ant_pos;// jiayou plane ant pos (shouyou plane ant as original,enu)

  struct point_3d rel_ant2plane_pos;// jiayou plane ant pos (shouyou plane ant as original,shouyou plane head as oridinator x,vertical as z,right hand ordinator)

  struct point_3d rel_tail2plane_pos;// jiayou plane tail pos(shouyou plane ant as original,shouyou plane head as oridinator x,vertical as z,right hand ordinator)

  struct point_3d rel_tail2head_pos;// jiayou plane tail pos(shouyou plane head as original,shouyou plane head as oridinator x,vertical as z,right hand ordinator)


  struct point_3d rel_speed_enu;
  struct point_3d rel_speed_xyz;

  int no_align_count;
  bool align_flag;

  char time_ch_jy[12];

  char speed_E_ch_jy[12];
  char speed_N_ch_jy[12];
  char speed_U_ch_jy[12];

//

/*
  int16_t gspeed;                ///< norm of 2d ground speed in cm/s
  int16_t speed_3d;              ///< norm of 3d speed in cm/s
  int32_t course;                ///< GPS course over ground in rad*1e7, [0, 2*Pi]*1e7 (CW/north)
  uint32_t pacc;                 ///< position accuracy in cm
  uint32_t sacc;                 ///< speed accuracy in cm/s
  uint32_t cacc;                 ///< course accuracy in rad*1e7
  uint16_t pdop;                 ///< position dilution of precision scaled by 100

  uint8_t fix;                   ///< status of fix
  int16_t week;                  ///< GPS week
  uint32_t tow;                  ///< GPS time of week in ms

  uint8_t nb_channels;           ///< Number of scanned satellites

  uint32_t last_fix_ticks;       ///< cpu time ticks at last valid fix
  uint32_t last_fix_time;        ///< cpu time in sec at last valid fix
  uint16_t reset;                ///< hotstart, warmstart, coldstart
  */
};

struct SpeedInfo{
/*char time_ch_x[12];
char speed_E_ch_x[12];
char speed_N_ch_x[12];
char speed_U_ch_x[12];
*/
char time_ch_jy[12];

char speed_E_ch_jy[12];
char speed_N_ch_jy[12];
char speed_U_ch_jy[12];

};

extern QList <SpeedInfo> list;
extern struct SpeedInfo info_jy;

extern struct GpsNmea gps_nmea;
extern struct GpsState gps;

extern struct GpsNmea gps_nmea_jy;
//extern struct GpsState gps_jy;


/*
 * This part is used by the autopilot to read data from a uart
 */
#define __GpsLink(dev, _x) dev##_x
#define _GpsLink(dev, _x)  __GpsLink(dev, _x)
#define GpsLink(_x) _GpsLink(GPS_LINK, _x)

//#define GpsBuffer() GpsLink(ChAvailable())



extern void gps_impl_init( void );

/** The function to be called when a characted friom the device is available */
extern void nmea_parse_char(char c);
extern void nmea_parse_msg(void);

extern void nmea_parse_char_jy(char c);
extern void nmea_parse_msg_jy(void);


#define gps_nmea_Reset(_val) { }

#endif /* GPS_NMEA_H */
