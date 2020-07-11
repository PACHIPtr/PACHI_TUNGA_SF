#pragma once

typedef struct buffer BUFFER;
typedef struct buffer* LPBUFFER;

struct buffer
{
	struct buffer* next;

	char* write_point;
	int write_point_pos;

	const char* read_point;
	int length;

	char* mem_data;
	int mem_size;

	long flag;
};

extern LPBUFFER buffer_new(int size);
extern void buffer_delete(LPBUFFER buffer);
extern void buffer_reset(LPBUFFER buffer);

extern DWORD buffer_size(LPBUFFER buffer);
extern int buffer_has_space(LPBUFFER buffer);

extern void buffer_write(LPBUFFER& buffer, const void* src, int length);
extern void buffer_read(LPBUFFER buffer, void* buf, int bytes);
extern BYTE buffer_get_byte(LPBUFFER buffer);
extern WORD buffer_get_word(LPBUFFER buffer);
extern DWORD buffer_get_dword(LPBUFFER buffer);

extern const void* buffer_read_peek(LPBUFFER buffer);
extern void buffer_read_proceed(LPBUFFER buffer, int length);

extern void* buffer_write_peek(LPBUFFER buffer);
extern void buffer_write_proceed(LPBUFFER buffer, int length);

extern void buffer_adjust_size(LPBUFFER& buffer, int add_size);
extern void safe_buffer_delete(LPBUFFER& buf);