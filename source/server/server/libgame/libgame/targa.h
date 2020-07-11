#pragma once

#pragma pack(1)
struct TGA_HEADER
{
	char idLen;
	char palType;
	char imgType;
	WORD colorBegin;
	WORD colorCount;
	char palEntrySize;
	WORD left;
	WORD top;
	WORD width;
	WORD height;
	char colorBits;
	char desc;
};
#pragma pack()

class CTargaImage
{
public:
	CTargaImage();
	~CTargaImage();

	void Create(int x, int y);
	char* GetBasePointer(int line = 0);
	bool Save(const char* filename);

protected:
	TGA_HEADER m_header;
	char* m_pbuf;
	int m_x, m_y;
};