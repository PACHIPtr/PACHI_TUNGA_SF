#ifndef __INC_SERVICE_H__
#define __INC_SERVICE_H__
/*******************************************************************/
/*** Main Macros ***/
/*******************************************************************/
//#define URIEL_AC_VERIFY
#define ENABLE_PET_SYSTEM											// TR Tipi Evcil Hayvan Sistemi
#define ENABLE_GROWTH_PET_SYSTEM									// TR Tipi Seviyeli Evcil Hayvan Sistemi
#define ENABLE_WEAPON_COSTUME_SYSTEM								// TR Tipi Silah Kostümü Sistemi
#define ENABLE_MAGIC_REDUCTION_SYSTEM								// TR Tipi Büyü Bozma Taşı Sistemi
#define ENABLE_ITEM_HIGHLIGHT_SYSTEM								// TR Tipi Yeni Gelen Eşya Parlama Sistemi
#define ENABLE_7AND8TH_SKILLS_SYSTEM								// TR Tipi Yedinci Ve Sekizinci Beceri Sistemi
#define ENABLE_WOLFMAN_CHARACTER									// TR Tipi Lycan Karakteri Sistemi
#define ENABLE_CHEQUE_SYSTEM										// TR Tipi Won Parası Sistemi
#define ENABLE_ITEM_SEALBIND_SYSTEM									// TR Tipi Eşya Ruha Bağlama Sistemi
#define ENABLE_TALISMAN_SYSTEM										// TR Tipi Tılsım Sistemi
#define ENABLE_MESSENGER_BLOCK_SYSTEM								// TR Tipi Oyuncu Engelleme Sistemi
#define ENABLE_CHANGELOOK_SYSTEM									// TR Tipi Eşya Yansıtma Sistemi
#define ENABLE_DICE_SYSTEM											// TR Tipi Grup Zar Atma Sistemi
#define ENABLE_SHOP_SEARCH_SYSTEM									// TR Tipi Pazarda Eşya Arama Sistemi
#define ENABLE_GEM_SYSTEM											// TR Tipi Gaya Parası Sistemi
#define ENABLE_SHOP_DECORATION_SYSTEM								// TR Tipi Kaşmir Paketi Sistemi
#define ENABLE_TARGET_ELEMENT_SYSTEM								// TR Tipi Element Bilgisi Sistemi
#define ENABLE_BATTLE_ZONE_SYSTEM									// TR Tipi Savaş Bölgesi Sistemi
#define ENABLE_ACCE_SYSTEM											// TR Tipi Omuz Kuşağı Sistemi
#define ENABLE_CHANNEL_SWITCH_SYSTEM								// TR Tipi Kanal Değiştirme Sistemi
#define ENABLE_ITEM_COMBINATION_SYSTEM								// TR Tipi Kostüm / Beceri Kitabı Kombinasyon Sistemi
#define ENABLE_COSTUME_ENCHANT_SYSTEM								// TR Tipi Kostüm Efsunlama Sistemi
#define ENABLE_EXTEND_INVENTORY_SYSTEM								// TR Tipi Envanter Genişletme Sistemi
#define ENABLE_ZODIAC_TEMPLE_SYSTEM									// TR Tipi Zodyak Tapınağı Sistemi
#define ENABLE_ITEM_SOUL_SYSTEM										// TR Tipi Rüya Ruhu Sistemi
#define ENABLE_ITEM_GACHA_SYSTEM									// TR Tipi Yeni Sandık Sistemi
#define ENABLE_DEFENSE_WAVE_SYSTEM									// TR Tipi Hidra Tapınağı Sistemi
#define ENABLE_DUNGEON_FOR_GUILD									// TR Tipi Meley Zindanı Sistemi
#define ENABLE_BONUS_STRONG_AGAINST_WEAPON							// TR Tipi Silahlara Karşı Güçlü Bonuslar
#define ENABLE_ELEMENT_NEW_BONUSES									// TR Tipi Element Bonusları
#define ENABLE_MAIL_BOX_SYSTEM										// TR Tipi Posta Kutusu Sistemi
#define ENABLE_DS_GRADE_MYTH										// TR Tipi Mitsi Simya Sistemi
#define ENABLE_COSTUME_SET_SYSTEM									// TR Tipi Set Bonus Sistemi
#define ENABLE_REFINE_MSG_ADD										// TR Tipi Yükseltme Uyarısı Sistemi
#define ENABLE_PET_ATTR_DETERMINE									// TR Tipi Yeni Pet Güncellemesi
#define ENABLE_EMOTION_SYSTEM										// TR Tipi Duygular Ekranı
#define ENABLE_AURA_SYSTEM											// TR Tipi Aura Sistemi
#define ENABLE_MOUNT_CHANGELOOK_SYSTEM								// TR Tipi Binek Yansitma Sistemi
#define ENABLE_QUIVER_SYSTEM										// TR Tipi Ok Cantasi Sistemi
#define ENABLE_AUTO_HUNTING_SYSTEM									// TR Tipi Otomatik Av Sistemi
#define ENABLE_GYEONGGONG_SKILL										// TR Tipi GYEONGGONG Guncellemesi
#define ENABLE_PAERYONG_SKILL										// TR Tipi Saman Skill Guncellemesi
#define ENABLE_SPECIAL_GACHA_SYSTEM									// TR Tipi Sans Kutusu Sistemi
#define ENABLE_KNOCKBACK_SKILL										// TR Tipi KnockBack Skill Guncellemesi

#define WJ_SHOP_MOB_INFO											// TR Tipi Canavar Seviye / Agresiflik Sistemi
#define WJ_TRADABLE_ICON											// TR Tipi Slot Marking Sistemi
//#define USE_SPECIAL_CHARACTERS										// Karakter Yaratırken Özel Karakter Kullanabilme Sistemi - locale_service.cpp 
//#define ENABLE_ACCOUNT_W_SPECIALCHARS								// Karakter Yaratırken Özel Karakter Kullanabilme Sistemi - input_auth.cpp

#define ENABLE_TARGET_INFORMATION_SYSTEM							// Canavar Bilgi Sistemi
#define ENABLE_VIEW_TARGET_MONSTER_HP								// Canavar Yaşam Puanı Gösterme Sistemi
#define ENABLE_SPECIAL_INVENTORY_SYSTEM								// Geliştirilmiş Yeni K Envanteri Sistemi
#define ENABLE_BUFFI_SYSTEM											// Yardımcı Şaman Karakteri Sistemi
#define ENABLE_DROP_DIALOG_EXTENDED_SYSTEM							// Yere Eşya Düşürürken Sil Sat Sistemi
#define ENABLE_GUILD_LEADER_SYSTEM									// Lonca Lideri Ve Generali Sistemi
#define ENABLE_VIEW_EQUIPMENT_SYSTEM								// Onaylı Ekipman Görüntüleme Sistemi
#define ENABLE_YOUTUBER_SYSTEM										// Youtuber Sistemi Ve Guisi
#define ENABLE_SHOW_CHEST_DROP_SYSTEM								// Sandık Aynası Sistemi
#define ENABLE_WEAPON_RARITY_SYSTEM									// Silah Evrim Puanı Sistemi
#define ENABLE_EXTENDED_BIOLOG_SYSTEM								// Gelişmiş Biyolog Sistemi
#define ENABLE_OFFLINE_SHOP_SYSTEM									// Çevrimdışı Pazar Sistemi
#define ENABLE_GIFTBOX_SYSTEM										// Hediye Kutusu Sistemi
#define ENABLE_HEALTH_BOARD_SYSTEM									// Oyuncu Üzerinde Yaşam Puanı Görüntüleme Sistemi
#define ENABLE_UPGRADE_SOCKET_SYSTEM								// Ekipmanlar İçin Yeni Soket Açma Sistemi
#define ENABLE_MESSENGER_TEAM_SYSTEM								// Arkadaş Listesinde Yönetim Sistemi
#define ENABLE_EXTENDED_PET_SYSTEM									// Giyilebilir Evcil Hayvan Sistemi
#define ENABLE_FAST_SOUL_STONE_SYSTEM								// Hızlı Ruh Taşı Okuma Sistemi
#define ENABLE_FAST_SKILL_BOOK_SYSTEM								// Hızlı Beceri Kitabı Okuma Sistemi
#define ENABLE_IMPROVED_GUILD_WAR_SYSTEM							// Gelişmiş Lonca Savaşı Sistemi
#define ENABLE_GUILD_SPY_SYSTEM										// Lonca Ajan Sistemi
#define ENABLE_FAST_SKILL_SELECT_SYSTEM								// Hızlı Beceri Seçme Sistemi
#define ENABLE_GUILD_HISTORY_SYSTEM									// Lonca Geçmişi Sistemi
#define ENABLE_GAME_OPTION_SYSTEM									// Oyun Seçeneklerine Yeni Eklentiler
#define ENABLE_DEFAULT_PRIV_SYSTEM									// Otomatik Drop Sistemi
#define ENABLE_GUILD_NOTICE_SYSTEM									// Lonca Duyuru Sistemi
#define ENABLE_EXCHANGE_WINDOW_SYSTEM								// Yeni Ticaret Sistemi
#define ENABLE_BASIC_ITEM_SYSTEM									// Başlangıç Hediyesi Sistemi
#define ENABLE_BOSS_DEDECTOR_SYSTEM									// Patron Yaratık Dedektörü
#define ENABLE_AFK_MODE_SYSTEM										// Afk Sistemi
#define ENABLE_DC_COUPON_SYSTEM										// Ejderha Parası Kuponu Sistemi
#define ENABLE_IMPROVE_DRAGON_COIN									// Ejderha Parasının Bir Çok Yerde Kullanımı
#define ENABLE_BONUS_STRONG_AGAINST_BOSS							// Patron Yaratıklara Karşı Güçlü Bonuslar
#define ENABLE_EXTEND_ITEM_AWARD									// Gelişmiş ItemAward Tablosu
#define ENABLE_DANCE_EVENT_SYSTEM									// Dans Etkinliği Sistemi
#define ENABLE_SOLD_ITEM_SYSTEM										// Offline Shop Satıldı Sistemi
#define ENABLE_SHOP_SEEN											// Offline Shop Bakanlar Sistemi
#define ENABLE_SAFEBOX_MONEY_SYSTEM									// Depoda Won ve Yang Sistemi
#define ENABLE_CHEQUE_COUPON_SYSTEM									// Won Çeki Sistemi
//#define ENABLE_WORSHIP_SYSTEM										// Ibadet Sistemi
//#define ENABLE_CAPTCHA_SYSTEM										// Captcha Kontrol Sistemi
#define ENABLE_ABUSE_SYSTEM											// Hile Kontrol Sistemi
#define ENABLE_ALIGNMENT_SYSTEM										// Yeni Derece Sistemi
#define ENABLE_VALUE_PACK_SYSTEM									// Değerli Paket Sistemi
#define ENABLE_BUY_WITH_ITEM										// Buy with ITEM on SHOP like Rubinum
#define ENABLE_MOUNT_COSTUME_SYSTEM									// Yeni Binek Sistemi
#define ENABLE_EFFECT_STONE_SYSTEM									// Efekt Taşı Sistemi
#define ENABLE_DEBUG_MODE_SYSTEM									// Gelistirici Mod Sistemi
#define ENABLE_RANGE_NPC_SYSTEM										// Uzaktan NPC Sistemi
#define ENABLE_DUNGEON_BACK_SYSTEM									// Dungeon Geri Don Sistemi
#define ENABLE_DUNGEON_INFO_SYSTEM									// Zindan Takip Sistemi
#define ENABLE_AUTO_NOTICE											// Otomatik Duyuru Sistemi
#define ENABLE_AFFECT_POLYMORPH_REMOVE								// Donusumden Cikma Sistemi
#define ENABLE_SKILL_AFFECT_REMOVE									// Skillden Cikma Sistemi
#define ENABLE_FAST_CHEQUE_GOLD_TRANSFER							// Hizli Won Yang Cevirme Sistemi
#define ENABLE_MUSHROOM_DUNGEON										// Mantar Zindani Sistemi
#define ENABLE_WATER_DUNGEON										// Su Zindani Sistemi
#define ENABLE_KING_GUILD_SYSTEM									// Hukumdar Lonca Sistemi
#define ENABLE_GUILD_HIGHLIGHT_EQUIP_ITEM							// Lonca Savasi Item Giyme Sorgu
#define ENABLE_AVERAGE_ITEM_PRICE									// Fiyat Belirleme Sistemi
#define ENABLE_PERMA_ACCESSORY_SYSTEM								// Perma Cevher Sistemi
#define ENABLE_GUILD_RANKING_SYSTEM									// Lonca Siralamasi Sistemi
#define ENABLE_BOSS_GEM_SYSTEM										// Patron Gaya Sistemi
#define ENABLE_BONUS_TO_BOSS										// Patronlara Karsi Guc
#define ENABLE_PERMA_BLEND_SYSTEM									// Perma Sebnem Sistemi
#define ENABLE_BOSS_MANAGER_SYSTEM									// Boss tracking system
//#define ENABLE_BLEND_ICON_SYSTEM									// Sebnem Icon Sistemi
/*******************************************************************/
/*** Update Macros ***/
/*******************************************************************/
#define ENABLE_VERSION_162_UPDATE									// TR Tipi Ochao Tapınağı Ve Büyülü Orman Güncellemesi
#define ENABLE_PARTY_UPDATE											// TR Tipi Grup Güncellemesi
#define ENABLE_OX_RENEWAL											// TR Tipi Ox Etkinliği Güncellemesi
#define ENABLE_SAFEBOX_UPDATE										// TR Tipi Nesne Market Güncellemesi

#define ENABLE_CUBE_RENEWAL											// Arındırma Paneli Güncellemesi
#define ENABLE_WHISPER_RENEWAL										// PM De Yazıyor Güncellemesi
#define ENABLE_AUTO_POTION_RENEWAL									// Otopot Güncellemesi
#define ENABLE_CUBE_EX_RENEWAL										// Arındırma Penceresi Yeni Gui
#define ENABLE_MONSTER_DEATHBLOW_RENEWAL							// Canavar DeathBlow Güncellemesi
#define ENABLE_WIND_SHOES_RENEWAL									// Rüzgarın Ayakkabıları Yenilemesi
#define ENABLE_SKILL_RENEWAL										// Beceri Yenilemesi
//#define ENABLE_CRITICAL_PCT_RENEWAL									// Kritik Hasar Yenilemesi
#define ENABLE_STUN_RENEWAL											// Sersemlik Efekti Iptali
#define ENABLE_ATTR_RENEWAL											// Efsun Renewal
#define ENABLE_CUBE_PERCENT_RENEWAL									// Cube Penceresi Sans Gorunumu
/*******************************************************************/
/*** Event Macros ***/
/*******************************************************************/
#define ENABLE_MINIGAME_RUMI_EVENT									// TR Tipi Okey Kart Etkinliği
#define ENABLE_FISH_JIGSAW_EVENT									// TR Tipi Balık Yapboz Etkinliği
#define ENABLE_ATTENDANCE_EVENT										// TR Tipi Patron Avcıları Etkinliği
#define ENABLE_MINI_GAME_CATCH_KING									// TR Tipi Kralı Yakala Etkinliği
#define ENABLE_HALLOWEEN_EVENT										// TR Tipi Cadılar Bayramı Etkinliği
#define ENABLE_MOON_DROP_EVENT										// TR Tipi Ayışığı Define Sandığı Etkinliği
#define ENABLE_HEXAGON_DROP_EVENT									// TR Tipi Altıgen Sandığı Etkinliği
#define ENABLE_CARVING_STONE_EVENT									// TR Tipi Oyma Taş Etkinliği
#define ENABLE_RAMADAN_EVENT										// TR Tipi Ramazan Etkinliği
#define ENABLE_XMAS_EVENT											// TR Tipi Yılbaşı Etkinliği
#define ENABLE_EASTER_EVENT											// TR Tipi Paskalya Etkinliği
#define ENABLE_FOOTBALL_EVENT										// TR Tipi Futbol Topu Etkinliği
#define ENABLE_VALENTINE_EVENT										// TR Tipi Sevgililer Günü Etkinliği
#define ENABLE_KIDS_DAY_EVENT										// TR Tipi Bulmaca Kutusu Etkinliği
#define ENABLE_OLYMPIC_EVENT										// TR Tipi Olimpiyat Etkinliği
#define ENABLE_MOUNT_EVENT											// TR Tipi Binek Parşomeni Etkinliği
#define ENABLE_YEAR_ROUND_EVENT										// TR Tipi Yıldönümü Etkinliği
#define ENABLE_HARVEST_FESTIVAL_EVENT								// TR Tipi Yağmacıları Avla Etkinliği
#define ENABLE_SUPER_METIN_EVENT									// TR Tipi Süper Metin Etkinliği

#define ENABLE_PET_EVENT											// Evcil Hayvan Etkinliği
#define ENABLE_ALIGNMENT_EVENT										// Derece Etkinliği
#define ENABLE_MINING_EVENT											// Madencilik Etkinliği
#define ENABLE_MAKE_WONSO_EVENT										// Tatlı Yapım Etkinliği
#define ENABLE_GOLDEN_RAIN_EVENT									// Süper Yang Etkinliği
#define ENABLE_MUSIC_EVENT											// Müzik Etkinkiği
#define ENABLE_LUCKYBOX_EVENT										// Sans Kutusu Etkinligi

#define ENABLE_EVENT_SYSTEM											// Etkinlik Guisi Sistemi
#define ENABLE_AUTO_EVENT_SYSTEM									// Otomatik Etkinlik Sistemi
/*******************************************************************/
/*** Sub Macros ***/
/*******************************************************************/
#ifdef ENABLE_VIEW_TARGET_MONSTER_HP
#define USE_TARGET_DECIMAL_HP									// Canavar Yaşam Puanını Yüzdeli Gösterme
#endif
#ifdef ENABLE_VERSION_162_UPDATE
#define HEALING_SKILL_VNUM 265									// Ochao Tapınağı İyileştirme Becerisi Kodu
#endif
#ifdef ENABLE_BATTLE_ZONE_SYSTEM
#define ENABLE_BATTLE_ZONE_SYSTEM_HIDE_INFO_USER				// Savaş bölgesi oyuncu bilgisi gizleme
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#define SHOP_TIME_REFRESH 1*60									// Eski dükkanları yenileme süresi
#define SHOP_BLOCK_GAME99										// Kanal99'da pazar kurulamasın
#define SHOP_AUTO_CLOSE											// Otomatik dükkan kapatma
#define SHOP_GM_PRIVILEGES GM_IMPLEMENTOR 						// GM çevrimdışı pazar yetkileri.
#endif
#ifdef ENABLE_DUNGEON_FOR_GUILD
#define ENABLE_MELEY_LAIR_DUNGEON								// Meley Ejderhası
#ifdef ENABLE_MELEY_LAIR_DUNGEON
#define DESTROY_INFINITE_STATUES_GM							// GM Ler Sonsuz Statü Yokedebilir
#define LASER_EFFECT_ON_75HP								// Yüzde 75 Yaşam Puanında Lazer Efekti
#define LASER_EFFECT_ON_50HP								// Yüzde 50 Yaşam Puanında Lazer Efekti
#endif
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
#define USE_TRANSMUTATION_SYSTEM_MAILBOX						// Posta Kutusu İçin Eşya Yansıtma
#define USE_WON_SYSTEM_MAILBOX									// Posta Kutusu İçin Won Parası
#define USE_ACCE_SYSTEM_MAILBOX									// Posta Kutusu İçin Omuz Kuşağı Sistemi
#endif
#ifdef ENABLE_EXTEND_ITEM_AWARD
	#define USE_ITEM_AWARD_CHECK_ATTRIBUTES 					// c++11 or higher
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	#define ENABLE_ZODIAC_TEMPLE_CHAT							// zodyak tapınağı için chat
#endif
/*******************************************************************/
/*** Security Macros ***/
/*******************************************************************/
#define _IMPROVED_PACKET_ENCRYPTION_								// TR Tipi Paket Şifreleme Sistemi
#define ENABLE_UDP_BLOCK											// UDP Paket Bloklaması
#define ENABLE_NONSWORD_SKILL_BUG_FIX								// Silah Ekipmanı Olmadan Beceri Kullanımı Önlemi
#define ENABLE_MYSQL_LOGS											// Mysql Loglarını Aktif Eder
#define ENABLE_PTSYS_LOG											// PT Ve Sys Loglarını Aktif Eder
#define ENABLE_HWID_BAN_SYSTEM										// Hwid Güvenlik Sistemi
#define LSS_SECURITY_KEY "PraE5Jg43fZsG6pt"									// Güvenlik Kodu
#define ENABLE_ANTI_CMD_FLOOD										// Hızlı Komut Kullanımı Önlemi
#define ENABLE_DROP_HACK_FIX										// Hızlı Yere Eşya Düşürme Önlemi
#define ENABLE_CAMP_FIRE_FIX										// Hızlı Kamp Ateşi Kullanma Önlemi
#define ENABLE_PORT_SECURITY										// P2P Ve Port Güvenlik Önlemleri
#define ENABLE_MESSENGER_AUTH_EXPLOIT_FIX							// Messenger Üzerinden Oyundan Attırma Hilesi Önlemi
#define ENABLE_SPECIAL_ITEM_FIX										// Special_item komutu Kullanıldığında Crash Önlemi
#define ENABLE_CAPE_EFFECT_FIX										// Hızlı Cesaret Pelerini Kullanımı Önlemi
#define ENABLE_SKILL_GROUP_VISUAL_BUG_FIX							// Skill Değişikliğinde Hava Kılıcı vs. Açık Kalması
#define ENABLE_OBSERVER_DAMAGE_FIX									// İzleyici Modu Hasar Fixi
#define ENABLE_ANTISAFEZONE											// Anti Safezone Engeli
#define ENABLE_GHOSTMODE_FIX										// Ölüyken Yürüme Hilesi Engeli
//#define ENABLE_WALLHACK_FIX											// Duvardan Geçme Hilesi Engeli
#define ENABLE_ANTINEARWATER_FIX									// Su Olmayan Yerde Balık Tutma Hilesi
#define ENABLE_FISHBOT_FIX											// Balık Botu Fixi
//#define ENABLE_WAITHACK_FIX											// Damage Hilesi Fixi
#define ENABLE_CLIENT_VERSION_SYSTEM								// Giris Guvenlik Sistemi

//#define ENABLE_LOYEIN_SVSIDE_HACKSHIELD							// Svside Hile Engel Sistemi
/*******************************************************************/
/*** Utils Macros ***/
/*******************************************************************/
#define ENABLE_CLEAR_OLD_GUILD_LANDS								// Eski Lonca Arazilerinin Silinmesi
#define ENABLE_COORDINATES_ON_COMMAND_USER							// User Komutunda Oyuncunun Yanında Koordinat Gösterme
#define ENABLE_FULL_NOTICE											// Büyük Duyuru Güncelleştirmesi
#define ENABLE_CHAT_LOGGING											// Yazışma Loglarının Tutulması
#define ENABLE_DRAGON_SOUL_ACTIVE_EFFECT							// Simya Aktif Efekti
#define ENABLE_BLOCK_ITEMS_ON_WAR_MAP								// Yaşam Kutsaması vs. Lonca Savaşında Engellenir
#define ENABLE_SET_STATE_WITH_TARGET								// State Komutu İle İlgili Güncelleştirme
#define ENABLE_ACCESSORY_BUG_FIX									// Ekipman Soketleri İle İlgili Düzenleme
#define ENABLE_TOGGLE_BUFF_SYSTEM									// Oyuna Girişlerde Pet Etc. gitmez
#define ENABLE_OXEVENT_COORDINAT_BUFF								// Ox Etkinliği Koordinat Düzenlemesi
#define ENABLE_ADVANCED_RELOAD_SYSTEM								// Reload Komutları Güncelleştirmesi
#define ENABLE_IGNORE_LOWER_BUFFS									// Düşük Seviye Buffları Engeller
#define ENABLE_PLAYER_PER_ACCOUNT5									// Tek Hesapta 5 Adet Karakter Oluşturma
#define ENABLE_ITEMAWARD_REFRESH									// item_award.sql Güncelleştirme
#define ENABLE_PARTY_KILL											// Grup ile ilgili kill güncelleştirmesi
#define ENABLE_GOHOME_IF_MAP_NOT_EXIST								// Map Bulunmuyorsa Oyuncuyu Şehre Işınlar
#define ENABLE_TIME_WEAPON_POINT_FIX								// Süreli silahlar ile ilgili bir düzeltme
#define ENABLE_FALL_FIX												// Yere Düşme İle İlgili Engeller
#define ENABLE_NEMERE_ADDONS										// Nemere İçin Source Kontrolleri
#define ENABLE_PARTY_EXP_FIX										// Grup Tecrübe Paylaşımı Mesafe Kontrolü
#define ENABLE_POISON_UNAGGR										// Canavarlar Zehir Yedikleri Zaman Agresif Olmazlar
#define ENABLE_FLUSH_CACHE_FEATURE									// ShopEx Hızlı Kaydetme Aktifliği
#define ENABLE_LUA_FUNCTION_TIME									// Lua İçin Zaman Uyarlaması
#define ENABLE_COSTUME_UNEQUIP_MARRIAGE_ITEMS						// Kostümler İçin Evlilik Eşyası Engeli
#define ENABLE_RELOAD_COMMAND_ALL_CORES								// Tüm Coreler İçin Reload Komutu İşlevi
//#define ENABLE_COMPUTE_POINTS_WITHOUT_UPDATE_PACKET					// Update Packet Kullanmadan Compute points İşlevleri
#define ENABLE_NON_CHECK_DS_QUALIFIED								// Simya Kontrolünü Devre Dışı Bırakır
//#define GOLD_MULTIPLER	18											// Yang Katsayisi 18
//#define EXP_MULTIPLER	2											// Exp Katsayisi 2
#define ENABLE_OFFLINESHOP_MESSAGE_SYSTEM							// Offline Shop Mesaj Gonderme Sistemi
#define ENABLE_DROP_PCT_EXTRA										// Ekstra 100 Drop Pct
#define ENABLE_TALISMAN_ATTR										// Tilsim Efsun Nesnesi
#define DISABLE_PRIVATE_SHOP_SEARCH_NEED_ITEM						// Esyasiz Item Arama Kullanimi
#define ENABLE_FAST_SOUL_REFINE										// Simya Arindirma Refine Penceresi
#define INC_GOLD_MULTIPLER	2										// Yang katsayisi dusurme

#define ENABLE_ANNOUNCEMENT_LEVELUP									// Seviye Atlandığında Duyuru Geçer
#define ENABLE_ANNOUNCEMENT_REFINE_SUCCES							// +basma duyuru
#define ENABLE_ANNOUNCEMENT_REFINE_SUCCES_MIN_LEVEL 9 				// duyurmasini istedigin +degeri
#define ENABLE_ANNOUNCEMENT_REFINE_LEVEL_LIMIT	54					// duyurmasını istediğin itemin level limiti

//#define ENABLE_IMMUNE_PERC										// Yüzde 75 Sersemlikten Korunma Şansı
/*******************************************************************/
#endif