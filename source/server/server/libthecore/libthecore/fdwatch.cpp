#include "stdafx.h"

LPFDWATCH fdwatch_new(int nfiles)
{
	LPFDWATCH fdw;
	int kq;

	kq = kqueue();

	if (kq == -1)
	{
		sys_err("%s", strerror(errno));
		return NULL;
	}

	CREATE(fdw, FDWATCH, 1);

	fdw->kq = kq;
	fdw->nfiles = nfiles;
	fdw->nkqevents = 0;

	CREATE(fdw->kqevents, KEVENT, nfiles * 2);
	CREATE(fdw->kqrevents, KEVENT, nfiles * 2);
	CREATE(fdw->fd_event_idx, int, nfiles);
	CREATE(fdw->fd_rw, int, nfiles);
	CREATE(fdw->fd_data, void*, nfiles);

	return (fdw);
}

void fdwatch_delete(LPFDWATCH fdw)
{
	free(fdw->fd_data);
	free(fdw->fd_rw);
	free(fdw->kqevents);
	free(fdw->kqrevents);
	free(fdw->fd_event_idx);
	free(fdw);
}

int fdwatch(LPFDWATCH fdw, struct timeval* timeout)
{
	int	i, r;
	struct timespec ts;

	if (fdw->nkqevents)
	{
		sys_log(2, "fdwatch: nkqevents %d", fdw->nkqevents);
	}

	if (!timeout)
	{
		ts.tv_sec = 0;
		ts.tv_nsec = 0;

		r = kevent(fdw->kq, fdw->kqevents, fdw->nkqevents, fdw->kqrevents, fdw->nfiles, &ts);
	}
	else
	{
		ts.tv_sec = timeout->tv_sec;
		ts.tv_nsec = timeout->tv_usec;

		r = kevent(fdw->kq, fdw->kqevents, fdw->nkqevents, fdw->kqrevents, fdw->nfiles, &ts);
	}

	fdw->nkqevents = 0;

	if (r == -1)
	{
		return -1;
	}

	memset(fdw->fd_event_idx, 0, sizeof(int) * fdw->nfiles);

	for (i = 0; i < r; i++)
	{
		int fd = fdw->kqrevents[i].ident;

		if (fd >= fdw->nfiles)
		{
			sys_err("ident overflow %d nfiles: %d", fdw->kqrevents[i].ident, fdw->nfiles);
		}
		else
		{
			if (fdw->kqrevents[i].filter == EVFILT_WRITE)
			{
				fdw->fd_event_idx[fd] = i;
			}
		}
	}

	return (r);
}

void fdwatch_register(LPFDWATCH fdw, int flag, int fd, int rw)
{
	if (flag == EV_DELETE)
	{
		if (fdw->fd_rw[fd] & FDW_READ)
		{
			fdw->kqevents[fdw->nkqevents].ident = fd;
			fdw->kqevents[fdw->nkqevents].flags = flag;
			fdw->kqevents[fdw->nkqevents].filter = EVFILT_READ;
			++fdw->nkqevents;
		}

		if (fdw->fd_rw[fd] & FDW_WRITE)
		{
			fdw->kqevents[fdw->nkqevents].ident = fd;
			fdw->kqevents[fdw->nkqevents].flags = flag;
			fdw->kqevents[fdw->nkqevents].filter = EVFILT_WRITE;
			++fdw->nkqevents;
		}
	}
	else
	{
		fdw->kqevents[fdw->nkqevents].ident = fd;
		fdw->kqevents[fdw->nkqevents].flags = flag;
		fdw->kqevents[fdw->nkqevents].filter = (rw == FDW_READ) ? EVFILT_READ : EVFILT_WRITE;

		++fdw->nkqevents;
	}
}

void fdwatch_clear_fd(LPFDWATCH fdw, socket_t fd)
{
	fdw->fd_data[fd] = NULL;
	fdw->fd_rw[fd] = 0;
}

void fdwatch_add_fd(LPFDWATCH fdw, socket_t fd, void* client_data, int rw, int oneshot)
{
	int flag;

	if (fd >= fdw->nfiles)
	{
		sys_err("fd overflow %d", fd);
		return;
	}

	if (fdw->fd_rw[fd] & rw)
	{
		return;
	}

	fdw->fd_rw[fd] |= rw;
	sys_log(2, "FDWATCH_REGISTER fdw %p fd %d rw %d data %p", fdw, fd, rw, client_data);

	if (!oneshot)
	{
		flag = EV_ADD;
	}
	else
	{
		sys_log(2, "ADD ONESHOT fd_rw %d", fdw->fd_rw[fd]);
		flag = EV_ADD | EV_ONESHOT;
		fdw->fd_rw[fd] |= FDW_WRITE_ONESHOT;
	}

	fdw->fd_data[fd] = client_data;
	fdwatch_register(fdw, flag, fd, rw);
}

void fdwatch_del_fd(LPFDWATCH fdw, socket_t fd)
{
	fdwatch_register(fdw, EV_DELETE, fd, 0);
	fdwatch_clear_fd(fdw, fd);
}

void fdwatch_clear_event(LPFDWATCH fdw, socket_t fd, unsigned int event_idx)
{
	assert(event_idx < fdw->nfiles * 2);

	if (fdw->kqrevents[event_idx].ident != fd)
	{
		return;
	}

	fdw->kqrevents[event_idx].ident = 0;
}

int fdwatch_check_event(LPFDWATCH fdw, socket_t fd, unsigned int event_idx)
{
	assert(event_idx < fdw->nfiles * 2);

	if (fdw->kqrevents[event_idx].ident != fd)
	{
		return 0;
	}

	if (fdw->kqrevents[event_idx].flags & EV_ERROR)
	{
		return FDW_EOF;
	}

	if (fdw->kqrevents[event_idx].flags & EV_EOF)
	{
		return FDW_EOF;
	}

	if (fdw->kqrevents[event_idx].filter == EVFILT_READ)
	{
		if (fdw->fd_rw[fd] & FDW_READ)
		{
			return FDW_READ;
		}
	}
	else if (fdw->kqrevents[event_idx].filter == EVFILT_WRITE)
	{
		if (fdw->fd_rw[fd] & FDW_WRITE)
		{
			if (fdw->fd_rw[fd] & FDW_WRITE_ONESHOT)
			{
				fdw->fd_rw[fd] &= ~FDW_WRITE;
			}

			return FDW_WRITE;
		}
	}
	else
	{
		sys_err("fdwatch_check_event: Unknown filter %d (descriptor %d)", fdw->kqrevents[event_idx].filter, fd);
	}

	return 0;
}

int fdwatch_get_ident(LPFDWATCH fdw, unsigned int event_idx)
{
	assert(event_idx < fdw->nfiles * 2);
	return fdw->kqrevents[event_idx].ident;
}

int fdwatch_get_buffer_size(LPFDWATCH fdw, socket_t fd)
{
	int event_idx = fdw->fd_event_idx[fd];

	if (fdw->kqrevents[event_idx].filter == EVFILT_WRITE)
	{
		return fdw->kqrevents[event_idx].data;
	}

	return 0;
}

void* fdwatch_get_client_data(LPFDWATCH fdw, unsigned int event_idx)
{
	int fd;

	assert(event_idx < fdw->nfiles * 2);

	fd = fdw->kqrevents[event_idx].ident;

	if (fd >= fdw->nfiles)
	{
		return NULL;
	}

	return (fdw->fd_data[fd]);
}