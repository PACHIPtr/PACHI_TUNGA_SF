#pragma once
#ifdef ENABLE_WHISPER_RENEWAL
class CPythonWhisper : public CSingleton<CPythonWhisper>
{
	public:
		CPythonWhisper() { Clear(); };
		virtual ~CPythonWhisper() { Clear(); };
		enum { TARGET, ME };
		void	Clear() 								{for (int i = TARGET; i <= ME; i++) b_writing[i].clear();};		
		void	AddName(std::string name, BYTE i) 	 	{if (std::find(b_writing[i].begin(), b_writing[i].end(), name) == b_writing[i].end()) b_writing[i].emplace_back(name);};
		void	DeleteName(std::string name, BYTE i) 	{if (std::find(b_writing[i].begin(), b_writing[i].end(), name) != b_writing[i].end()) b_writing[i].remove(name);};
		bool	CheckName(std::string name, BYTE i) 	{return(std::find(b_writing[i].begin(), b_writing[i].end(), name) != b_writing[i].end());};
	protected:
		std::list<std::string> b_writing[ME+1];
};
#endif
