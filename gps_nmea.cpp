

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
  gps_nmea.pos_available = FALSE;
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
  double time = strtod(&gps_nmea.msg_buf[i],&endptr);
 // gps.tow = (uint32_t)((time+1)*1000);
  gps.tow = time*1000;

  //AD TODO: strtod itow
  int j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: latitude
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.time_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.time_ch[j]='\0';
  // get latitude [ddmm.mmmmm]
  double lat = strtod(&gps_nmea.msg_buf[i], &endptr);
  // convert to pure degrees [dd.dddd] format
//  minutesfrac = modf(lat/100, &degrees);
 // lat = degrees + (minutesfrac*100)/60;
  // convert to radians
  //GpsInfo.PosLLA.lat.f *= (M_PI/180);
  gps.lat = lat*10000;
    j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: N/S indicator
    if (i >= gps_nmea.msg_len) {
      NMEA_PRINT("p_GPGGA() - skipping incomplete message\n\r");
      return;
    }
    gps.lat_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.lat_ch[j]='\0';
  gps.NorS = gps_nmea.msg_buf[i];
  // correct latitute for N/S
  if(gps_nmea.msg_buf[i] == 'S')
    lat = -lat;

  // convert to radians
//  lla_f.lat = RadOfDeg(lat);

//  gps.lla_pos.lat = lla_f.lat * 1e7; // convert to fixed-point
  NMEA_PRINT("p_GPGGA() - lat=%d gps_lat=%i\n\r", (lat*1000), lla_f.lat);


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: longitude
    if (i >= gps_nmea.msg_len)
      return;
  }

  // get longitude [ddmm.mmmmm]
  double lon = strtod(&gps_nmea.msg_buf[i], &endptr);
  // convert to pure degrees [dd.dddd] format
//  minutesfrac = modf(lon/100, &degrees);
//  lon = degrees + (minutesfrac*100)/60;
  gps.lon = lon*10000;
  // convert to radians
  //GpsInfo.PosLLA.lon.f *= (M_PI/180);
  j=0;
  while(gps_nmea.msg_buf[i++] != ',') {              // next field: E/W indicator
    if (i >= gps_nmea.msg_len)
      return;
    gps.lon_ch[j++]=gps_nmea.msg_buf[i-1];
  }
  gps.lat_ch[j]='\0';
    gps.EorW = gps_nmea.msg_buf[i];
  // correct latitute for E/W
  if(gps_nmea.msg_buf[i] == 'W')
    lon = -lon;

  // convert to radians
//  lla_f.lon = RadOfDeg(lon);

//  gps.lla_pos.lon = lla_f.lon * 1e7; // convert to fixed-point
  NMEA_PRINT("p_GPGGA() - lon=%d gps_lon=%i time=%u\n\r", (lon*1000), lla_f.lon, gps.tow);


  while(gps_nmea.msg_buf[i++] != ',') {              // next field: position fix status
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



}




/**
 * parse NVVTG-nmea-messages stored in
 * gps_nmea.msg_buf .
 */
void parse_nmea_NVVTG(void) {
  int i = 6;     // current position in the message, start after: GPGGA,
//  char* endptr;  // end of parsed substrings
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
 * parse_nmea_char() has a complete line.
 * Find out what type of message it is and
 * hand it to the parser for that type.
 */
void nmea_parse_msg( void ) {


    if(gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "NVGGA", 5)) {
      gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
      NMEA_PRINT("parse_gps_msg() - parsing GGA gps-message \"%s\" \n\r",gps_nmea.msg_buf);
      NMEA_PRINT("GGA");
      parse_nmea_GPGGA();
    }
    else {
      if(gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "NVVTG", 5)) {
        gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
        NMEA_PRINT("GSA: \"%s\" \n\r",gps_nmea.msg_buf);
        NMEA_PRINT("GSA");
        parse_nmea_NVVTG();

      } else {
          if(gps_nmea.msg_len > 5 && !strncmp(gps_nmea.msg_buf , "NVDVI", 5)) {
              gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
              NMEA_PRINT("GSA: \"%s\" \n\r",gps_nmea.msg_buf);
              NMEA_PRINT("GSA");
              parse_nmea_NVDVI();
              //        parse_nmea_GPGSA();
          } else {
              gps_nmea.msg_buf[gps_nmea.msg_len] = 0;
              NMEA_PRINT("ignoring: len=%i \n\r \"%s\" \n\r", gps_nmea.msg_len, gps_nmea.msg_buf);
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
    if (c == '\r' || c == '\n' || c == '$')
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
