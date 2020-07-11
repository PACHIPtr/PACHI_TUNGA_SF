#ifndef __INC_METIN_II_GAME_CONFIG_H__
#define __INC_METIN_II_GAME_CONFIG_H__

#define  ADDRESS_MAX_LEN 15

void config_init(const std::string& st_localeServiceName); // default "" is CONFIG

extern char sql_addr[256];

extern WORD mother_port;
extern WORD p2p_port;

extern char db_addr[ADDRESS_MAX_LEN + 1];
extern WORD db_port;

extern int passes_per_sec;
extern int save_event_second_cycle;
extern int ping_event_second_cycle;
extern int test_server;
extern bool	guild_mark_server;
extern BYTE guild_mark_min_level;

extern bool	g_bNoMoreClient;
extern bool	g_bNoRegen;

extern int g_iStatusPointGetLevelLimit;
extern int g_iStatusPointSetMaxValue;

extern BYTE	g_bChannel;

extern bool	map_allow_find(int index);
extern void	map_allow_copy(long* pl, int size);
extern bool	no_wander;

extern int	g_iUserLimit;
extern time_t	g_global_time;

const char* get_table_postfix();

extern std::string	g_stHostname;
extern std::string	g_stLocale;
extern std::string	g_stLocaleFilename;

extern char		g_szPublicIP[16];
extern char		g_szInternalIP[16];

extern int (*is_twobyte) (const char* str);
extern int (*check_name) (const char* str);

extern bool		g_bSkillDisable;

extern int		g_iFullUserCount;
extern int		g_iBusyUserCount;
extern void		LoadStateUserCount();

extern bool	g_bEmpireWhisper;

extern BYTE	g_bAuthServer;

extern BYTE	PK_PROTECT_LEVEL;

extern std::string	g_stAuthMasterIP;
extern WORD		g_wAuthMasterPort;

extern std::string	g_stClientVersion;
extern void		CheckClientVersion();

extern std::string	g_stQuestDir;
//extern std::string	g_stQuestObjectDir;
extern std::set<std::string> g_setQuestObjectDir;

extern std::vector<std::string>	g_stAdminPageIP;
extern std::string	g_stAdminPagePassword;

extern int g_server_id;
extern std::string g_strWebMallURL;

extern int VIEW_RANGE;
extern int VIEW_BONUS_RANGE;

extern bool g_protectNormalPlayer;      // 범법자가 "평화모드" 인 일반유저를 공격하지 못함
extern bool g_noticeBattleZone;         // 중립지대에 입장하면 안내메세지를 알려줌

extern DWORD g_GoldDropTimeLimitValue;

extern int gPlayerMaxLevel;
extern int minAddonFKS;
extern int maxAddonFKS;
extern int minAddonDSS;
extern int maxAddonDSS;
extern bool attr_always_add;
extern bool attr_always_5_add;
extern bool stone_always_add;
extern bool emotion_without_mask;
extern bool emotion_same_gender;
extern int taxes;
extern int pc_max_movement_speed;
extern int pc_max_attack_speed;
extern int mob_max_movement_speed;
extern int mob_max_attack_speed;
extern bool trade_effect;
extern int trade_effect_exchange_threshold;
extern int trade_effect_shop_threshold;
extern bool glass_enable;
extern bool glass_needed;
extern bool syserr_enable;
extern bool syslog_enable;
extern int sb_delay;
extern int sb_need_exp;
extern bool belt_allow_all_items;
extern int g_iShoutLimitLevel;
extern int g_iShoutLimitValue;
extern BYTE g_bItemCountLimit;
extern bool gbTest_Enabled;
extern bool gbWolfman_Enabled;
extern bool gbWeaponRarity_Enabled;
extern bool gbPrivateShopSearch_Enabled;
extern bool gbSkillSelectEx_Enabled;
extern bool gbHairSelectEx_Enabled;
extern bool gbPointBoxes_Enabled;
extern bool gbCheque_Enabled;
extern bool gbAcce_Enabled;
extern bool gbChangeLook_Enabled;
extern bool gbCombination_Enabled;
extern bool gbSpecialInventory_Enabled;
extern bool gbGem_Enabled;
extern bool gbChannelChange_Enabled;
extern bool gbAura_Enabled;
extern bool gbPet_Enabled;
extern bool gbCostumeSet_Enabled;
extern bool gbSafeboxMoney_Enabled;
extern bool gbValuePack_Enabled;
extern bool gbBossTracking_Enabled;
extern bool gbDcCoupon_Enabled;
extern bool gbMailBox_Enabled;
extern bool gbRemoveSkillAffect_Enabled;
extern bool gbOpenGiftBox_Enabled;
extern bool gbTransferMoney_Enabled;
extern bool gbGuildRank_Enabled;
extern bool gbAveragePrice_Enabled;
extern bool gbShopMoney_Enabled;

extern bool g_BlockCharCreation;
#endif /* __INC_METIN_II_GAME_CONFIG_H__ */
