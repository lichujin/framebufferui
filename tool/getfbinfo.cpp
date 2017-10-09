#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>


int main()
{
	int fd;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

	fd = open("/dev/fb0",O_RDWR);
	if(fd<0){
		printf("Open file error\n");
		return -1;
	}


	ioctl(fd,FBIOGET_FSCREENINFO,&finfo);
	printf("id:%s\n",finfo.id);
	printf("smem_len:%d\n",finfo.smem_len);
	printf("type:%d\n",finfo.type);
	printf("type_aux%d\n",finfo.type_aux);
	printf("visual:%d\n",finfo.visual);
	printf("xpanstep:%d\n",finfo.xpanstep);
	printf("ypanstep:%d\n",finfo.ypanstep);
	printf("ywrapstep:%d\n",finfo.ywrapstep);
	printf("line_length:%d\n",finfo.line_length);
	printf("mio_start:%d\n",finfo.mmio_start);
	printf("mio_len:%d\n",finfo.mmio_len);
	printf("accel:%d\n",finfo.accel);
	
	ioctl(fd,FBIOGET_VSCREENINFO,&vinfo); 
	printf("xres                    %d\n",vinfo.xres);
	printf("yres                    %d\n",vinfo.yres);
	printf("xres_virtual            %d\n",vinfo.xres_virtual);
	printf("yres_virtual            %d\n",vinfo.yres_virtual);
	printf("xoffs                   %d\n",vinfo.xoffset);
	printf("yoff                    %d\n",vinfo.yoffset);
	printf("bpp						%d\n",vinfo.bits_per_pixel);
	printf("nonstd                  %d\n",vinfo.nonstd);
	printf("width                   %d\n",vinfo.width);
	printf("height                  %d\n",vinfo.height);
	//printf("hsync_len                 %d\n",vinfo.hsync_len); 
	//printf("vsync_len                 %d\n",vinfo.vsync_len);
	//printf("sync                      %d\n",vinfo.sync); 
	//printf("vmode                     %d\n",vinfo.vmode);
	//printf("rotate                    %d\n",vinfo.rotate);
	//printf("rimar                    %d\n",vinfo.right_margin);
	//printf("leftm                    %d\n",vinfo.left_margin);
	close(fd);
	return 0;

}

