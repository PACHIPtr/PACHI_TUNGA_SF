#pragma once

class CPythonSystem : public CSingleton<CPythonSystem>
{
	public:
		enum EWindow
		{
			WINDOW_STATUS,
			WINDOW_INVENTORY,
			WINDOW_ABILITY,
			WINDOW_SOCIETY,
			WINDOW_JOURNAL,
			WINDOW_COMMAND,

			WINDOW_QUICK,
			WINDOW_GAUGE,
			WINDOW_MINIMAP,
			WINDOW_CHAT,

			WINDOW_MAX_NUM,
		};

		enum ESystem
		{
			FREQUENCY_MAX_NUM  = 30,
			RESOLUTION_MAX_NUM = 100
		};

		typedef struct SResolution
		{
			DWORD	width;
			DWORD	height;
			DWORD	bpp;		// bits per pixel (high-color = 16bpp, true-color = 32bpp)

			DWORD	frequency[20];
			BYTE	frequency_count;
		} TResolution;

		typedef struct SWindowStatus
		{
			int		isVisible;
			int		isMinimized;

			int		ixPosition;
			int		iyPosition;
			int		iHeight;
		} TWindowStatus;

		typedef struct SConfig
		{
			DWORD			width;
			DWORD			height;
			DWORD			bpp;
			DWORD			frequency;

			bool			is_software_cursor;
			bool			is_object_culling;
			int				iDistance;
			int				iShadowLevel;
#ifdef ENABLE_SHOPNAMES_RANGE
			float			shopnames_range;
#endif
			
			FLOAT			music_volume;
			BYTE			voice_volume;

			int				gamma;

			int				isSaveID;
			char			SaveID[20];

			bool			bWindowed;
			bool			bDecompressDDS;
			bool			bNoSoundCard;
			bool			bUseDefaultIME;
			BYTE			bSoftwareTiling;
			bool			bViewChat;
			int				bAlwaysShowName;
			bool			bShowDamage;
			bool			bShowSalesText;
			bool			iShowOfflineShop;
#ifdef ENABLE_FOG_FIX
			bool			bFogMode;
#endif
			bool			bHidePets;
			bool			bHideMounts;
			bool			bHideShamans;
#if defined(WJ_SHOW_MOB_INFO)
			bool			bShowMobLevel;
			bool			bShowMobAIFlag;
#endif
			bool			bShowMoneyLog;
			bool			bShowDiceInfo;
#ifdef ENABLE_SHOW_NIGHT_SYSTEM
			bool			bNightMode;
#endif
			bool			bAutoPickUp;
			bool			bSnowTexturesMode;
			bool			bDesertTexturesMode;
			bool			bEnableSnow;
#if defined(ENABLE_GRAPHIC_MASK)
			bool			bGraphicMaskObject;
			bool			bGraphicMaskCloud;
			bool			bGraphicMaskWater;
			bool			bGraphicMaskTree;
#endif
			bool			bItemHighlight;
		} TConfig;

	public:
		CPythonSystem();
		virtual ~CPythonSystem();

		void Clear();
		void SetInterfaceHandler(PyObject * poHandler);
		void DestroyInterfaceHandler();

		// Config
		void							SetDefaultConfig();
		bool							LoadConfig();
		bool							SaveConfig();
		void							ApplyConfig();
		void							SetConfig(TConfig * set_config);
		TConfig *						GetConfig();
		void							ChangeSystem();

		// Interface
		bool							LoadInterfaceStatus();
		void							SaveInterfaceStatus();
		bool							isInterfaceConfig();
		const TWindowStatus &			GetWindowStatusReference(int iIndex);

		DWORD							GetWidth();
		DWORD							GetHeight();
		DWORD							GetBPP();
		DWORD							GetFrequency();
		bool							IsSoftwareCursor();
		bool							IsWindowed();
		bool							IsViewChat();
		int								IsAlwaysShowName();
		bool							IsShowDamage();
		bool							IsShowSalesText();
		bool							IsUseDefaultIME();
		bool							IsNoSoundCard();
		bool							IsAutoTiling();
		bool							IsSoftwareTiling();
		bool							IsHidePets()										{ return m_Config.bHidePets; }
		bool							IsHideMounts()										{ return m_Config.bHideMounts; }
		bool							IsHideShamans()										{ return m_Config.bHideShamans; }
		void							SetSoftwareTiling(bool isEnable);
		void							SetViewChatFlag(int iFlag);
		void							SetAlwaysShowNameFlag(int iFlag);
		void							SetShowDamageFlag(int iFlag);
		void							SetShowSalesTextFlag(int iFlag);
		void							SetHidePets(bool iFlag)								{ m_Config.bHidePets = iFlag; }
		void							SetHideMounts(bool iFlag)							{ m_Config.bHideMounts = iFlag; }
		void							SetHideShamans(bool iFlag)							{ m_Config.bHideShamans = iFlag; }
		
		bool							GetShowOfflineShopFlag();
		void							SetShowOfflineShop(int iFlag);
		
#ifdef ENABLE_FOG_FIX
		void							SetFogMode(int iFlag);
		bool							IsFogMode();
#endif

#if defined(WJ_SHOW_MOB_INFO)
		void							SetShowMobAIFlag(int iFlag);
		bool							IsShowMobAIFlag();
		void							SetShowMobLevel(int iFlag);
		bool							IsShowMobLevel();
#endif

		bool							IsShowMoneyLog();
		void							SetShowMoneyLog(int iFlag);
		
		bool							IsShowDiceInfo();
		void							SetShowDiceInfo(int iFlag);
		
#if defined(ENABLE_SHOW_NIGHT_SYSTEM)
		void							SetNightMode(int iFlag);
		bool							GetNightMode();
#endif

		void							SetAutoPickUp(int iFlag);
		bool							IsAutoPickUp();
		
		void							SetSnowTexturesMode(int iFlag);
		bool							IsSnowTexturesMode();
		
		void							SetDesertTexturesMode(int iFlag);
		bool							IsDesertTexturesMode();
		
		bool							IsEnableSnowFall();
		void							SetEnableSnowFall(int iFlag);
		
		bool							IsItemHighlight();
		void							SetItemHighlight(int iFlag);
		
		// Window
		void							SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight);

		// SaveID
		int								IsSaveID();
		const char *					GetSaveID();
		void							SetSaveID(int iValue, const char * c_szSaveID);

		/// Display
		void							GetDisplaySettings();

		int								GetResolutionCount();
		int								GetFrequencyCount(int index);
		bool							GetResolution(int index, OUT DWORD *width, OUT DWORD *height, OUT DWORD *bpp);
		bool							GetFrequency(int index, int freq_index, OUT DWORD *frequncy);
		int								GetResolutionIndex(DWORD width, DWORD height, DWORD bpp);
		int								GetFrequencyIndex(int res_index, DWORD frequency);
		bool							isViewCulling();

		// Sound
		float							GetMusicVolume();
		int								GetSoundVolume();
		void							SetMusicVolume(float fVolume);
		void							SetSoundVolumef(float fVolume);

		int								GetDistance();
		int								GetShadowLevel();
		void							SetShadowLevel(unsigned int level);
		
#ifdef ENABLE_SHOPNAMES_RANGE
		void							SetShopNamesRange(float fRange);
		float							GetShopNamesRange();
#endif

#if defined(ENABLE_GRAPHIC_MASK)
		bool							GetGraphicMaskPart(const BYTE bPart);
		void							SetGraphicMaskPart(const BYTE bPart, const bool bFlag);
#endif

	protected:
		TResolution						m_ResolutionList[RESOLUTION_MAX_NUM];
		int								m_ResolutionCount;

		TConfig							m_Config;
		TConfig							m_OldConfig;

		bool							m_isInterfaceConfig;
		PyObject *						m_poInterfaceHandler;
		TWindowStatus					m_WindowStatus[WINDOW_MAX_NUM];
};