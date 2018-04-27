#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<linux/fb.h>
#include<linux/kd.h>
#include<sys/mman.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<string.h>
#include<error.h>
#include"front.h"
#include"draw.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)  
{  
	   const char *devfile = "/dev/fb0";  
	   long int screensize = 0;  
	   int fbFd = 0;  
		   
	   int nextLetterArray[2] = { 0 };
	   int nextWindowsArray[2] = { WINDOWS_POS_X0 + LCD_LINE_WIDTH_BY_PIX *2 + WINDOWS_LEFT_INSIDE_WIDTH,
			   WINDOWS_POS_Y0 };
			   
			    
	   /* Open the file for reading and writing */  
	   fbFd = open (devfile, O_RDWR);  
	   if (fbFd == -1)  
	   {  
	      perror ("Error: cannot open framebuffer device");  
	      exit (1);  
	   }  
				      
	   //获取finfo信息并显示  
	   if (ioctl (fbFd, FBIOGET_FSCREENINFO, &finfo) == -1)  
	   {  
	       perror ("Error reading fixed information");  
	       exit (2);  
	   }  
	   printFixedInfo ();  
	   //获取vinfo信息并显示  
	   if (ioctl (fbFd, FBIOGET_VSCREENINFO, &vinfo) == -1)  
	   {  
	      perror ("Error reading variable information");  
	      exit (3);  
	   }  
	   printVariableInfo ();  
							      
	  /* Figure out the size of the screen in bytes */  
	   screensize = finfo.smem_len;//fb的缓存长度  
								     
	   /* Map the device to memory */  
	   frameBuffer =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,fbFd, 0);  
	   if (frameBuffer == MAP_FAILED)  
	   {  
	      perror ("Error: Failed to map framebuffer device to memory");  
	      exit (4);  
	   }  
										      

	   /* 清屏操作 */
	   cleanScreen(0xffffffff);
		/* 画表格 9*2  */
	   drawMP3Windows(0x00000000);
	   /*   */
	   printfWindows("mp3 one",0x00000000,nextWindowsArray);
	   printfWindows("mp3 two",0x00000000,nextWindowsArray);
	   printfWindows("mp3 three",0x00000000,nextWindowsArray);




	   sleep (2);  
	   printf (" Done.\n");  
															     
	   munmap (frameBuffer, screensize);   //解除内存映射，与mmap对应  
																			    
	   close (fbFd);  
	   return 0;  
}
