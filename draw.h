#ifndef DRAW_H
#define DRAW_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include"front.h"

#define LCD_WIDTH_BY_PIX (800u)       // LCD宽
#define LCD_HEIGHT_BY_PIX (480u)      // LCD长
#define LCD_LINE_WIDTH_BY_PIX (2u)    // LCD线宽
#define LETTER_HEIGHT (37u)           // 字符高度 
#define LETTER_WIDTH (24)             // 字符宽度

#define WINDOWS_POS_X0 (20u)          // 9*2窗口左上角坐标值
#define WINDOWS_POS_Y0 (20u)

#define WINDOWS_SINGLE_HEIGHT (40u)   // 单个窗口高度
#define WINDOWS_LEFT_INSIDE_WIDTH (72u) // 左边窗口内部空白大小
#define WINDOWS_RIGHT_INSIDE_WIDTH (600u)


extern struct fb_fix_screeninfo finfo;
extern char* frameBuffer;
extern struct fb_var_screeninfo vinfo;
//打印fb驱动中fix结构信息，注：在fb驱动加载后，fix结构不可被修改。  
void printFixedInfo ();

//打印fb驱动中var结构信息，注：fb驱动加载后，var结构可根据实际需要被重置  
void printVariableInfo ();

//画一条直线  
void drawline_rgb16 (int x0,int y0, int width,int height, int color,int flag0);
 
//画大小为width*height的同色矩阵，5reds+6greens+5blues  
void drawRect_rgb16 (int x0, int y0, int width,int height, int color);

// 画一个空心矩形,可以选择边框颜色,左上角坐标，长宽都已像素为单位
// int x0,y0:左上角坐标
// int width,height: 宽高
// int color:边框颜色
void drawRectEmpty_rgb16 (int x0, int y0, int width,int height, int color);  
//画表格，错误返回0，正确返回1
//int x0,y0  左上角坐标
//int width ,height 每个小方格的宽高
//int color 方框边色
//int col,row  横竖各有几个小方格
int drawStuff_rgb16(int x0,int y0,int width,int height,int color,int col,int row);

// 写一个字符在某处，返回下一个字符的坐标
// int x0,y0 左上角坐标
// int color 字体颜色
// ch 字符
// int arrary[] 必须是一个大小为2的数组
 
int drawLetter(int x0, int y0,char ch,int color,int array[]);



 // 写一个字符串，如果覆盖掉原来的或者超出屏幕范围都将是未定义的
 //
int printfScreenLetter(int x0,int y0,const char* frame,int color);

// 画一个MP3的显示列表
// 窗口左上角坐标，窗棱高度，左边和右边窗口大小使用宏定义
// 成功返回0，失败返回1
int drawMP3Windows(int color);


// 在窗口中写字
// 可以设置字的颜色，返回下一个格子写字的坐标
// 从array中取出开始坐标，然后下一个格子的开始坐标写入array
int printfWindows(const char* frame,int color,int array[]);
/* 清屏操作 */
int cleanScreen(int color);
#endif
