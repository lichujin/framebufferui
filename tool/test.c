#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <curses.h>


#if 0

#pragma pack(1)
struct bmphead{
	unsigned short	filetype;
	unsigned int 	filesize;
	unsigned short 	reserved1;
	unsigned short 	reserved2;
	unsigned int 	bfoffset;

	unsigned int 	bisize;
	unsigned int 	biwidth;
	unsigned int 	biheight;
	unsigned short 	biplanes;
	unsigned short 	bibitcount;
	unsigned int 	bicompression;
	unsigned int 	bisizeimage;
	unsigned int 	bixpelspermeter;
	unsigned int 	biypelspermeter;
	unsigned int 	biclrused;
	unsigned int 	biclrimportant;
};

int main(int argc,char **argv){

	struct bmphead a;
	int fd;
	char *file = argv[1];

	if(argc!=2){
		printf("Usage tool xxx.bmp\n");
		return -1;
	}

	printf("Change from %s\n",file);
	fd = open(file,O_RDONLY);
	if(fd<0){
		printf("open file error\n");
		return -1;
	}

	read(fd,&a,sizeof(a));

	printf("filetype	:%d\n",a.filetype);
	printf("filesize	:%d\n",a.filesize);
	printf("reserved1	:%d\n",a.reserved1);	
	printf("reserved2	:%d\n",a.reserved2);	
	printf("bfoffset	:%d\n",a.bfoffset);	
	printf("bisize		:%d\n",a.bisize);	
	printf("biwidth		:%d\n",a.biwidth);		
	printf("biheight	:%d\n",a.biheight);	
	printf("biplanes	:%d\n",a.biplanes);	
	printf("bibitcount	:%d\n",a.bibitcount);	
	printf("bicompression	:%d\n",a.bicompression);	
	printf("bisizeimage	:%d\n",a.bisizeimage);	
	printf("bixpelspermeter	:%d\n",a.bixpelspermeter);	
	printf("biypelspermeter	:%d\n",a.biypelspermeter);	
	printf("biclrused	:%d\n",a.biclrused);	
	printf("biclrimportant	:%d\n",a.biclrimportant);

/*
filetype        :19778
bfoffset        :54
bisize          :40
bibitcount      :24
bicompression   :0*/


	if((a.filetype!=19778)&&(a.bfoffset!=54)&&
		(a.bisize!=40)&&(a.bibitcount!=24)){
		printf("Not correct BMP file\n");
		close(fd);
		return -2;
	}



	close(fd);

	return 0;
}


#endif

#if 0
int main()
{


	unsigned char a = 0xFF;
	int color = a<<8;

	printf("%x\n",color);

	return 0;
}
#endif

int main()
{
	int i;
	struct termios old,save;
	tcgetattr(0,&old);
	tcgetattr(0,&save); 
	cfmakeraw(&old) ;
	tcsetattr(0,TCSANOW,&old);

	int s;
	for(i = 0;i<10;i++){
	s =getchar();
	printf("%d\n",s);}


	tcsetattr(0,TCSANOW,&save);













	return 0;
}
















