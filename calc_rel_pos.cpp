#include "gps_nmea.h"
#include <math.h>


/* these value should be measured by the plane employee
(jiayou?? plane ant as original,jiayou plane head as oridinator x,vertical as z,right hand ordinator)
the position value of tail to the ant at jiayou plane.
may jiayou plane was not coordinate with shouyou plane ??
if know the heading,atitude of jiayou plane and heading of shouyou plane, the const may can be calibrated.
*/
#define POS_TAIL2ANT_JIAYOU_X 0
#define POS_TAIL2ANT_JIAYOU_Y 0
#define POS_TAIL2ANT_JIAYOU_Z 0

/* these value should be measured by the plane employee
(shouyou plane ant as original,shouyou plane head as oridinator x,vertical as z,right hand ordinator)
the position value of head to the ant at shouyou plane.
*/
#define POS_HEAD2ANT_SHOUYOU_X 0
#define POS_HEAD2ANT_SHOUYOU_Y 0
#define POS_HEAD2ANT_SHOUYOU_Z 0


/* rotate the enu cordinator to the xyz cordinator(shouyou plane ant as original,shouyou plane head as oridinator x,vertical as z,right hand ordinator)
 *should consider the latancy of bestvela message.
*/
/*
void calc_xyz_plane_ordinator()
{
    double rot_angle = 90+gps.speed_angle;//if>360??
    gps.rel_ant2plane_pos.x = gps.rel_ant_pos.x * cos(rot_angle) + gps.rel_ant_pos.y * sin(rot_angle);
    gps.rel_ant2plane_pos.y = gps.rel_ant_pos.y * cos(rot_angle) - gps.rel_ant_pos.x * sin(rot_angle);
    gps.rel_ant2plane_pos.z = gps.rel_ant_pos.z;

}
*/

void calc_enu2xyz_plane_ordinator(struct point_3d *dest,struct point_3d *src)
{
    double rot_angle = 90+gps.speed_angle;//if>360??

    const double pi = 4.0*atan(1.0);
    double rot_angle_rad = rot_angle*pi/180;


    (*dest).x = (*src).x * cos(rot_angle_rad) + (*src).y * sin(rot_angle_rad);
   (*dest).y = (*src).y * cos(rot_angle_rad) - (*src).x * sin(rot_angle_rad);
    (*dest).z = (*src).z;

}


/*the odinator was not changed.
*/
void calc_tail2plane_pos()
{
    gps.rel_tail2plane_pos.x = gps.rel_ant2plane_pos.x + POS_TAIL2ANT_JIAYOU_X;
    gps.rel_tail2plane_pos.y = gps.rel_ant2plane_pos.y + POS_TAIL2ANT_JIAYOU_Y;
    gps.rel_tail2plane_pos.z = gps.rel_ant2plane_pos.z + POS_TAIL2ANT_JIAYOU_Z;
}

/*the ordinator was moved to the head of shouyou plane.
*/
void calc_tail2head_pos()
{
        gps.rel_tail2head_pos.x = gps.rel_tail2plane_pos.x - POS_HEAD2ANT_SHOUYOU_X;
        gps.rel_tail2head_pos.y = gps.rel_tail2plane_pos.y - POS_HEAD2ANT_SHOUYOU_Y;
        gps.rel_tail2head_pos.z = gps.rel_tail2plane_pos.z - POS_HEAD2ANT_SHOUYOU_Z;
}

/*calculate the relative speed method 1:
 *distance/time = speed ,
 *then convert from enu to xyz plane.
*/

void calc_rel_speed_method1()
{
    gps.rel_speed_enu.x = (gps.rel_ant_pos.x - gps.rel_ant_last_pos.x)/0.05;
    gps.rel_speed_enu.y = (gps.rel_ant_pos.y - gps.rel_ant_last_pos.y)/0.05;
    gps.rel_speed_enu.z = (gps.rel_ant_pos.z - gps.rel_ant_last_pos.z)/0.05;

    calc_enu2xyz_plane_ordinator(&(gps.rel_speed_xyz),&(gps.rel_speed_enu));

}
