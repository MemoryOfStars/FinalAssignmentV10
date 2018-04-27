#include"draw.h"
#include"front.h"

struct fb_var_screeninfo vinfo;  
struct fb_fix_screeninfo finfo;  
char *frameBuffer = 0;  

//打印fb驱动中fix结构信息，注：在fb驱动加载后，fix结构不可被修改。  
void printFixedInfo ()  
{  
	   printf ("Fixed screen info:\n"  
		       "\tid: %s\n"  
			   "\tsmem_start:0x%lx\n"  
			   "\tsmem_len:%d\n"  
			   "\ttype:%d\n"  
			   "\ttype_aux:%d\n"  
			   "\tvisual:%d\n"  
			   "\txpanstep:%d\n"  
			   "\typanstep:%d\n"  
			   "\tywrapstep:%d\n"  
			   "\tline_length: %d\n"  
			   "\tmmio_start:0x%lx\n"  
			   "\tmmio_len:%d\n"  
			   "\taccel:%d\n"  
			   "\n",  
			   finfo.id, finfo.smem_start, finfo.smem_len, finfo.type,  
			   finfo.type_aux, finfo.visual, finfo.xpanstep, finfo.ypanstep,  
			   finfo.ywrapstep, finfo.line_length, finfo.mmio_start,  
			   finfo.mmio_len, finfo.accel);  
}  
   
//打印fb驱动中var结构信息，注：fb驱动加载后，var结构可根据实际需要被重置  
void printVariableInfo ()  
{  
	   printf ("Variable screen info:\n"  
			   "\txres:%d\n"  
			   "\tyres:%d\n"  
			   "\txres_virtual:%d\n"  
			   "\tyres_virtual:%d\n"  
			   "\tyoffset:%d\n"  
			   "\txoffset:%d\n"  
			   "\tbits_per_pixel:%d\n"  
			   "\tgrayscale:%d\n"  
			   "\tred: offset:%2d, length: %2d, msb_right: %2d\n"  
			   "\tgreen: offset:%2d, length: %2d, msb_right: %2d\n"  
			   "\tblue: offset:%2d, length: %2d, msb_right: %2d\n"  
			   "\ttransp: offset:%2d, length: %2d, msb_right: %2d\n"  
			   "\tnonstd:%d\n"  
			   "\tactivate:%d\n"  
			   "\theight:%d\n"  
			   "\twidth:%d\n"  
			   "\taccel_flags:0x%x\n"  
			   "\tpixclock:%d\n"  
			   "\tleft_margin:%d\n"  
			   "\tright_margin: %d\n"  
			   "\tupper_margin:%d\n"  
			   "\tlower_margin:%d\n"  
			   "\thsync_len:%d\n"  
			   "\tvsync_len:%d\n"  
			   "\tsync:%d\n"  
			   "\tvmode:%d\n"  
			   "\n",  
		       vinfo.xres, vinfo.yres, vinfo.xres_virtual, vinfo.yres_virtual,  
			   vinfo.xoffset, vinfo.yoffset, vinfo.bits_per_pixel,  
			   vinfo.grayscale, vinfo.red.offset, vinfo.red.length,  
			   vinfo.red.msb_right,vinfo.green.offset, vinfo.green.length,  
			   vinfo.green.msb_right, vinfo.blue.offset, vinfo.blue.length,  
			   vinfo.blue.msb_right, vinfo.transp.offset, vinfo.transp.length,  
			   vinfo.transp.msb_right, vinfo.nonstd, vinfo.activate,  
			   vinfo.height, vinfo.width, vinfo.accel_flags, vinfo.pixclock,  
			   vinfo.left_margin, vinfo.right_margin, vinfo.upper_margin,  
			   vinfo.lower_margin, vinfo.hsync_len, vinfo.vsync_len,  
			   vinfo.sync, vinfo.vmode);  
}

//画一条直线  
void drawline_rgb16 (int x0,int y0, int width,int height, int color,int flag0)  
{  
	   const int bytesPerPixel = 2;//因为是rgb16，用16位来描述色深，所以2个字节  
       const int stride = finfo.line_length / bytesPerPixel;// 
       const int red = (color & 0xff0000) >> (16 + 3);//下面是颜色的操作，我目前还没弄明白  
       const int green = (color & 0xff00) >> (8 + 2);  
       const int blue = (color & 0xff) >> 3;  
       const short color16 = blue | (green << 5) | (red << (5 +6));  
       int flag=flag0;//这里我为了图个方便就用一个flag来区分是画横线还是竖线，0表示横线，1表示竖线。  
					    
				     
	   short *dest = (short *) (frameBuffer)+ (y0 + vinfo.yoffset) * stride + (x0 +vinfo.xoffset);//这个就是我们画点的起始位置，其+stride就是换行（这个是我个人通过代码测试得出来的结论）  
						     
       int x=0,y=0;  
	   if(flag==0)  
       {       
  	     for (x = 0; x < width; ++x)//width就是我们x方向的终点  
         {  
 	           dest[x] = color16;  
         }  
       }  
       else if(flag==1)  
       {  
           for(y=0;y<height;y++)//height就是我们y方向的终点  
           {  
	             dest[x]=color16;//这里x始终为0，和下面一句结合起来就是每一行就画一个点，一共画height行，不就是一条竖线了么，这里我还思考了很久。  
																			             
	             dest +=stride;  
		   }  
       }  
} 


//画大小为width*height的同色矩阵，5reds+6greens+5blues  
void drawRect_rgb16 (int x0, int y0, int width,int height, int color)  
{  
	   const int bytesPerPixel = 2;  
	   const int stride = finfo.line_length / bytesPerPixel;  
	   const int red = (color & 0xff0000) >> (16 + 3);  
	   const int green = (color & 0xff00) >> (8 + 2);  
	   const int blue = (color & 0xff) >> 3;  
	   const short color16 = blue | (green << 5) | (red << (5 +6));  
	 
	   short *dest = (short *) (frameBuffer)+ (y0 + vinfo.yoffset) * stride + (x0 +vinfo.xoffset);  
				    
	   int x, y;  
	   for (y = 0; y < height; ++y)  
	   {  
	       for (x = 0; x < width; ++x)  
	       {  
	           dest[x] = color16;  
	       }  
	       dest += stride;  
	   }  
} 

// 画一个空心矩形,可以选择边框颜色,左上角坐标，长宽都已像素为单位
// int x0,y0:左上角坐标
// int width,height: 宽高
// int color:边框颜色
void drawRectEmpty_rgb16 (int x0, int y0, int width,int height, int color)  
{
		drawRect_rgb16(x0,y0,width,LCD_LINE_WIDTH_BY_PIX,color);
		drawRect_rgb16(x0,y0,LCD_LINE_WIDTH_BY_PIX,height,color);
		drawRect_rgb16(x0+width,y0,LCD_LINE_WIDTH_BY_PIX,height,color);
		drawRect_rgb16(x0,y0+height,width,LCD_LINE_WIDTH_BY_PIX,color);
		drawRect_rgb16(x0+width,y0+height,LCD_LINE_WIDTH_BY_PIX,LCD_LINE_WIDTH_BY_PIX,color);
}



//画表格，错误返回0，正确返回1
//int x0,y0  左上角坐标
//int width ,height 每个小方格的宽高
//int color 方框边色
//int col,row  横竖各有几个小方格
int drawStuff_rgb16(int x0,int y0,int width,int height,int color,int col,int row)
{
		if((x0 + width * col) > LCD_WIDTH_BY_PIX )
		{
				return 0;
		}
		if((y0 + height * row) > LCD_HEIGHT_BY_PIX)
		{
				return 0;
		}
		int i,j;
		for(i = 0;i<row;i++)
		{
				for(j = 0; j < col; j++)
				{
						drawRectEmpty_rgb16(x0 + j*width,y0 + i*height,width,height,color);
				}//if end
		}// if end
}
// 写一个字符在某处，返回下一个字符的坐标
// int x0,y0 左上角坐标
// int color 字体颜色
// ch 字符
// int arrary[] 必须是一个大小为2的数组

int drawLetter(int x0, int y0,char ch,int color,int array[])  
{  
	   const int bytesPerPixel = 2;  
	   const int stride = finfo.line_length / bytesPerPixel;  
	   const int red = (color & 0xff0000) >> (16 + 3);  
	   const int green = (color & 0xff00) >> (8 + 2);  
	   const int blue = (color & 0xff) >> 3;  
	   const short color16 = blue | (green << 5) | (red << (5 +6));  
	 
	   short *dest = (short *) (frameBuffer)+ (y0 + vinfo.yoffset) * stride + (x0 +vinfo.xoffset);  
				    
	   unsigned char* letter = getLetterFront(ch);
	   int x, y;  
	   int height = LETTER_HEIGHT;
	   int width = LETTER_WIDTH;
	   for (y = 0; y < height; ++y)  
	   {  
		   
	       for (x = 0; x < width; ++x)  
	       {  
	            if(isBitOne(letter))
				{
						dest[x] = color16;
				}
	       }  
	       dest += stride;  
	   }  
	   cleanLetterXY();  // 清零字符像素

	   array[0] = x0 + LETTER_WIDTH; // 保存下一个要写入的坐标
	   array[1] = y0;

	   if(array[0] > LCD_WIDTH_BY_PIX) // 自动换行，换到下一行开始，如果是最后一行，光标到左上角
	   {
			   array[0] = 0;
			   array[1] = (y0 + LETTER_HEIGHT) % LCD_HEIGHT_BY_PIX; // 	
	   }
	   return 0;
} 
// 写一个字符串，如果覆盖掉原来的或者超出屏幕范围都将是未定义的
//
int printfScreenLetter(int x0,int y0,const char* frame,int color)
{
		int array[2] = {x0,y0};
		for(;*frame;)
		{
				drawLetter(array[0],array[1],*frame,color,array);
				frame++;
		}
		return 0;
}


// 画一个MP3的显示列表
// 窗口左上角坐标，窗棱高度，左边和右边窗口大小使用宏定义
 // 成功返回0，失败返回1
 int drawMP3Windows(int color)
{
		int arrary[2] = {
			WINDOWS_POS_X0+LCD_LINE_WIDTH_BY_PIX,
			WINDOWS_POS_Y0+LCD_LINE_WIDTH_BY_PIX
		};
		char ch[2] = "1";
		drawStuff_rgb16(WINDOWS_POS_X0,WINDOWS_POS_Y0,
						WINDOWS_LEFT_INSIDE_WIDTH + LCD_LINE_WIDTH_BY_PIX,
						WINDOWS_SINGLE_HEIGHT,
						color,1,9);
		drawStuff_rgb16(WINDOWS_POS_X0 + LCD_LINE_WIDTH_BY_PIX + WINDOWS_LEFT_INSIDE_WIDTH,
						WINDOWS_POS_X0,
						WINDOWS_RIGHT_INSIDE_WIDTH + LCD_LINE_WIDTH_BY_PIX,
						WINDOWS_SINGLE_HEIGHT,
						color,1,9);
		for(;ch[0]<='9';ch[0]++)
		{
			printfWindows(ch,0x0f0f0f0f,arrary);
		}
		return 0;

}
 
 
// 在窗口中写字
 // 可以设置字的颜色，返回下一个格子写字的坐标
 //
int printfWindows(const char* frame,int color,int array[])
{
		int x0 = array[0];
		int y0 = array[1];
		printfScreenLetter(x0,y0,frame,color);
		array[1] = array[1] + WINDOWS_SINGLE_HEIGHT;
}

/* 清屏操作 */
int cleanScreen(int color)
{

	   const int bytesPerPixel = 2;//因为是rgb16，用16位来描述色深，所以2个字节  
       const int stride = finfo.line_length / bytesPerPixel;// 
       const int red = (color & 0xff0000) >> (16 + 3);//下面是颜色的操作，我目前还没弄明白  
       const int green = (color & 0xff00) >> (8 + 2);  
       const int blue = (color & 0xff) >> 3;  
       const short color16 = blue | (green << 5) | (red << (5 +6));  
	   short *dest = (short *) (frameBuffer);  
						     
	   int  screensize = finfo.smem_len / 2;//fb的缓存长度  
	   int x = 0;
	   for(;x < screensize; x++)
	   {
			   dest[x] = color16;
	   }
}

