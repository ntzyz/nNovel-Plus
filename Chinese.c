#include "os.h"
#include "graph.h"
#include "config.h"

#define FONT_GRAY_DIAM 1
#define FONT_COMMON    2
#define FONT_GRAY_ZWEB 3
#define FONT_COMM_GBK  4

int font_type = FONT_GRAY_DIAM;

void write_graph16(char * VRAM , int x,int y,int width,int height,unsigned char * pimage,int is_rev)
{
	int i,j,k,rx,ry,color;
	unsigned char p;
    int rwidth = width/2 + width %2;
	
	for (i=0;i<height;++i,pimage+=rwidth)
	{
		ry = y+i;
		rx = x;
		for (j=0;j<rwidth;++j)
		{
			p = pimage[j];
			for (k=0;k<2;++k,++rx)
			{
				color = p>>(k<<2) & 0xF ;
				if(!is_rev)
					DrawPoint_VRAM(VRAM,rx,ry,color);
				else
					DrawPoint_VRAM(VRAM,rx,ry,15-color);
			}
		}
	}
}

FILE * HZK ;
void OpenHZK()
{
	char temp[50];
	HZK =  fopen( config.Font_path , "rb");
	if( HZK == NULL )
	{
		char msg[500];
		sprintf( msg , "无法打开字库：\n%s\n请确保该字库存在于Fonts目录下，或删除nNovel.cfg后重试。" , config.Font_path );
		show_msgbox( "出错了！" , msg );
		exit(0);
	}
	fseek( HZK , 0 , SEEK_END );
	if( ftell(HZK) == 1058944 )
		font_type = FONT_GRAY_DIAM;
	else if( ftell(HZK) == 766080 )
		font_type = FONT_COMM_GBK;
	else if( ftell(HZK) < 1058944 )
		font_type = FONT_COMMON;
	else 
		font_type = FONT_GRAY_ZWEB;
}

void CloseHZK()
{
	fclose(HZK);
}

void PrintChStr  (char * VRAM,int x,int y, char * str,int cl_fg,int cl_bg)
{
	unsigned char c1,c2,mat[16][8];
	while(*str)
	{
		if(x>320)
		{
			x=0;y+=16;
		}
		if( y >= 240 )
			return ;
		c1=*str++;
		if( !(c1&0x80) )
		{
			if( c1 == '\r' || c1 == '\n'  )
			{
				y += 16 ; x = 0 ; str ++;
				continue;
			}
			if(x > 312)
			{
				y += 16 ; x = 0 ;
			}
			switch(font_type)
			{
				case FONT_GRAY_DIAM:
					DrawAsciiChar_Gray_VRAM( VRAM , x , y , 16 , 8 , c1 , (cl_fg == 15 && cl_bg == 0) ? 1 : 0 );
					break;
				case FONT_COMM_GBK:
				case FONT_COMMON:
					DrawAsciiChar_VRAM( VRAM , x , y+2 , c1 , cl_fg , cl_bg  );
					break;
				default:break;
			}
			x+=8;
			continue;
		}
		if(x + 8 >= 320)
			{x = 0 ; y +=16;}
		c2=*str++;
		if(c1==0xA1&&c2==0xA1)
		{
			x+=16;
			continue;
		}
		switch(font_type)
		{
			case FONT_GRAY_DIAM:
				fseek(HZK,(94*(c1 -0xa0)+(c2-0xa0))*128,SEEK_SET);
				fread(mat,128,1,HZK);
				write_graph16(VRAM,x,y,16,16,mat , (cl_fg == 15 && cl_bg == 0) ? 1 : 0 );
				break;
			case FONT_COMM_GBK:
				if (c2<0x7F)
					fseek(HZK,(190*(c1-0x81)+(c2-0x40))*32,SEEK_SET);
				else
					fseek(HZK,(190*(c1-0x81)+(c2-0x41))*32,SEEK_SET);
				fread(mat,32,1,HZK);
				DrawGraph_VRAM(VRAM,x,y,16,16,mat,cl_fg,cl_bg);
				break;
			case FONT_COMMON:
				fseek(HZK,(94*(c1-0xa1)+(c2-0xa1))*32,SEEK_SET);
				fread(mat,32,1,HZK);
				DrawGraph_VRAM(VRAM,x,y,16,16,mat,cl_fg,cl_bg);
				break;
			default:break;
		}
		x+=16;
	}
}