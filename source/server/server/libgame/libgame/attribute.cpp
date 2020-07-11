#include "../../libthecore/libthecore/stdafx.h"
#include "attribute.h"

#define SET_BIT(var,bit)		((var) |= (bit))
#define REMOVE_BIT(var,bit)		((var) &= ~(bit))

void CAttribute::Initialize(DWORD width, DWORD height)
{
	dataType = D_DWORD;
	defaultAttr = 0;
	this->width = width;
	this->height = height;
	data = NULL;
	bytePtr = NULL;
	wordPtr = NULL;
	dwordPtr = NULL;
}

void CAttribute::Alloc()
{
	size_t memSize;

	switch (dataType)
	{
	case D_DWORD:
		memSize = width * height * sizeof(DWORD);
		break;

	case D_WORD:
		memSize = width * height * sizeof(WORD);
		break;

	case D_BYTE:
		memSize = width * height;
		break;

	default:
		assert(!"dataType error!");
		return;
	}
	data = malloc(memSize);

	switch (dataType)
	{
	case D_DWORD:
		dwordPtr = new DWORD * [height];
		dwordPtr[0] = (DWORD*)data;

		for (DWORD y = 1; y < height; ++y)
		{
			dwordPtr[y] = dwordPtr[y - 1] + width;
		}

		for (DWORD y = 0; y < height; ++y)
			for (DWORD x = 0; x < width; ++x)
			{
				dwordPtr[y][x] = defaultAttr;
			}

		break;

	case D_WORD:
		wordPtr = new WORD * [height];
		wordPtr[0] = (WORD*)data;

		for (DWORD y = 1; y < height; ++y)
		{
			wordPtr[y] = wordPtr[y - 1] + width;
		}

		for (DWORD y = 0; y < height; ++y)
			for (DWORD x = 0; x < width; ++x)
			{
				wordPtr[y][x] = defaultAttr;
			}

		break;

	case D_BYTE:
		bytePtr = new BYTE * [height];
		bytePtr[0] = (BYTE*)data;

		for (DWORD y = 1; y < height; ++y)
		{
			bytePtr[y] = bytePtr[y - 1] + width;
		}

		for (DWORD y = 0; y < height; ++y)
			for (DWORD x = 0; x < width; ++x)
			{
				bytePtr[y][x] = defaultAttr;
			}

		break;
	}
}

CAttribute::CAttribute(DWORD width, DWORD height)
{
	Initialize(width, height);
	Alloc();
}

CAttribute::CAttribute(DWORD* attr, DWORD width, DWORD height)
{
	Initialize(width, height);

	int i;
	int size = width * height;

	for (i = 0; i < size; ++i)
		if (attr[0] != attr[i])
		{
			break;
		}

	if (i == size)
	{
		defaultAttr = attr[0];
	}
	else
	{
		int allAttr = 0;

		for (i = 0; i < size; ++i)
		{
			allAttr |= attr[i];
		}

		if (!(allAttr & 0xffffff00))
		{
			dataType = D_BYTE;
		}
		else if (!(allAttr & 0xffff0000))
		{
			dataType = D_WORD;
		}
		else
		{
			dataType = D_DWORD;
		}

		Alloc();

		if (dataType == D_DWORD)
		{
			thecore_memcpy(data, attr, sizeof(DWORD) * width * height);
		}
		else
		{
			DWORD* pdw = (DWORD*)attr;

			if (dataType == D_BYTE)
			{
				for (DWORD y = 0; y < height; ++y)
					for (DWORD x = 0; x < width; ++x)
					{
						bytePtr[y][x] = *(pdw++);
					}
			}
			else if (dataType == D_WORD)
			{
				for (DWORD y = 0; y < height; ++y)
					for (DWORD x = 0; x < width; ++x)
					{
						wordPtr[y][x] = *(pdw++);
					}
			}
		}
	}
}

CAttribute::~CAttribute()
{
	if (data)
	{
		free(data);
	}

	if (bytePtr)
	{
		delete[] bytePtr;
	}

	if (wordPtr)
	{
		delete[] wordPtr;
	}

	if (dwordPtr)
	{
		delete[] dwordPtr;
	}
}

int CAttribute::GetDataType()
{
	return dataType;
}

void* CAttribute::GetDataPtr()
{
	return data;
}

void CAttribute::Set(DWORD x, DWORD y, DWORD attr)
{
	if (x > width || y > height)
	{
		return;
	}

	if (!data)
	{
		Alloc();
	}

	if (bytePtr)
	{
		SET_BIT(bytePtr[y][x], attr);
		return;
	}

	if (wordPtr)
	{
		SET_BIT(wordPtr[y][x], attr);
		return;
	}

	SET_BIT(dwordPtr[y][x], attr);
}

void CAttribute::Remove(DWORD x, DWORD y, DWORD attr)
{
	if (x > width || y > height)
	{
		return;
	}

	if (!data)
	{
		return;
	}

	if (bytePtr)
	{
		REMOVE_BIT(bytePtr[y][x], attr);
		return;
	}

	if (wordPtr)
	{
		REMOVE_BIT(wordPtr[y][x], attr);
		return;
	}

	REMOVE_BIT(dwordPtr[y][x], attr);
}

DWORD CAttribute::Get(DWORD x, DWORD y)
{
	if (x > width || y > height)
	{
		return 0;
	}

	if (!data)
	{
		return defaultAttr;
	}

	if (bytePtr)
	{
		return bytePtr[y][x];
	}

	if (wordPtr)
	{
		return wordPtr[y][x];
	}

	return dwordPtr[y][x];
}

void CAttribute::CopyRow(DWORD y, DWORD* row)
{
	if (!data)
	{
		for (DWORD x = 0; x < width; ++x)
		{
			row[x] = defaultAttr;
		}

		return;
	}

	if (dwordPtr)
	{
		thecore_memcpy(row, dwordPtr[y], sizeof(DWORD) * width);
	}
	else
	{
		if (bytePtr)
		{
			for (DWORD x = 0; x < width; ++x)
			{
				row[x] = bytePtr[y][x];
			}
		}
		else if (wordPtr)
		{
			for (DWORD x = 0; x < width; ++x)
			{
				row[x] = wordPtr[y][x];
			}
		}
	}
}