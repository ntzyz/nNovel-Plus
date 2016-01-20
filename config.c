#include <os.h>
#include "graph.h"
#include "Chinese.h"
#include "menu.h"

#define CONFIG_PATH "/documents/ndless/nNovel.cfg.tns"

typedef struct
{
	int show_time;
	int show_second;
	int show_page_slide;
	int auto_save;
	int auto_save_time;
	int auto_save_exit;
	char Font_path[50];	
} CONFIG;
CONFIG config;

void load_config()
{
	FILE * cfgdata = fopen( CONFIG_PATH , "rb+" );
	
	if( cfgdata == NULL )
	{
		cfgdata = fopen( CONFIG_PATH , "wb+" );
		fseek( cfgdata , 0 , SEEK_SET );
		config.show_time		= 1 ;
		config.show_second		= 1 ; 
		config.show_page_slide	= 1 ;
		config.auto_save		= 1 ;
		config.auto_save_time	= 20 ;
		config.auto_save_exit	= 1 ;
		strcpy( config.Font_path , "/documents/Fonts/HZK16.nft.tns" );
		fwrite( &config , sizeof(int)*6 + sizeof(char)*50 , 1 , cfgdata );
		fclose( cfgdata );
	}
	else
	{
		fseek( cfgdata , 0 , SEEK_SET );
		fread( &config , sizeof(int)*6 + sizeof(char)*50 , 1 , cfgdata );
		fclose( cfgdata );
	}
}

void setup_config()
{
	FILE * cfgdata = fopen( CONFIG_PATH , "rb+" );
	char menu_title[200],temp[50];
	menu_title[0] = 0 ;
	
	strcat( menu_title , "    ÉèÖÃ\n¸ü»»×ÖÌå          ");

	strcat( menu_title , "\nÊ±¼äÏÔÊ¾          ");
	if(config.show_time == 1)
		strcat( menu_title , "¿ª" );
	else 
		strcat( menu_title , "¹Ø" );
	
	
	strcat( menu_title , "\nÃëÊıÏÔÊ¾          ");
	if(config.show_second == 1)
		strcat( menu_title , "¿ª" );
	else 
		strcat( menu_title , "¹Ø" );
	
	
	strcat( menu_title , "\nÏñËØ·­Ò³Ğ§¹û      ");
	if(config.show_page_slide == 1)
		strcat( menu_title , "¿ª" );
	else 
		strcat( menu_title , "¹Ø" );
	
	
	strcat( menu_title , "\n×Ô¶¯±£´æ          ");
	if(config.auto_save == 1)
		strcat( menu_title , "¿ª" );
	else 
		strcat( menu_title , "¹Ø" );
	
	
	strcat( menu_title , "\n×Ô¶¯±£´æÑ¡Ïî      ");	
	strcat( menu_title , "\n½¨Á¢ÎÄ¼ş¹ØÁª      ");	
	
	strcat( menu_title , "\nÍË³öÊ±×Ô¶¯±£´æ    ");
	if(config.auto_save_exit == 1)
		strcat( menu_title , "¿ª\n" );
	else 
		strcat( menu_title , "¹Ø\n" );	
	
	int i ;
	i = menu( SCREEN_BASE_ADDRESS , 30 , 30 , menu_title , "\nÑ¡ÔñÒ»¸ö×ÖÌåÎÄ¼ş\nÊÇ·ñÔÚÆÁÄ»×óÏÂ·½ÏÔÊ¾Ê±¼ä\nÏÔÊ¾µÄÊ±¼äÊÇ·ñ°üÀ¨ÃëÊı\nµ±Ğ§¹û²»¼ÑÊ±¿ÉÒÔ¹Ø±Õ\nÊÇ·ñ¿ªÆô×Ô¶¯±£´æ\n×Ô¶¯±£´æµÄÆµÂÊ\n¹ØÁªºó¿ÉÖ±½Ó´ò¿ªtxtÎÄ¼şÔÄ¶Á\n³ÌĞòÍË³öÊ±ÊÇ·ñ±£´æ½ø¶È\n" );
	switch(i)
	{
		case 1 :
		{
			if(file_browse("/documents/Fonts",".nft.tns",temp))
			{	
				CloseHZK();
				sprintf(config.Font_path,"/documents/Fonts/%s",temp);
				OpenHZK();
			}
			
			break ;
		}
		case 2 : if( config.show_time == 1 )  				config.show_time = 0 ;			else config.show_time = 1 ;			break;
		case 3 : if( config.show_second == 1 )  			config.show_second = 0 ;		else config.show_second = 1 ;		break;
		case 4 : if( config.show_page_slide == 1 )  		config.show_page_slide = 0 ;	else config.show_page_slide = 1 ;	break;
		case 5 : if( config.auto_save == 1 )  				config.auto_save = 0 ;			else config.auto_save = 1 ;			break;
		case 6 :     config.auto_save_time = num_input( SCREEN_BASE_ADDRESS , 50 , 50 , "µ±Ò³Âë±»Õû³ıÊ±" );			break;
		case 7 : 
		{
			FILE *fp = NULL;
			fp = fopen( "/documents/ndless/ndless.cfg.tns" , "rb+" );
			if( fp == NULL )
			{
				wait_no_key_pressed();
				if(show_msgbox_2b("æç¤º","æœªæ‰¾åˆ°ndless.cfg.tns\næ˜¯å¦è¦å»ºç«‹è¯¥æ–‡ä»¶ï¼Ÿ","æ˜¯","ä¸ï¼Œè°¢è°¢")==1)
				{
					fp = fopen( "/documents/ndless/ndless.cfg.tns" , "wb+" );
				}
				else
					break;
			}
			int cfgsize = 0;
			fseek( fp , 0 , SEEK_END );
			cfgsize = ftell(fp);
			char *pncfg = NULL;
			pncfg = malloc( cfgsize + 50 );
			if(pncfg==NULL)
			{
				wait_no_key_pressed();
				show_msgbox("é”™è¯¯","å†…å­˜ä¸¥é‡ä¸è¶³ï¼Œæ— æ³•ç»§ç»­ï¼");
				fclose( fp );
				break;
			}
			fseek(fp,0,SEEK_SET);
			fread(pncfg,cfgsize,1,fp);
			if( strstr(pncfg,"ext.txt=nNovel") )
			{
				wait_no_key_pressed();
				show_msgbox("æç¤º","æ–‡ä»¶å…³è”å·²å»ºç«‹ï¼Œæ— éœ€é‡å¤æ“ä½œã€‚");
				fclose( fp );
				free(pncfg);
				break;
			}
			else
			{
				strcat(pncfg,"\r\next.txt=nNovel");
				fseek(fp,0,SEEK_SET);
				fwrite(pncfg,strlen(pncfg),1,fp);
				wait_no_key_pressed();
				show_msgbox("æç¤º","æ–‡ä»¶å…³è”å·²å»ºç«‹!");
				fclose( fp );
				free(pncfg);
				break;
			}
		}
		case 8 : if( config.auto_save_exit == 1 )  			config.auto_save_exit = 0 ;		else config.auto_save_exit = 1 ;	break;
		default : break ;
	}
	fseek( cfgdata , 0 , SEEK_SET );
	fwrite( &config , sizeof(int)*6 + sizeof(char)*50 , 1 , cfgdata );
	fclose( cfgdata );
}


