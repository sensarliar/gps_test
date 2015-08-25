

/**
 * @file gps_nmea.c
 * Parser for the NMEA protocol.
 *
 * TODO: THIS NMEA-PARSER IS NOT WELL TESTED AND INCOMPLETE!!!
 * Status:
 *  Parsing GGA and RMC is complete, GSA and other records are
 *  incomplete.
 */

//#include "subsystems/gps.h"
//#include "gps.h"
#include "gps_nmea.h"
#include <unistd.h>
#include <cstring>
#include <stdio.h>

//#define FALSE 0
//#define TRUE 1

#if GPS_USE_LATLONG
/* currently needed to get nav_utm_zone0 */
#include "subsystems/navigation/common_nav.h"
#endif


#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#ifdef DEBUG_NMEA
// do debug-output if run on the DEBUG_NMEA-target

#endif


struct GpsNmea gps_nmea;
struct GpsState gps;

//void parse_nmea_GPGSA(void);
//void parse_nmea_GPRMC(void);
void parse_nmea_NVVTG(void);
void parse_nmea_GPGGA(void);


void gps_impl_init( void ) {
  gps_nmea.msg_available = FALSE;
//  gps_nmea.pos_available = FALSE;
//  gps_nmea.gps_nb_ovrn = 0;
 // gps_nmea.msg_len = 0;
}








/**
 * parse GPGGA-nmea-messages stored in
 * gps_nmea.msg_buf .
 */
void parse_nmea_GPGGA(void) {
  int i = 6;     // current position in the message, start after: GPGGA,
  char* endptr;  // end of parsed substrings
//  double degrees, minutesfrac;
//  struct LlaCoor_f lla_f;

  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }

  // get UTC time [hhmmss.sss]
  // ignored GpsInfo.PosLLA.TimeOfFix.f = strtod(&packet[i], &endptr);
  // FIXME: parse UTC time correctly
  //double time = strtod(&gps_nmea.msg_buf[i],&endptr);
 // gps.tow = (uint32_t)((time+1)*1000);
  //gps.tow = time*1000;

  //AD TODO: strtod itow
  int j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: time
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.time_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.time_ch[j]='\0';
  // get latitude [ddmm.mmmmm]
//  double lat = strtod(&gps_nmea.msg_buf[i], &endptr);
  // convert to pure degrees [dd.dddd] format
//  minutesfrac = modf(lat/100, &degrees);
 // lat = degrees + (minutesfrac*100)/60;
  // convert to radians
  //GpsInfo.PosLLA.lat.f *= (M_PI/180);
 // gps.lat = lat*10000;
    j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: latitude
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.lat_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.lat_ch[j]='\0';

  gps.lat_du_ch[0]=gps.lat_ch[0];
  gps.lat_du_ch[1]=gps.lat_ch[1];
  gps.lat_du_ch[2]='\0';
  /*gps.lat = strtod(&gps.lat_ch[5], &endptr);
  gps.lat = gps.lat*60/10000;
  gps.lat=(double((int)(gps.lat*100)))/100;
  //strtod();
*/
  gps.NorS = gps_nmea.msg_buf[i];
  // correct latitute for N/S
 // if(gps_nmea.msg_buf[i] == 'S')
 //   lat = -lat;

  // convert to radians
//  lla_f.lat = RadOfDeg(lat);

//  gps.lla_pos.lat = lla_f.lat * 1e7; // convert to fixed-point
  NMEA_PRINT("p_GPGGA() - lat=%d gps_lat=%i\n\r", (lat*1000), lla_f.lat);


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: N / S
    if (i >= gps_nmea.msg_len)
      return;
  }
/*
  // get longitude [ddmm.mmmmm]
  double lon = strtod(&gps_nmea.msg_buf[i], &endptr);
  // convert to pure degrees [dd.dddd] format
//  minutesfrac = modf(lon/100, &degrees);
//  lon = degrees + (minutesfrac*100)/60;
  gps.lon = lon*10000;
  // convert to radians
  //GpsInfo.PosLLA.lon.f *= (M_PI/180);
  */
  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: LOGITUDE
    if (i >= gps_nmea.msg_len)
      return;
    gps.lon_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.lon_ch[j]='\0';

  gps.lon_du_ch[0]=gps.lon_ch[0];
  gps.lon_du_ch[1]=gps.lon_ch[1];
  gps.lon_du_ch[2]=gps.lon_ch[2];
  gps.lon_du_ch[3]='\0';

    gps.EorW = gps_nmea.msg_buf[i];
  // correct latitute for E/W
 /* if(gps_nmea.msg_buf[i] == 'W')
    lon = -lon;
*/
  // convert to radians
//  lla_f.lon = RadOfDeg(lon);

//  gps.lla_pos.lon = lla_f.lon * 1e7; // convert to fixed-point
  NMEA_PRINT("p_GPGGA() - lon=%d gps_lon=%i time=%u\n\r", (lon*1000), lla_f.lon, gps.tow);


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: E / W
    if (i >= gps_nmea.msg_len)
      return;
  }

  // position fix status
  // 0 = Invalid, 1 = Valid SPS, 2 = Valid DGPS, 3 = Valid PPS
  // check for good position fix
  if( (gps_nmea.msg_buf[i] != '0') && (gps_nmea.msg_buf[i] != ',') )  {
    gps_nmea.pos_available = TRUE;
    NMEA_PRINT("p_GPGGA() - POS_AVAILABLE == TRUE\n\r");
  } else {
    gps_nmea.pos_available = FALSE;
    NMEA_PRINT("p_GPGGA() - gps_pos_available == false\n\r");
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field: satellites used
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r\r");
      return;
    }
  }
  // get number of satellites used in GPS solution
  gps.num_sv = atoi(&gps_nmea.msg_buf[i]);
  NMEA_PRINT("p_GPGGA() - gps_numSatlitesUsed=%i\n\r", gps.num_sv);

  while(gps_nmea.msg_buf[i++] != ',') {              // next field: HDOP (horizontal dilution of precision)
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
  }
  // we use HDOP here, as the PDOP is not in the message
  float hdop = strtof(&gps_nmea.msg_buf[i], &endptr);
 // gps.pdop = hdop * 100;
  gps.pdop = hdop;

  while(gps_nmea.msg_buf[i++] != ',') {              // next field: altitude
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
  }
  // get altitude (in meters) above geoid (MSL)
  // lla_f.alt should actuall be height above ellipsoid,
  // but since we don't get that, use hmsl instead
 // lla_f.alt = strtof(&gps_nmea.msg_buf[i], &endptr);
  float lla_f_alt = strtof(&gps_nmea.msg_buf[i], &endptr);
//  gps.hmsl = lla_f.alt * 1000;
  // gps.hmsl = lla_f_alt * 1000;
  gps.alt = lla_f_alt;
//  gps.lla_pos.alt = gps.hmsl;
//  NMEA_PRINT("p_GPGGA() - gps_alt=%i\n\r", gps.hmsl);
 j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: altitude units, always 'M'
    if (i >= gps_nmea.msg_len)
      return;
   gps.alt_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.alt_ch[j]='\0';
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: geoid seperation
    if (i >= gps_nmea.msg_len)
      return;
  }

  //while(gps_nmea.msg_buf[i++] != '*');              // next field: checksum

       gps.bestvela_parse_ok =1;  //align at the last one

}




/**
 * parse NVVTG-nmea-messages stored in
 * gps_nmea.msg_buf .
 */
void parse_nmea_NVVTG(void) {
  int i = 6;     // current position in the message, start after: GPGGA,
  char* endptr;  // end of parsed substrings
//  double degrees, minutesfrac;
//  struct LlaCoor_f lla_f;

  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }


  int j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: direction,000.0~359.9
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.direction_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.direction_ch[j]='\0';


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: T
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
  }



  while(gps_nmea.msg_buf[i++] != ',') {              // next field: magnatic direction,compass
    if (i >= gps_nmea.msg_len)
      return;
  }



  while(gps_nmea.msg_buf[i++] != ',') {              // next field:M
    if (i >= gps_nmea.msg_len)
      return;
  }


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: speed of knots
    if (i >= gps_nmea.msg_len)
      return;
  }


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: unit of knots
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r\r");
      return;
    }
  }

    j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.speed_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.speed_ch[j]='\0';
  gps.gspeed= strtod(&gps.speed_ch[0],&endptr);
//  gps.gspeed /= 36;  // transfer from km/h to m/s

  gps.gspeed=(double((int)(gps.gspeed*100/36)))/10;



  while(gps_nmea.msg_buf[i++] != ',') {              // next field: km/h
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
  }


}




/**
 * parse NVDVI-nmea-messages stored in
 * gps_nmea.msg_buf .
 */
void parse_nmea_NVDVI(void) {
  int i = 6;     // current position in the message, start after: GPGGA,
//  char* endptr;  // end of parsed substrings


  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }


  int j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: East speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.speed_E_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.speed_E_ch[j]='\0';


 j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: North speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.speed_N_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.speed_N_ch[j]='\0';


  j=0;
   while(gps_nmea.msg_buf[i++] != '*') {              // next field: UP speed
     if (i >= gps_nmea.msg_len) {
       NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
       return;
     }
     gps.speed_U_ch[j++]=gps_nmea.msg_buf[i-1];
   }
   gps.speed_U_ch[j]='\0';




}


/**
 * parse GPZDA-nmea-messages stored in
 * gps_nmea.msg_buf .
 */
void parse_nmea_GPZDA(void) {
  int i = 6;     // current position in the message, start after: GPZDA,
//  char* endptr;  // end of parsed substrings


  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }


  int j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: East speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.time_zda_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.time_zda_ch[j]='\0';
  gps.time_zda_ch[6]='\0';
  gps.time_zda=atoi(&gps.time_zda_ch[0]);

  bool time_zda_flag=0;
  gps.time_zda+=80000;
  if(gps.time_zda>=240000)
  {
      time_zda_flag=1;
      gps.time_zda-=240000;
  }


  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: East speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.day_zda_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.day_zda_ch[j]='\0';
  gps.day_zda=atoi(&gps.day_zda_ch[0]);
/*   the day month,and year was dealt at writing function ,so ignore it ;only the time_zda was concerned*/
//  if(1==time_zda_flag)
//  {gps.day_zda++;}

  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: East speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.month_zda_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.month_zda_ch[j]='\0';
  gps.month_zda=atoi(&gps.month_zda_ch[0]);


  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: East speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.year_zda_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.year_zda_ch[j]='\0';
  gps.year_zda=atoi(&gps.year_zda_ch[0]);

  if(gps.time_zda||gps.day_zda||gps.month_zda||gps.year_zda)
      gps.time_zda_ok=1;

}

/**
 * parse ALIGNBSLNENUA, NOVATEL messages stored in
 * gps_nmea.msg_buf .
 * #ALIGNBSLNENUA,COM1,0,68.5,FINESTEERING,1802,459952.900,00000000,8c02,12832;SOL_COMPUTED,
 */
void parse_novatel_alignbslnenua(void) {
  int i = 14;     // current position in the message, start after: bestvela,
  char* endptr;  // end of parsed substrings


  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }

  while(gps_nmea.msg_buf[i++] != ';') {              // next field:12832;;;
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:SOL_COMPUTED
    if (i >= gps_nmea.msg_len)
      return;
  }

  int j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field:NARROW_INT
    if (i >= gps_nmea.msg_len)
      return;
    gps.align_pos_av_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.align_pos_av_ch[j]='\0';

  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field:EAST
    if (i >= gps_nmea.msg_len)
      return;
    gps.rel_pos_E_ch[j++]=gps_nmea.msg_buf[i-1];
  }
   gps.rel_pos_E_ch[j]='\0';

 gps.rel_ant_last_pos.x =  gps.rel_ant_pos.x;
 gps.rel_ant_pos.x = strtod(&gps.rel_pos_E_ch[0],&endptr);


   j=0;
   while(gps_nmea.msg_buf[i++] != ',') {              // next field:EAST
     if (i >= gps_nmea.msg_len)
       return;
     gps.rel_pos_N_ch[j++]=gps_nmea.msg_buf[i-1];
   }
    gps.rel_pos_N_ch[j]='\0';

     gps.rel_ant_last_pos.y =  gps.rel_ant_pos.y;
     gps.rel_ant_pos.y = strtod(&gps.rel_pos_N_ch[0],&endptr);

    j=0;
    while(gps_nmea.msg_buf[i++] != ',') {              // next field:EAST
      if (i >= gps_nmea.msg_len)
        return;
      gps.rel_pos_U_ch[j++]=gps_nmea.msg_buf[i-1];
    }
     gps.rel_pos_U_ch[j]='\0';

      gps.rel_ant_last_pos.z =  gps.rel_ant_pos.z;
      gps.rel_ant_pos.z = strtod(&gps.rel_pos_U_ch[0],&endptr);

   if((strlen(gps.align_pos_av_ch) > 9)&&!strncmp(gps.align_pos_av_ch , "NARROW_INT", 10)){
        gps.align_pos_av = 1;
    }else
    {
        gps.align_pos_av = 0;
    }

      gps.align_flag = 1;

}


/**
 * parse BESTVELA, NOVATEL messages stored in
 * gps_nmea.msg_buf .
 * #BESTVELA,COM1,0,68.5,FINESTEERING,1802,459952.900,00000000,8c02,12832;SOL_COMPUTED,DOPPLER_VELOCITY,0.150,0.000,0.0505,12.520238,-0.0537,0.0*18021b75
 */
void parse_novatel_bestvela(void) {
  int i = 9;     // current position in the message, start after: bestvela,
  char* endptr;  // end of parsed substrings


  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: COM1
    if (i >= gps_nmea.msg_len)
      return;
  }
/*
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: 0
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field: 68.5
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:FINESTEERING
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:1802
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:459952.900
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:00000000
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:8c02
    if (i >= gps_nmea.msg_len)
      return;
  }
  */

  while(gps_nmea.msg_buf[i++] != ';') {              // next field:12832;;;
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:SOL_COMPUTED
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:DOPPLER_VELOCITY
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:latency，0.150
    if (i >= gps_nmea.msg_len)
      return;
  }

  while(gps_nmea.msg_buf[i++] != ',') {              // next field:Differential age in seconds，0.000
    if (i >= gps_nmea.msg_len)
      return;
  }



  int j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: horizontal speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.speed_horizon_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.speed_horizon_ch[j]='\0';

  gps.speed_H = strtod(&gps.speed_horizon_ch[0],&endptr);



 j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field:  speed angle
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.speed_angle_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.speed_angle_ch[j]='\0';

  double speed_angle = strtod(&gps.speed_angle_ch[0],&endptr);
  gps.speed_angle = speed_angle;

  const double pi = 4.0*atan(1.0);
  double speed_angle_rad = speed_angle*pi/180;

      gps.speed_E= gps.speed_H*sin(speed_angle_rad);
      gps.speed_N= gps.speed_H*cos(speed_angle_rad);

   //   gps.speed_E= gps.speed_H*sin(speed_angle);
   //   gps.speed_N= gps.speed_H*cos(speed_angle);


  gps.speed_E*=100;
 int temp = (int)gps.speed_E;
  gps.speed_E=double(temp)/100;

  gps.speed_N*=100;

  gps.speed_N=(double((int)gps.speed_N))/100;

  j=0;
   while(gps_nmea.msg_buf[i++] != ',') {              // next field: vertical UP speed
     if (i >= gps_nmea.msg_len) {
       NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
       return;
     }
     gps.speed_U_ch[j++]=gps_nmea.msg_buf[i-1];
   }
   gps.speed_U_ch[j]='\0';
   gps.speed_U = strtod(&gps.speed_U_ch[0],&endptr);
    gps.speed_U=(double((int)(gps.speed_U*100)))/100;

    gps.speed_H=(double((int)(gps.speed_H*100)))/100;//nomalize the speed_H





}


/**
 * parse BESTSATSA, NOVATEL messages stored in
 * gps_nmea.msg_buf .
 * #BESTSATSA,COM1,0,68.5,FINESTEERING,1802,459952.900,00000000,8c02,12832;SOL_COMPUTED,DOPPLER_VELOCITY,0.150,0.000,0.0505,12.520238,-0.0537,0.0*18021b75
 */
void parse_novatel_bestsatsa(void) {
  int i = 10;     // current position in the message, start after: bestvela,
//  char* endptr;  // end of parsed substrings


  // attempt to reject empty packets right away
  if(gps_nmea.msg_buf[i]==',' && gps_nmea.msg_buf[i+1]==',') {
    NMEA_PRINT("p_GPGGA() - skipping empty message\n\r");
    return;
  }


  while(gps_nmea.msg_buf[i++] != ';') {              // next field:12832;;;
    if (i >= gps_nmea.msg_len)
      return;
  }


  int j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: horizontal speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.num_sats_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.num_sats_ch[j]='\0';

  gps.num_sats = atoi(&gps.num_sats_ch[0]);
/*  double num_sats = strtod(&gps.num_sats_ch[0],&endptr);
  gps.num_sats = (int)num_sats;
*/
  if(gps.num_sats <= 0){
      gps.num_sats = 0;
      gps.num_gps = 0;
      gps.num_beidou = 0;
      return;
  }

int gps_num_temp = 0;
int beidou_num_temp = 0;
     //gps.num_gps = 0;
  while(gps_nmea.msg_buf[i++] != '*') {              // next field: horizontal speed
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
  //  if((gps_nmea.msg_buf[i-1] == 'G') && (gps_nmea.msg_buf[i] == 'P')&&gps_nmea.msg_buf[i+1] == 'S'){
    if((gps_nmea.msg_buf[i-1] == 'G')&&gps_nmea.msg_buf[i+1] == 'S'){
       // gps.num_gps++;
        gps_num_temp++;
    }
    if((gps_nmea.msg_buf[i-1] == 'B')&&gps_nmea.msg_buf[i+2] == 'D'){
       // gps.num_gps++;
        beidou_num_temp++;
    }

  }

  if((gps.num_sats >= 0)&&(gps.num_gps >= 0)&&(gps.num_sats >= gps.num_gps)){
 //       gps.num_gps = gps_num_temp;
   //   gps.num_beidou = gps.num_sats - gps.num_gps;
      gps.num_gps = gps_num_temp;
      gps.num_beidou = beidou_num_temp;
  }
  else {
      gps.num_sats = 0;
      gps.num_gps = 0;
      gps.num_beidou = 0;
  }




}


/**
 * parse_nmea_char() has a complete line.
 * Find out what type of message it is and
 * hand it to the parser for that type.
 */
void nmea_parse_msg( void ) {


    if(gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "GPGGA", 5)) {
      gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
      NMEA_PRINT("parse_gps_msg() - parsing GGA gps-message \"%s\" \n\r",gps_nmea.msg_buf);
      NMEA_PRINT("GGA");
      parse_nmea_GPGGA();
    }
    else {
      if(gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "GPVTG", 5)) {
        gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
        NMEA_PRINT("GSA: \"%s\" \n\r",gps_nmea.msg_buf);
        NMEA_PRINT("GSA");
        parse_nmea_NVVTG();

      } else {
          if(gps_nmea.msg_len > 8 && !strncmp(gps_nmea.msg_buf , "BESTVELA", 8)) {
              gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
              NMEA_PRINT("GSA: \"%s\" \n\r",gps_nmea.msg_buf);
              NMEA_PRINT("GSA");
              //parse_nmea_NVDVI();
              parse_novatel_bestvela();
              //        parse_nmea_GPGSA();
          } else {

              if(!gps.file_name_flag && gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "GPZDA", 5)) {
                  gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
                  parse_nmea_GPZDA();

              } else{
                  if(gps_nmea.msg_len > 9 && !strncmp(gps_nmea.msg_buf , "BESTSATSA", 9)) {
                      gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
                      parse_novatel_bestsatsa();

                  } else if(gps_nmea.msg_len > 13 && !strncmp(gps_nmea.msg_buf , "ALIGNBSLNENUA", 13)) {
                      gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
                      parse_novatel_alignbslnenua();

                          } else{

                            gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
                            NMEA_PRINT("ignoring: len=%i \n\r \"%s\" \n\r", gps_nmea.msg_len, gps_nmea.msg_buf);
                          }
              }
          }
      }
    }


  // reset message-buffer
  gps_nmea.msg_len = 0;
}


/**
 * This is the actual parser.
 * It reads one character at a time
 * setting gps_nmea.msg_available to TRUE
 * after a full line.
 */
void nmea_parse_char( char c ) {
  //reject empty lines
  if (gps_nmea.msg_len == 0) {
    if (c == '\r' || c == '\n' || c == '#'|| c == '$')
      return;
  }

  // fill the buffer, unless it's full
  if (gps_nmea.msg_len < NMEA_MAXLEN - 1) {

    // messages end with a linefeed
    //AD: TRUNK:       if (c == '\r' || c == '\n')
    if (c == '\r' || c == '\n') {
      gps_nmea.msg_available = TRUE;
    } else {
      gps_nmea.msg_buf[gps_nmea.msg_len] = c;
      gps_nmea.msg_len ++;
    }
  }

  if (gps_nmea.msg_len >= NMEA_MAXLEN - 1)
    gps_nmea.msg_available = TRUE;
   //   gps_nmea.msg_available = FALSE;
}
