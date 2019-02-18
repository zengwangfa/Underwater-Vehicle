#ifndef CAMERA_H_H
#define CAMERA_H_H



void Circle_deal_last();
void camera_circle_clear();
void find_edge();
void P_transf();
void img_compress(void *src, void *dst, uint32_t srclen) ;
void img_extract(void *dst, void *src, uint32_t srclen);
void Camera_center();
void cover_image();
void Edge_sum();
void Edge_integral();
void Three_line_mode();
uint8 find_edge_start(uint8 a,uint8 b);
uint8 get_edge_point();
void find_circle_point();
void draw_car_line();
void draw_center_line();
void get_white_proportion();


#endif