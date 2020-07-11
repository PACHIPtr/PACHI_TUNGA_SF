/*********************************************************************
* title_name		: Auto Notice System
* date_created		: 2017.11.01
* filename			: AutoNotice.cpp
* author			: TreeN
* version			: Version 0.0.1
*/

#include "stdafx.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "utils.h"
#include "config.h"
#include "constants.h"
#include "char.h"

#ifdef ENABLE_AUTO_NOTICE
#include "AutoNotice.h"

CAutoNotice::CAutoNotice()
{
}

CAutoNotice::~CAutoNotice()
{
}

void CAutoNotice::SendNoticeLine(const char * format, ...)
{
	if (!format)
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	va_list args;

	va_start(args, format);
	vsnprintf(chatbuf, sizeof(chatbuf), format, args);
	va_end(args);

	SendNotice(chatbuf);
}

void CAutoNotice::Check(int day, int hour, int minute, int second)
{
	HourlyAnnouncement(minute, second);
	//SpecialHourAnnoucement(day, hour, minute, second);
	//TimeIntervalAnnoucement(day, hour, minute, second);
}

void CAutoNotice::HourlyAnnouncement(int minute, int second)
{
	if ((bool)AutoNoticeSettings::ENABLE_HOURLY_ANNOUCEMENT == false)
		return;

	if (second != 00)
		return;

	if ((int)AutoNoticeSettings::HOURLY_MINUTE == minute)
	{
		// to create new line of message simply add new SendNoticeLine function here

		SendNoticeLine(LC_TEXT("Don't feed the yangsellers. Buying from the bots damage the"));
		SendNoticeLine(LC_TEXT("server economy, spoils gameplay and you will end your game banned for it!"));
		SendNoticeLine(LC_TEXT("Visit our webpages! www.servername.com | ts3.servername.com | forum.servername.com "));
	}
}

void CAutoNotice::SpecialHourAnnoucement(int day, int hour, int minute, int second)
{
	if ((bool)AutoNoticeSettings::ENABLE_SPECIAL_HOUR_ANNOUCEMENT == false)
		return;

	if (second != 00)
		return;

	// when you add new hours to first table(or delete) please add(or delete) messages from szpNoticeText. 
	/*
		MONDAY = 1,
		TUESDAY = 2,
		WEDNESDAY = 3,
		THURSDAY = 4,
		FRIDAY = 5,
		SATURDAY = 6,
		SUNDAY = 0,

		EVERYDAY = 100,
	*/
	// if you add new notices please fix the table size
	// example of showing notice
	// notice from szpNoticeHours[0] will appear in Monday at 19:00 and shows 2 lines 
	int szpNoticeHours[][3] = 
	{
		{ 19, 00, MONDAY },
		{ 20, 00, WEDNESDAY },
		{ 21, 00, EVERYDAY }
	};

	// maximum 5 lines of notice at once, if you set "" then this notice line not appear
	const int MAX_NOTICE_LINE = 5;
	char* szpNoticeText[][MAX_NOTICE_LINE] =
	{
		{ "Today from 19:00 to 19:30 we have sales in our ItemShop", "Open Ingame Itemshop and check new prices!", "", "", "" },						//szpNoticeHours[0]
		{ "Hmm did you saw a cheater? ", "Yes? Fantastic! Please open Ticket Menager", "and send a ticket to us", "Thank you for your help.", "" },		//szpNoticeHours[1]
		{ "The best server ", "of 2017", "is ServerName", "visit our web pages", "www.servername.com | www.forum.servername.com" }						//szpNoticeHours[2]
	};

	for (int i = 0; i < _countof(szpNoticeHours); i++)
	{
		if ((hour == szpNoticeHours[i][0] && minute == szpNoticeHours[i][1]))
		{
			if (szpNoticeHours[i][2] != EVERYDAY)
				if (day != szpNoticeHours[i][2])
					continue;

			for (int j = 0; j < MAX_NOTICE_LINE; j++)
			{
				if (szpNoticeText[i][j] == "")
					continue;

				SendNoticeLine(szpNoticeText[i][j]);
			}
		}
	}
}

void CAutoNotice::TimeIntervalAnnoucement(int day, int hour, int minute, int second)
{
	if ((bool)AutoNoticeSettings::ENABLE_TIME_INTERVAL_ANNOUCEMENT == false)
		return;

	if (second != 00)
		return;

	// when you add new hours to first table(or delete) please add(or delete) messages from szpNoticeText. 
	/*
	MONDAY = 1,
	TUESDAY = 2,
	WEDNESDAY = 3,
	THURSDAY = 4,
	FRIDAY = 5,
	SATURDAY = 6,
	SUNDAY = 0,

	EVERYDAY = 100,
	*/
	// if you add new notices please fix the table size
	// this notices shows every 20 minutes
	// example of showing notice
	// notice from szpNoticeHours[0] will appear in Monday at 19:00 and 19:20 and shows 2 lines 
	int szpNoticeHours[][5] =
	{
		{ 19, 00,		19, 30,		MONDAY },
		{ 20, 00,		20, 45,		TUESDAY },
		{ 21, 00,		23, 30,		EVERYDAY },
	};

	// maximum 5 lines of notice at once, if you set "" then notice not appear
	const int MAX_NOTICE_LINE = 5;
	char* szpNoticeText[][MAX_NOTICE_LINE] =
	{
		{ "Special Time Notice 1.1", "Special Time Notice 1.2", "", "", "" },						//szpNoticeHours[0]
		{ "Special Time Notice 2.1", "Special Time Notice 2.2", "Special Time Notice 2.3", "Special Time Notice 2.4", "" },	//szpNoticeHours[1]
		{ "Special Time Notice 3.1", "Special Time Notice 3.2", "Special Time Notice 3.3", "Special Time Notice 3.4", "Special Time Notice 3.5 | Special Time Notice 3.5" }						//szpNoticeHours[2]
	};

	for (int i = 0; i < _countof(szpNoticeHours); i++)
	{
		if ((hour >= szpNoticeHours[i][0] && minute >= szpNoticeHours[i][1]) && (hour <= szpNoticeHours[i][2] && minute <= szpNoticeHours[i][3])
			&& (minute == 00 || minute == 20 || minute == 40))
		{
			if (szpNoticeHours[i][4] != EVERYDAY && day != szpNoticeHours[i][4])
				continue;

			for (int j = 0; j < MAX_NOTICE_LINE; j++)
			{
				if (szpNoticeText[i][j] == "")
					continue;

				SendNoticeLine(szpNoticeText[i][j]);
			}
		}
	}
}

#endif