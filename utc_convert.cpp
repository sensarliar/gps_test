#include "gps_nmea.h"

//int utc2bj_time(int utc_hour, struct GpsState gps);

int getDays(int month, int year) {

       int days = 0;

       switch (month) {
           case 1:
           case 3:
           case 5:
           case 7:
           case 8:
           case 10:
           case 12:
               days = 31;
               break;
           case 4:
           case 6:
           case 9:
           case 11:
               days = 30;
               break;
           case 2:
               if(year % 4 == 0) {
                    if((year%100==0)&&(year%400!=0))
                    {
                    days = 28;
                    }else{
                    days = 29;
                    }
               }
               else {
                   days = 28;
               }
               break;
           default:
               break;
       }

       return days;
   }


int utc2bj_time(int utc_hour, struct GpsState* gps_p)
{
    int bj_hour;
    if(utc_hour+8 <24)
    {
        bj_hour = utc_hour+8;
    /*     (*gps_p).day_zda =10;
        (*gps_p).month_zda =7;
        (*gps_p).year_zda =2088;
    test only
    */

    }else
    {
        bj_hour = (utc_hour+8)%24;
        if((*gps_p).day_zda < getDays((*gps_p).month_zda,(*gps_p).year_zda))
        {
            (*gps_p).day_zda++;
        }
        else
        {
            (*gps_p).day_zda = 1;
            if((*gps_p).month_zda < 12)
            {
                (*gps_p).month_zda++;
            }
            else
            {
                (*gps_p).month_zda = 1;
                (*gps_p).year_zda++;
            }

        }
    }
    return bj_hour;

}











