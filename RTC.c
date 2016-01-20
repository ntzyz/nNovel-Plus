#include <os.h>
#define SECOND_OF_YEAR  31536000
#define SECOND_OF_DAY   86400
#define SECOND_OF_HOUR  3600

long * const p_RTC = (long*)0x90090000;
typedef struct 
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
}TIME;

TIME time;

int leap (int year)//ÅÐ¶ÏÈòÄê
{ 
	if( year % 4 == 0 && year % 100 != 0 || year % 400 == 0 )
		return 1;
	else
		return 0;
}

void update_time()
{
	long seconds = *p_RTC;
	int  year = 1970 ;
	int  day = 1,month = 1;
	int  minute = 0, hour = 0;
	int  day_of_month[]={ 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 };
	year = 1970 ;
	day = 1;
	month = minute = hour = 0;
	seconds = *p_RTC;
	while( seconds >= 0)
	{
		seconds -= SECOND_OF_YEAR  ;
		if(leap(year)) seconds -= SECOND_OF_DAY;
		year ++ ;
	}
	seconds += SECOND_OF_YEAR ;
	year -- ;
	
	while( seconds >=0 )
	{
		seconds -= SECOND_OF_DAY;
		day ++;
	}
	day -- ;
	seconds += SECOND_OF_DAY ;

	while(day>0)
	{
		day-=day_of_month[month];
		month ++;
	}
	day+=day_of_month[month];
	
	while( seconds >=0 )
	{
		seconds -= SECOND_OF_HOUR;
		hour ++;
	}
	seconds += SECOND_OF_HOUR;
	hour--;
	while( seconds >=0 )
	{
		seconds -= 60;
		minute ++;
	}
	seconds += 60;
	minute--;
	
	time.year   = year   ;
	time.month  = month  ;
	time.day    = day    ;
	time.hour   = hour   ;
	time.minute = minute ;
	time.second = seconds ;
}
