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
*/

void calc_xyz_plane_ordinator()
{
    double rot_angle = 90+gps.speed_angle;//if>360??
    gps.rel_ant2plane_pos.x = gps.rel_ant_pos.x * cos(rot_angle) + gps.rel_ant_pos.y * sin(rot_angle);
    gps.rel_ant2plane_pos.y = gps.rel_ant_pos.y * cos(rot_angle) - gps.rel_ant_pos.x * sin(rot_angle);
    gps.rel_ant2plane_pos.z = gps.rel_ant_pos.z;

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


