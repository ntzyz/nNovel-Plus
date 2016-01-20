long * const p_RTC;
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
int leap (int year);
void update_time();