enum
{
	TEXT_TAG_PLAIN,
	TEXT_TAG_TAG, // ||
	TEXT_TAG_COLOR, // |cffffffff
	TEXT_TAG_HYPERLINK_START, // |H
	TEXT_TAG_HYPERLINK_END, // |h ex) |Hitem:1234:1:1:1|h
	TEXT_TAG_RESTORE_COLOR,
#ifdef ENABLE_EMOJI_IN_TEXTLINE
	TEXT_TAG_EMOJI_START, // |E
	TEXT_TAG_EMOJI_END, // |e ex) |Epath/filename|h?
#endif
};

extern int GetTextTag(const wchar_t * src, int maxLen, int & tagLen, std::wstring & extraInfo);
extern std::wstring GetTextTagOutputString(const wchar_t * src, int src_len);
extern int GetTextTagOutputLen(const wchar_t * src, int len);
extern int FindColorTagEndPosition(const wchar_t * src, int src_len);
extern int FindColorTagStartPosition(const wchar_t * src, int src_len);
extern int GetTextTagInternalPosFromRenderPos(const wchar_t * src, int src_len, int offset);
