#ifndef _rtc_H
#define _rtc_H	 
#include "sys.h"

//?????
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;	
	
	//??????
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}_calendar;					 
extern _calendar calendar;	//?????

struct SET_ALARM
{
	s8 hour;
	s8 minute;
	u8 week;
};

    
u8 RTC_Init_LSI(void);//???RTC,??0,??;1,??;
u8 RTC_Init_LSE(void);
u8 Is_Leap_Year(u16 year);//??,????
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //????   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//????	
void  get_time(void);
void weekset(void);
#endif
