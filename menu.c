#include <os.h>
#include "graph.h"
#include "Chinese.h"
#define MENU_LENGTH 200

void fade_out(char * VRAM)
{
	int i,j;
	for( j = 0 ; j < 2*SCREEN_BYTES_SIZE ; j ++ )
	{
		i = GetPoint_VRAM( VRAM , j%320 , j/320 );
		if( i + 7 <= 15 ) DrawPoint_VRAM( VRAM , j%320 , j/320 , i + 7 );
		else 			  DrawPoint_VRAM( VRAM , j%320 , j/320 ,  15   );
	}
}

void fade_in( char * VRAM )
{
	int j ;
	for( j = 0 ; j < 2*SCREEN_BYTES_SIZE ; j ++ )
		if(GetPoint_VRAM( VRAM , j%320 , j/320 ) == 10) DrawPoint_VRAM( VRAM , j%320 , j/320 , 0 );
}

int menu(char * _VRAM ,int x , int y , char * text , char * explain)
{
	fade_out(_VRAM);
	char * VRAM = init_VRAM();
	memcpy(VRAM,_VRAM,SCREEN_BYTES_SIZE);
	int i , j , _x = x + 3 , _y = y + 2 ;
	int number_of_item = 0 , selected_item = 1 ;
	char temp[ 50 ];
	char exp[50];
	for( i = 0 ; text[i] != '\0' ; i ++)
		if( text[i] == '\n' ) number_of_item++ ; number_of_item-- ;
	AreaClr_VRAM( VRAM ,  x , y , x + MENU_LENGTH + 2, y + 20 * (number_of_item + 1) + 3 );
	Draw_Rect_VRAM( VRAM ,  x , y , x + MENU_LENGTH , y + 20 * (number_of_item + 1) + 1 , 0 );
	Draw_Line_VRAM( VRAM , x + MENU_LENGTH + 1 , y + 1 , x + MENU_LENGTH + 1 , y + 20 * (number_of_item + 1) + 2 , 0 );
	Draw_Line_VRAM( VRAM , x + MENU_LENGTH + 2 , y + 2 , x + 202 , y + 20 * (number_of_item + 1) + 3 , 0 );
	Draw_Line_VRAM( VRAM , x + 1 , y + 20 * (number_of_item + 1) + 2 , x + MENU_LENGTH + 1 , y + 20 * (number_of_item + 1) + 2 , 0 );
	Draw_Line_VRAM( VRAM , x + 2 , y + 20 * (number_of_item + 1) + 3 , x + MENU_LENGTH + 2 , y + 20 * (number_of_item + 1) + 3 , 0 );
	j = 0 ;
	for( i = 0 ; text[i] != '\0' ; i ++ ,j++)
	{
		temp[ j ] = text[ i ];
		if( temp[ j ] == '\n' )
		{
			temp[ j ] = 0;
			PrintChStr( VRAM, _x , _y , temp , 0 , 15 );
			Draw_Line_VRAM( VRAM , _x -3 , _y -2 , _x +MENU_LENGTH - 3 , _y -2 , 0 );
			_y += 20 ;
			j = -1;
		}
	}
	AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) );
	PutDisp_DDVRAM( VRAM );
	wait_no_key_pressed();
	while( 1 )
	{
		if( isKeyPressed(KEY_NSPIRE_UP  ) && selected_item != 1 ) 
		{
			AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) );
			for( j = 1 ; j <= 20 ; j ++ )
			{
				AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 - j , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) - j  );
				PutDisp_DDVRAM( VRAM );
				sleep(1);
				AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 - j , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) - j  );
			}
			selected_item --;
			AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) );
		}
		if( isKeyPressed(KEY_NSPIRE_DOWN) && selected_item !=number_of_item )
		{
			AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) );
			for( j = 0 ; j < 20 ; j ++ )
			{
				AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 + j , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) + j + 1 );
				PutDisp_DDVRAM( VRAM );
				sleep(1);
				AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 + j , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) + j + 1 );
			}
			selected_item ++;
			AreaRev_VRAM( VRAM , x + 1 , y + 20 * selected_item + 1 , x + MENU_LENGTH - 1, y + 20 * ( selected_item + 1 ) );
		}
		
		memset( exp , 0 , 50 );
		j = 0 ;
		for( i = 0 ; i < selected_item ; i ++ )
		{
			for( ; explain[j] != '\n' ; j ++ );
			j++;
		}
		for( i = 0 ; explain[j] != '\n' ; i++ , j++ )
			exp[i] = explain[j] ;
		
		Fill_Rect_VRAM( VRAM , 0 , 224 , 320 , 240 , 0 );
		PrintChStr( VRAM , 0 , 224 , exp , 15 , 0 );
		
		PutDisp_DDVRAM( VRAM );
		
		if( isKeyPressed(KEY_NSPIRE_ESC ) ) { selected_item = 0 ; break; }
		if( isKeyPressed(KEY_NSPIRE_ENTER) || isKeyPressed(KEY_NSPIRE_CLICK) ) break;
		if( !(isKeyPressed(KEY_NSPIRE_UP)||isKeyPressed(KEY_NSPIRE_DOWN)) )
			wait_key_pressed();
		
	}
	close_VRAM(VRAM);
	return selected_item;
}

int num_input(char *_VRAM , int x , int y , char * msg )
{
	fade_out(_VRAM);
	char *VRAM = init_VRAM();
	memcpy(VRAM,_VRAM,SCREEN_BYTES_SIZE);
	
	int ap_value[5] = { 0 , 0 , 0 , 0 , 0 };
	int i,j;
	int right_key_pressed = 0;
	char UP_SELECT[]=		{0x0,0x0,0x4 ,0x0 ,0xE ,0x0 ,0x1F,0x0 ,0x3F,0x80,0x7F,0xC0,0x7F,0xC0,0x7F,0xC0,0x7F,0xC0,0x0 ,0x0};
	char UP[]=				{0x0,0x0,0x4 ,0x0 ,0xA ,0x0 ,0x11,0x0 ,0x20,0x80,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0xC0,0x0 ,0x0};
	char DOWN_SELECT[]=		{0x0,0x0,0x7F,0xC0,0x7F,0xC0,0x7F,0xC0,0x7F,0xC0,0x3F,0x80,0x1F,0x0 ,0xE ,0x0 ,0x4 ,0x0 ,0x0 ,0x0};
	char DOWN[]=			{0x0,0x0,0x7F,0xC0,0x40,0x40,0x40,0x40,0x40,0x40,0x20,0x80,0x11,0x0 ,0xA ,0x0 ,0x4 ,0x0 ,0x0 ,0x0};



	AreaClr_VRAM( VRAM , x , y , x + 152 , y + 82 );
	Draw_Rect_VRAM( VRAM , x , y , x + 150 , y + 80 , 0 );
	Draw_Line_VRAM( VRAM , x+1 , y+81 , x + 151 , y + 81 , 0 );
	Draw_Line_VRAM( VRAM , x+2 , y+82 , x + 152 , y + 82 , 0 );
	Draw_Line_VRAM( VRAM , x+151 , y+1 , x+151 , y+81 , 0 );
	Draw_Line_VRAM( VRAM , x+152 , y+2 , x+152 , y+82 , 0 );
	PrintChStr( VRAM , x + 10 , y + 4 , msg , 0 , 15);
	for( i = 0 ; i < 5 ; i ++ )
	{
		DrawGraph_VRAM( VRAM , x +28 + 20 * i , y + 30 , 11 , 10 , UP , 0 , 15 );
		DrawGraph_VRAM( VRAM , x +28 + 20 * i , y + 56 , 11 , 10 ,DOWN, 0 , 15 );
		DrawAsciiChar_VRAM( VRAM , x +30 + 20 * i , y + 43 , ap_value[i]+'0' , 0 , 15 );
	}
	i = 0 ;
	while(1)
	{
		for( j = 0 ; j < 5 ; j ++ )
		{
			DrawGraph_VRAM( VRAM , x +28 + 20 * j , y + 30 , 11 , 10 , UP , 0 , 15 );
			DrawGraph_VRAM( VRAM , x +28 + 20 * j , y + 56 , 11 , 10 ,DOWN, 0 , 15 );
			DrawAsciiChar_VRAM( VRAM , x +30 + 20 * j , y + 43 , ap_value[j]+'0' , 0 , 15 );
		}
		if(ap_value[i] == -1)
			ap_value[i] = 9;
		DrawGraph_VRAM( VRAM , x +28 + 20 * i , y + 30 , 11 , 10 , UP_SELECT , 0 , 15 );
		DrawGraph_VRAM( VRAM , x +28 + 20 * i , y + 56 , 11 , 10 ,DOWN_SELECT, 0 , 15 );
		DrawAsciiChar_VRAM( VRAM , x +30 + 20 * i , y + 43 , ap_value[i]+'0' , 0 , 15 );
		PutDisp_DDVRAM(VRAM);
		right_key_pressed = 0;
		wait_key_pressed();
		if( isKeyPressed(KEY_NSPIRE_UP) )
		{
			ap_value[i] ++ ;
			if(ap_value[i] == 10)ap_value[i] = 0;
		}
		if( isKeyPressed(KEY_NSPIRE_DOWN) )
		{
			ap_value[i] -- ;
		}
		if( isKeyPressed(KEY_NSPIRE_1) )
		{
			ap_value[i] = 1;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_2) )
		{
			ap_value[i] = 2;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_3) )
		{
			ap_value[i] = 3;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_4) )
		{
			ap_value[i] = 4;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_5) )
		{
			ap_value[i] = 5;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_6) )
		{
			ap_value[i] = 6;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_7) )
		{
			ap_value[i] = 7;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_8) )
		{
			ap_value[i] = 8;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_9) )
		{
			ap_value[i] = 9;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_0) )
		{
			ap_value[i] = 0;
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_RIGHT) )
		{
			right_key_pressed = 1;
		}
		if( isKeyPressed(KEY_NSPIRE_LEFT) )
		{
			DrawGraph_VRAM( VRAM , x +28 + 20 * i , y + 30 , 11 , 10 , UP , 0 , 15 );
			DrawGraph_VRAM( VRAM , x +28 + 20 * i , y + 56 , 11 , 10 ,DOWN, 0 , 15 );
			i --;
			if( i == -1 ) i = 4 ;
		}
		if( right_key_pressed == 1 )
		{
			DrawGraph_VRAM( VRAM , x +28 + 20 * i , y + 30 , 11 , 10 , UP , 0 , 15 );
			DrawGraph_VRAM( VRAM , x +28 + 20 * i , y + 56 , 11 , 10 ,DOWN, 0 , 15 );
			i ++;
			if( i == 5 ) i = 0 ;
			
		}
		if( isKeyPressed(KEY_NSPIRE_ENTER) )return ap_value[0]*10000+ap_value[1]*1000+ap_value[2]*100+ap_value[3]*10+ap_value[4];
		if( isKeyPressed(KEY_NSPIRE_ESC  ) )return -1;
	}	
}
