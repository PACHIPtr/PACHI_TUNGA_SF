/*********************************************************************
* title_name		: Auto Notice System
* date_created		: 2017.11.01
* filename			: AutoNotice.h
* author			: TreeN
* version			: Version 0.0.1
*/

class CAutoNotice : public singleton<CAutoNotice>
{
	enum AutoNoticeSettings
	{
		ENABLE_HOURLY_ANNOUCEMENT = true,
		// set the minute in hour when horuly annoucement will show(00 - 59)
		// it will show every hour at choosen time, ex. if HOURLY_MINUTE = 15 then notice will show at 10:15, 11:15, ... 23:15 etc.
		HOURLY_MINUTE = 15,

		// if you want make some special hour annoucements simply turn this variable to TRUE and set table in SpecialHourAnnoucement() function 
		ENABLE_SPECIAL_HOUR_ANNOUCEMENT = true,

		// if you want make same annoucement in time intervals then turn this variable to TRUE and set tables in TimeIntervalAnnoucement() function
		ENABLE_TIME_INTERVAL_ANNOUCEMENT = true,

		MONDAY = 1,
		TUESDAY = 2,
		WEDNESDAY = 3,
		THURSDAY = 4,
		FRIDAY = 5,
		SATURDAY = 6,
		SUNDAY = 0,
		EVERYDAY = 100,
	};

public:
	CAutoNotice();
	virtual ~CAutoNotice();
	void Check(int day, int hour, int minute, int second);

private:
	void SendNoticeLine(const char * format, ...);
	void HourlyAnnouncement(int minute, int second);
	void SpecialHourAnnoucement(int day, int hour, int minute, int second);
	void TimeIntervalAnnoucement(int day, int hour, int minute, int second);

};