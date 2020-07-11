#ifndef __INC_LIBTHECORE_EVENT_H__
#define __INC_LIBTHECORE_EVENT_H__

#include <boost/intrusive_ptr.hpp>
#define event_create(func, info, when) event_create_ex(func, info, when)
#define EVENTFUNC(name) long (name) (LPEVENT event,long processing_time)
#define EVENTINFO(name) struct name:public event_info_data

struct event_info_data
{
	event_info_data() {}
	virtual ~event_info_data() {}
};

typedef struct event EVENT;
typedef boost::intrusive_ptr<EVENT> LPEVENT;
typedef long (*TEVENTFUNC) (LPEVENT event, long processing_time);

struct TQueueElement;
struct event
{
	event() : func(nullptr), info(nullptr), q_el(nullptr), ref_count(0) {}
	~event()
	{
		if (info != nullptr)
		{
			M2_DELETE(info);
		}
	}

	TEVENTFUNC func;
	event_info_data* info;
	TQueueElement* q_el;
	char is_force_to_end;
	char is_processing;

	size_t ref_count;
};

extern void intrusive_ptr_add_ref(EVENT* p);
extern void intrusive_ptr_release(EVENT* p);

template<class T>T* AllocEventInfo()
{
	return M2_NEW T;
}

extern void event_destroy();
extern int event_process(int pulse);
extern int event_count();

extern LPEVENT event_create_ex(TEVENTFUNC func, event_info_data* info, long when);
extern void event_cancel(LPEVENT* event);
extern long event_processing_time(LPEVENT event);
extern long event_time(LPEVENT event);
extern void event_reset_time(LPEVENT event, long when);
extern void event_set_verbose(int level);

extern event_info_data* FindEventInfo(DWORD dwID);
extern event_info_data* event_info(LPEVENT event);

#endif
