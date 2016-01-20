#include <os.h>
#include "MainRun.h"
#include "Chinese.h"
#include "graph.h"
#include "menu.h"
#include "config.h"

#define SCR_WIDTH		320
#define SCR_HEIGHT		240
#define SCR_TOP			16

#define FILE_NAME_DISP_MAX	36

#define OUT_OF_PAGE		-1
#define OUT_OF_MEMORY	-2


char fname[50] ;

int dirlist(const char* path,const char* pattern, char** result,const char * filter)
{
	unsigned char StructDstat[512];
	chdir((char *)path);
	if (NU_Get_First((struct dstat*)StructDstat, pattern))
	{
		NU_Done((struct dstat*)StructDstat);
		return -1;
	}
	int i = 0;
	do
	{
		char* dirname,*fname;
		fname = (char*) &StructDstat[13];
		if (strstr(fname,filter))
		{
			dirname = (char*) malloc(128);
			strcpy(dirname, (char*) fname);
			result[i] = dirname;
			i++;
		}
	} while (NU_Get_Next((struct dstat*)StructDstat) == 0); 
	NU_Done((struct dstat*)StructDstat);
	return i;
}

BOOL file_browse (const char * path,const char * filter,char * filename)
{
	char * VRAM = init_VRAM();
	AllClr_VRAM(VRAM);
	
	int			numfiles,i,top,bottom,index = 0;
	const int	h_max = SCR_HEIGHT/16-3;
	char *		filenames[512];
	BOOL		refresh = TRUE,retvalue;
	// get file names
	numfiles = dirlist(path,"*.*",filenames,filter);

	if (numfiles<=0)
	{
		PrintChStr(VRAM,0,0,"没有此类文件！\r\n请将所有的文本文件(*.txt.tns)放在文件夹nNovel中，将所有的字体文件(*.nft.tns)放在文件夹Fonts中，然后重试",0 , 15);
		PutDisp_DDVRAM(VRAM);
		wait_key_pressed();
		return FALSE;
	}
	//
	top = 0;bottom = h_max;
	while(TRUE)
	{
		if (refresh)
		{
			refresh = FALSE;
			int y;
			
			AllClr_VRAM(VRAM);
			
			if (bottom-top>h_max)bottom = top + h_max;
			if(top>index)
			{
				top = index;
				bottom = index + h_max;
			}
			if (index>bottom)
			{
				bottom = index;
				top = bottom - h_max;
			}
			if(bottom>numfiles - 1)bottom = numfiles - 1;
			Fill_Rect_VRAM(VRAM , 0 , 0 , 320 , 15 , 0 );
			Fill_Rect_VRAM(VRAM , 0 , 224 , 320 , 240 , 0 );
			PrintChStr(VRAM,0,0,					"              选择文件打开              ",15,0);
			PrintChStr(VRAM,0,224,				"选择Enter                        取消ESC",15,0);
			for (i=top;i<=bottom;++i)
			{
				y = i-top+1;
				if (strlen(filenames[i]) > FILE_NAME_DISP_MAX)
				{
					char s[128];
					strcpy(strcpy(s,filenames[i])+FILE_NAME_DISP_MAX-3,"...");
					if(i==index)
					{
						Fill_Rect_VRAM(VRAM, 0 , y * 16 , 320 , (y+1) * 16 -1, 0 );
						PrintChStr(VRAM,0,y*16,s,15,0);
					}
					else
						PrintChStr(VRAM,0,y*16,s,0,15);
				}
				else
					if(i==index)
					{
						Fill_Rect_VRAM(VRAM, 0 , y * 16 , 320 , (y+1) * 16 -1, 0 );
						PrintChStr(VRAM,0,y*16,filenames[i],15,0);
					}
					else
						PrintChStr(VRAM,0,y*16,filenames[i],0,15);
			}
			PutDisp_DDVRAM(VRAM);
		}
		wait_key_pressed();
		if (isKeyPressed(KEY_NSPIRE_UP)) 
			{if(--index<0) index = numfiles - 1;refresh = TRUE;}
		else if (isKeyPressed(KEY_NSPIRE_DOWN)) 
			{if(++index>numfiles - 1) index = 0;refresh = TRUE;}
		else if (isKeyPressed(KEY_NSPIRE_ESC))
		{
			retvalue = FALSE;
			break;
		}
		else if (isKeyPressed(KEY_NSPIRE_ENTER)||isKeyPressed(KEY_NSPIRE_CLICK))
		{
			retvalue = TRUE;
			break;
		}
	}
	close_VRAM(VRAM);
	strcpy(filename,filenames[index]);
	// clean up
	for(i=0;i<numfiles;i++)
	{
		free(filenames[i]);
	}
	return retvalue;
}

int main(int argc, char* argv[]) {
	lcd_ingray();
	load_config();	
	OpenHZK();	
	clrscr();

	if( argc == 1)
	{
		if( file_browse("/documents/nNovel",".txt.tns",fname))
			Read();
	}
	if( argc == 2)
	{
		strcpy(fname,argv[1]);
		Read();
	}
	CloseHZK();
	return 0;
}

/*
int main ( void )
{
	lcd_ingray();
	DrawAsciiChar_Gray_VRAM( SCREEN_BASE_ADDRESS , 0 , 0 , 16 , 8 , 'a' , 0 );
	wait_key_pressed();
	return 0;
}*/