#include "os.h"
#include "char.h"
#include "math.h"
#include "Chinese.h"
#include "graph.h"

void DrawAsciiChar_Gray_VRAM(char *VRAM , int x,int y,int width,int height,char c1,int is_rev)
{
	int i,j,rx,ry;
	unsigned char * pimage = ascii_gray + (c1-0x20)*64 ;
	for (i=0;i<16;i++)
	  for (j=0;j<4;j++)
	  {
		  rx=x+j*2;
		  ry=y+i;
		  if(!is_rev)
		  {
			DrawPoint_VRAM(VRAM,rx,ry,(pimage[i*4+j]>>4)&0xF);
			DrawPoint_VRAM(VRAM,rx+1,ry,pimage[i*4+j]&0xF);
		  }else
		  {
			DrawPoint_VRAM(VRAM,rx,ry,15-(pimage[i*4+j]>>4)&0xF);
			DrawPoint_VRAM(VRAM,rx+1,ry,15-pimage[i*4+j]&0xF);  
		  }
	  }
}
void DrawAsciiStringGray_VRAM(char *VRAM,int x,int y,char *string,int cl_fg,int cl_bg)
{
	while(1)
	{
		DrawAsciiChar_Gray_VRAM( VRAM , x , y , 16 , 8 , *string , (cl_fg == 15 && cl_bg == 0) ? 1 : 0 );
		string ++; x += 8 ;
		if(*string == '\0' || x > 320 )return;
	}
}

char * init_VRAM() 
{
	return malloc( SCREEN_BYTES_SIZE );
}

void AllClr_VRAM(char * VRAM)
{	memset(VRAM,0xff,SCREEN_BYTES_SIZE);}


void PutDisp_DDVRAM(char * VRAM)
{	memcpy(SCREEN_BASE_ADDRESS,VRAM,SCREEN_BYTES_SIZE);}

void close_VRAM(char * VRAM){free(VRAM);}

int DrawPoint_VRAM( char * VRAM , int x , int y , char color )
{
	if( x < 0 || x >= 320 || y < 0 || y >= 240 )
		return -1;
	unsigned char * p = VRAM + (x >> 1) + ( y << 7 ) + (y << 5 );
	*p = ( x & 1) ? ((*p & 0xf0 ) | color ) : (( *p & 0x0f ) | ( color << 4 ));
	return 1;
}

int GetPoint_VRAM(char * VRAM,int x, int y) 
{
	unsigned char* p = (unsigned char*)(VRAM + ((x >> 1) + (y << 7) + (y << 5)));
	if(x >= 0 && y >= 0 &&x < 320 && y < 240 )return ((x & 1) ? (*p & 0x0F) : (*p >> 4));
	return 0 ;
}

void AreaClr_VRAM(char * VRAM , int x1 , int y1 , int x2 , int y2 )
{
	int y ;
	for( ; x1 <= x2 ; x1 ++ )
		for( y = y1; y <= y2 ; y++ )
			DrawPoint_VRAM( VRAM , x1 , y , 15  );
}
void AreaRev_VRAM(char * VRAM , int x1 , int y1 , int x2 , int y2 )
{
	int y ;
	for( ; x1 <= x2 ; x1 ++ )
		for( y = y1; y <= y2 ; y++ )
			DrawPoint_VRAM( VRAM , x1 , y , 15 - GetPoint_VRAM( VRAM, x1 , y)  );
}

void DrawGraph_VRAM(char * VRAM,int x,int y,int width,int height,char * pimage,char cl_fg,char cl_bg)
{
	int i,j,k,pixel,rx=0,ry=0;
	unsigned char p;
	int iwidth = width/8 + (width % 8 ? 1:0);
	for (i=0;i<height;++i,pimage+=iwidth)
	{
		ry = y+i;
		if (ry>=240) return;
		else if (ry<0) continue;
		for (j=0;j<iwidth;++j)
		{
			p = pimage[j];
			for (k=0;k<8;++k)
			{
				rx = x+(8-k)+8*j;
				pixel = p % 2;
				p>>=1;
				if (pixel)			DrawPoint_VRAM(VRAM,rx-1,ry,cl_fg);
				else 				DrawPoint_VRAM(VRAM,rx-1,ry,cl_bg);
			}
		}
	}
}

void AllFill_VRAM(char * VRAM,int color)
{
	int x , y ;
	for( x = 0 ; x < 320 ; x ++)
		for(y = 0; y < 240 ; y ++)
			DrawPoint_VRAM(VRAM, x , y , color );
}

void Draw_Line_VRAM (char * VRAM,float x1 , float y1 , float x2 , float y2 , int color)
{
	float i ;
	if( x1 == x2 )				//此时，斜率不存在
	{
		if( y1 > y2 ) { i = y1 ; y1 = y2 ; y2 = i ;}
		for( ; y1 <= y2 ; y1 += 1)
			DrawPoint_VRAM (VRAM,x1 , y1 , color );
		return ;
	}
	float k , b ;
	k = (y2 - y1)/(x2 - x1);	//计算斜率
	b = y1 - k * x1 ;			//计算截距
	if( fabs(k) <= 1 )			//当斜率小于1时，直线比较平缓，以x方向逐个画点
	{
		if( x1 > x2 )			//交换x,y的值，如果需要的话
		{
			i = y1 ; y1 = y2 ; y2 = i ;
			i = x1 ; x1 = x2 ; x2 = i ;
		}
		for( ; x1 <= x2 ; x1 +=1 )
			DrawPoint_VRAM (VRAM, x1 , k * x1 + b , color);
		return ;
	}
	else						//当斜率大于1时，直线比较陡峭，以y方向逐个画点
	{
		if( y1 > y2 )			//交换x,y的值，如果需要的话  
		{
			i = y1 ; y1 = y2 ; y2 = i ;
			i = x1 ; x1 = x2 ; x2 = i ;
		}
		for( ; y1 <= y2 ; y1 +=1 )
			DrawPoint_VRAM (VRAM, ( y1 - b ) / k , y1 , color);
		return ;
	}
}

void DrawAsciiChar_VRAM(char *VRAM,int x,int y,char c,int cl_fg,int cl_bg)
{
	DrawGraph_VRAM( VRAM , x , y ,8 , 12 , charMap_ascii[c] , cl_fg , cl_bg );
}
void Draw_Mini_Char(char * VRAM,int x, int y, char ch, int textColor, int bgColor)
{
	int i, j, pixelOn;
	for(i = 0; i < 6; i++)
	{
		for(j = 8; j > 0; j--)
		{
			pixelOn = charMap_ascii_mini[(unsigned char)ch][i] << j ;
			pixelOn = pixelOn & 0x80 ;
			if (pixelOn) 		DrawPoint_VRAM(VRAM,x+i,y+8-j,textColor);
			else 			 	DrawPoint_VRAM(VRAM,x+i,y+8-j,bgColor);
		}
	}
}
void DrawAsciiString_VRAM(char *VRAM,int x,int y,char *string,int cl_fg,int cl_bg)
{
	while(1)
	{
		DrawAsciiChar_VRAM( VRAM , x , y , *string , cl_fg , cl_bg );
		string ++; x += 8 ;
		if(*string == '\0' || x > 320 )return;
	}
}

void DrawMiniString_VRAM (char * VRAM,int x , int y , char * str , int cl_fg , int cl_bg )
{	int pos = 0 ;
	while(1)
	{
		Draw_Mini_Char(VRAM,x , y , str[pos] , cl_fg , cl_bg );
		pos++;
		if( x + 6 > 320 ) return ;
		if(str[pos] == 0 ) return;
		x += 6;
	}
}

void Draw_Rect_VRAM(char * VRAM, int x1 , int y1 ,int x2 ,int y2,int color)
{
	Draw_Line_VRAM(VRAM,x1,y1,x2,y1,color);
	Draw_Line_VRAM(VRAM,x2,y1,x2,y2,color);
	Draw_Line_VRAM(VRAM,x2,y2,x1,y2,color);
	Draw_Line_VRAM(VRAM,x1,y2,x1,y1,color);
}
void Fill_Rect_VRAM(char * VRAM, int x1 , int y1 ,int x2 ,int y2,int color)
{
	int i ;
	if( x1 > x2 )
	{
		i = x1 ; x1 = x2 ; x2 = i ;
	}
	if( y1 > y2 )
	{
		i = y1 ; y1 = y2 ; y2 = i ;
	}
	for(;y1 <= y2 ; y1 ++)
		Draw_Line_VRAM(VRAM, x1 , y1 , x2 , y1 ,color );
}

void slide_up(char * string , char * VRAM_A , char * VRAM_B , int speed)//说明：A原来在屏幕中，B在下方，然后B滑道上方，A从上方退出
{
	int j ;
	char buff[3000];
	char * temp = init_VRAM();
	memset( buff , 0 , 3000 );
	PrintChStr( buff , 0 , 0 , string , 15 , 0 );

	for( j = 0 ; j <= 240 ; j += 3 )
	{
		memcpy( temp ,VRAM_A +  SCREEN_BYTES_SIZE - j *160 , j*160  );
		memcpy( temp + j*160 , VRAM_B , SCREEN_BYTES_SIZE - ( j  ) *160  );
		memcpy( temp + SCREEN_BYTES_SIZE - 16 * 160 , buff , 16 * 160 );
		PutDisp_DDVRAM(temp);
		sleep(speed);
	}
	close_VRAM( temp );
}
void slide_down(char * string , char * VRAM_A , char * VRAM_B , int speed)//说明：A原来在屏幕中，B在上方，然后B滑道下方，A从下方退出
{
	int j  ;
	char buff[3000];
	char * temp = init_VRAM();
	memset( buff , 0 , 3000 );
	PrintChStr( buff , 0 , 0 , string , 15 , 0 );

	for( j = 240 ; j >= 0 ; j -= 3 )
	{
		memcpy( temp + j*160 , VRAM_B  , SCREEN_BYTES_SIZE - ( j )*160  );
		memcpy( temp         , VRAM_A  + SCREEN_BYTES_SIZE - j*160 , j*160 );
		memcpy( temp + SCREEN_BYTES_SIZE - 16 * 160 , buff , 16 * 160 );
		PutDisp_DDVRAM(temp);
		sleep(speed);
	}
	close_VRAM( temp );
}

void Draw_Region_VRAM(char * VRAM ,int x1 , int y1 , int x2 , int y2 , int color )
{
	Draw_Line_VRAM( VRAM , x1+6 , y1 , x2-6 , y1 ,color );
	Draw_Line_VRAM( VRAM , x1+6 , y2 , x2-6 , y2 ,color );
	Draw_Line_VRAM( VRAM , x1 , y1+6 , x1 , y2-6 ,color );
	Draw_Line_VRAM( VRAM , x2 , y1+6 , x2 , y2-6 ,color );
	
	DrawPoint_VRAM( VRAM , x1+5 , y1+1 , color );
	DrawPoint_VRAM( VRAM , x1+4 , y1+1 , color );
	DrawPoint_VRAM( VRAM , x1+3 , y1+2 , color );
	DrawPoint_VRAM( VRAM , x1+2 , y1+3 , color );
	DrawPoint_VRAM( VRAM , x1+1 , y1+4 , color );
	DrawPoint_VRAM( VRAM , x1+1 , y1+5 , color );
	
	DrawPoint_VRAM( VRAM , x2-5 , y1+1 , color );
	DrawPoint_VRAM( VRAM , x2-4 , y1+1 , color );
	DrawPoint_VRAM( VRAM , x2-3 , y1+2 , color );
	DrawPoint_VRAM( VRAM , x2-2 , y1+3 , color );
	DrawPoint_VRAM( VRAM , x2-1 , y1+4 , color );
	DrawPoint_VRAM( VRAM , x2-1 , y1+5 , color );
	
	DrawPoint_VRAM( VRAM , x1+5 , y2-1 , color );
	DrawPoint_VRAM( VRAM , x1+4 , y2-1 , color );
	DrawPoint_VRAM( VRAM , x1+3 , y2-2 , color );
	DrawPoint_VRAM( VRAM , x1+2 , y2-3 , color );
	DrawPoint_VRAM( VRAM , x1+1 , y2-4 , color );
	DrawPoint_VRAM( VRAM , x1+1 , y2-5 , color );
	
	DrawPoint_VRAM( VRAM , x2-5 , y2-1 , color );
	DrawPoint_VRAM( VRAM , x2-4 , y2-1 , color );
	DrawPoint_VRAM( VRAM , x2-3 , y2-2 , color );
	DrawPoint_VRAM( VRAM , x2-2 , y2-3 , color );
	DrawPoint_VRAM( VRAM , x2-1 , y2-4 , color );
	DrawPoint_VRAM( VRAM , x2-1 , y2-5 , color );
}
