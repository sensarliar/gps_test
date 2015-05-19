#ifndef CALC_REL_POS_H
#define CALC_REL_POS_H

//void calc_xyz_plane_ordinator();
void calc_enu2xyz_plane_ordinator(struct point_3d *dest,struct point_3d *src);

void calc_tail2plane_pos();


/*the ordinator was moved to the head of shouyou plane.
*/
void calc_tail2head_pos();

void calc_rel_speed_method1();


#endif // CALC_REL_POS_H
