#ifndef _TYPES_H_
#define _TYPES_H_

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




#endif
