#include <stdio.h>
#include <string.h>
#if 0
#include <iostream>

using namespace std;



int utf8_ucde(unsigned char *tmp){
	//utf-8:1110xxxx 10xxxxxx 10xxxxxx 
	int ret = 0;
	unsigned char a,b,c;

	a = tmp[0]&0xF;//0-3

	b = tmp[1]&0x3F;//0-5

	c = tmp[2]&0x3F;//0-5


	ret = a<<12;
	ret = ret | (b<<6);
	ret = ret | c;

	printf("%x %x %x\n",a,b,c);

	return ret;
}








int main()
{
	string s="abcdef";
	const char *a=s.c_str();

	printf("size:%d %s\n",strlen(a),a);
	printf("%x %x %x %x %x %x\n",(unsigned char)a[0],(unsigned char)a[1],(unsigned char)a[2],(unsigned char)a[3],(unsigned char)a[4],(unsigned char)a[5]);

	unsigned char b = a[0];
	printf("%d\n",b);

//	printf("code:%x\n",utf8_ucde((unsigned char *)a));
	return 0;


}



#endif



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#if 0
int main()
{
	int fd,out;
	int i = 0;

	unsigned char c;
	unsigned char b = '\n';
	char str[5];

	fd = open("ASC48",O_RDONLY);
	if(fd <0){
		printf("open file error\n");
		return -1;
	}


	out = open("asc24_48",O_RDWR|O_CREAT);
	if(out <0){
		printf("open file error\n");
		return -1;
	}


	while(read(fd,&c,1)>0){
		i++;
		memset(str,0,5);
		sprintf(str,"0x%x,",c);
		write(out,str,strlen(str));
		if(i%24==0)
			write(out,&b,1);
	}

	close(fd);
	close(out);


}
#endif



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#if 0
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
	printf("xres                    %d\n",vinfo.xres_virtual);
	printf("yres                    %d\n",vinfo.yres_virtual);
	printf("offs                    %d\n",vinfo.xoffset);
	printf("yoff                    %d\n",vinfo.yoffset);
	printf("nonstd                    %d\n",vinfo.nonstd);
	printf("width                    %d\n",vinfo.width);
	printf("height                    %d\n",vinfo.height);
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

#endif


#if 0
int main()
{
	int fd;
	int i;
	char a[5];

	fd =open("/dev/input/mouse0",O_RDONLY);
	if(fd <0){
		printf("open device error\n");
		return -1;
	}
	for(i = 0;i<10;i++){
		memset(a,0,5);
		read(fd,&a,5);
		printf("%d %d %d %d %d\n",a[0],a[1],a[2],a[3],a[4]);
	}	

	close(fd);
	return 0;
}

#endif


int main()
{

	printf("%d\n",getch());




















}
