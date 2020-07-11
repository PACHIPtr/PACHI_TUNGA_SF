#include "packet.h"

std::string MythsiaPacketString(BYTE bHeader)
{
	switch (bHeader)
	{
		case HEADER_CG_LOGIN:
			return "Oyuna Giris (HEADER_CG_LOGIN)";
		case HEADER_CG_ATTACK:
			return "Saldiri (HEADER_CG_ATTACK)";
		case HEADER_CG_CHAT:
			return "Konusma (HEADER_CG_CHAT)";
		case HEADER_CG_CHARACTER_CREATE:
			return "Karakter Yaratma (HEADER_CG_CHARACTER_CREATE)";
		case HEADER_CG_CHARACTER_DELETE:
			return "Karakter Silme (HEADER_CG_CHARACTER_DELETE)";
		case HEADER_CG_CHARACTER_SELECT:
			return "Karakter Secme (HEADER_CG_CHARACTER_SELECT)";
		case HEADER_CG_MOVE:
			return "Hareket Etme (HEADER_CG_MOVE)";
		case HEADER_CG_SYNC_POSITION:
			return "Senkronizasyon (HEADER_CG_SYNC_POSITION)";
		case HEADER_CG_ENTERGAME:
			return "Haritaya Giris (HEADER_CG_ENTERGAME)";
		case HEADER_CG_ITEM_USE:
			return "Esya Kullanma (HEADER_CG_ITEM_USE)";
#ifdef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
		case HEADER_CG_ITEM_DELETE:
			return "Esya Silme (HEADER_CG_ITEM_DELETE)";
#else
		case HEADER_CG_ITEM_DROP:
			return "Esya Yere Atma (HEADER_CG_ITEM_DROP)";
#endif
		case HEADER_CG_ITEM_MOVE:
			return "Esya Yer Degistirme (HEADER_CG_ITEM_MOVE)";
#ifdef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
		case HEADER_CG_ITEM_SELL:
			return "Esya Satma (HEADER_CG_ITEM_SELL)";
#endif
		case HEADER_CG_ITEM_PICKUP:
			return "Esya Toplama (HEADER_CG_ITEM_PICKUP)";
		case HEADER_CG_QUICKSLOT_ADD:
			return "Taskbar Ekleme (HEADER_CG_QUICKSLOT_ADD)";
		case HEADER_CG_QUICKSLOT_DEL:
			return "Taskbar Silme (HEADER_CG_QUICKSLOT_DEL)";
		case HEADER_CG_QUICKSLOT_SWAP:
			return "Taskbar Degistirme (HEADER_CG_QUICKSLOT_SWAP)";
		case HEADER_CG_WHISPER:
			return "Ozel Konusma (HEADER_CG_WHISPER)";
#ifndef ENABLE_DROP_DIALOG_EXTENDED_SYSTEM
		case HEADER_CG_ITEM_DROP2:
			return "Gelismis Esya Yere Atma (HEADER_CG_ITEM_DROP2)";
#endif
		case HEADER_CG_ON_CLICK:
			return "Tiklama (HEADER_CG_ON_CLICK)";
		case HEADER_CG_EXCHANGE:
			return "Ticaret (HEADER_CG_EXCHANGE)";
		case HEADER_CG_CHARACTER_POSITION:
			return "Karakter Pozisyon (HEADER_CG_CHARACTER_POSITION)";
		case HEADER_CG_SCRIPT_ANSWER:
			return "Quest Cevap (HEADER_CG_SCRIPT_ANSWER)";
		case HEADER_CG_QUEST_INPUT_STRING:
			return "Quest Input String (HEADER_CG_QUEST_INPUT_STRING)";
		case HEADER_CG_QUEST_CONFIRM:
			return "Quest Kabul (HEADER_CG_QUEST_CONFIRM)";
		case HEADER_CG_SHOP:
			return "Dukkan (HEADER_CG_SHOP)";
		case HEADER_CG_FLY_TARGETING:
			return "Takip Eden Efekt (HEADER_CG_FLY_TARGETING)";
		case HEADER_CG_USE_SKILL:
			return "Beceri Kullanma (HEADER_CG_USE_SKILL)";
		case HEADER_CG_ADD_FLY_TARGETING:
			return "Takip Eden Efekt Ekleme (HEADER_CG_ADD_FLY_TARGETING)";
		case HEADER_CG_SHOOT:
			return "Vurus (HEADER_CG_SHOOT)";
		case HEADER_CG_MYSHOP:
			return "Pazar (HEADER_CG_MYSHOP)";
		case HEADER_CG_ITEM_USE_TO_ITEM:
			return "Esya Uzerine Esya Surukleme (HEADER_CG_ITEM_USE_TO_ITEM)";
		case HEADER_CG_TARGET:
			return "Hedef Tahtasi (HEADER_CG_TARGET)";
		case HEADER_CG_TEXT:
			return "P2P (HEADER_CG_TEXT)";
		case HEADER_CG_WARP:
			return "Isinlanma (HEADER_CG_WARP)";
		case HEADER_CG_SCRIPT_BUTTON:
			return "Quest Buton (HEADER_CG_SCRIPT_BUTTON)";
		case HEADER_CG_MESSENGER:
			return "Arkadas Listesi (HEADER_CG_MESSENGER)";
		case HEADER_CG_MALL_CHECKOUT:
			return "Nesne Market Esya Alma (HEADER_CG_MALL_CHECKOUT)";
		case HEADER_CG_SAFEBOX_CHECKIN:
			return "Depo Esya Ekleme (HEADER_CG_SAFEBOX_CHECKIN)";
		case HEADER_CG_SAFEBOX_CHECKOUT:
			return "Depo Esya Kaldirma (HEADER_CG_SAFEBOX_CHECKOUT)";
		case HEADER_CG_PARTY_INVITE:
			return "Grup Daveti (HEADER_CG_PARTY_INVITE)";
		case HEADER_CG_PARTY_INVITE_ANSWER:
			return "Grup Daveti Kabul (HEADER_CG_PARTY_INVITE_ANSWER)";
		case HEADER_CG_PARTY_REMOVE:
			return "Grup Daveti Red (HEADER_CG_PARTY_REMOVE)";
		case HEADER_CG_PARTY_SET_STATE:
			return "Grup Exp Paylasimi (HEADER_CG_PARTY_SET_STATE)";
		case HEADER_CG_PARTY_USE_SKILL:
			return "Grup Beceri Kullanimi (HEADER_CG_PARTY_USE_SKILL)";
		case HEADER_CG_SAFEBOX_ITEM_MOVE:
			return "Depo Esya Surukleme (HEADER_CG_SAFEBOX_ITEM_MOVE)";
		case HEADER_CG_PARTY_PARAMETER:
			return "Grup Uyesi Ozellik (HEADER_CG_PARTY_PARAMETER)";
		case HEADER_CG_GUILD:
			return "Lonca (HEADER_CG_GUILD)";
		case HEADER_CG_ANSWER_MAKE_GUILD:
			return "Lonca Kurma (HEADER_CG_ANSWER_MAKE_GUILD)";
		case HEADER_CG_FISHING:
			return "Balikcilik (HEADER_CG_FISHING)";
		case HEADER_CG_ITEM_GIVE:
			return "NPC Esya Surukleme (HEADER_CG_ITEM_GIVE)";
		case HEADER_CG_EMPIRE:
			return "Krallik (HEADER_CG_EMPIRE)";
		case HEADER_CG_REFINE:
			return "Esya Yukseltme (HEADER_CG_REFINE)";
		case HEADER_CG_MARK_LOGIN:
			return "Lonca Simgesi Loading (HEADER_CG_MARK_LOGIN)";
		case HEADER_CG_MARK_CRCLIST:
			return "Lonca Simgesi CrcList (HEADER_CG_MARK_CRCLIST)";
		case HEADER_CG_MARK_UPLOAD:
			return "Lonca Simgesi Yukleme (HEADER_CG_MARK_UPLOAD)";
		case HEADER_CG_MARK_IDXLIST:
			return "Lonca Simgesi IdxLİst (HEADER_CG_MARK_IDXLIST)";
		case HEADER_CG_HACK:
			return "Hile (HEADER_CG_HACK)";
		case HEADER_CG_CHANGE_NAME:
			return "Isim Degistir (HEADER_CG_CHANGE_NAME)";
		case HEADER_CG_LOGIN_KEY:
			return "Login Key (HEADER_CG_LOGIN_KEY)";
		case HEADER_CG_DUNGEON:
			return "Zindan (HEADER_CG_DUNGEON)";
		case HEADER_CG_LOGIN3:
			return "Real Giris Paket (HEADER_CG_LOGIN3)";
		case HEADER_CG_GUILD_SYMBOL_UPLOAD:
			return "Lonca Simgesi Upload (Diger) (HEADER_CG_GUILD_SYMBOL_UPLOAD)";
		case HEADER_CG_SYMBOL_CRC:
			return "Sembol CrC (HEADER_CG_SYMBOL_CRC)";
		case HEADER_CG_SCRIPT_SELECT_ITEM:
			return "Quest Esya Secme (HEADER_CG_SCRIPT_SELECT_ITEM)";
		case HEADER_CG_DRAGON_SOUL_REFINE:
			return "Ejderha Tasi Simyasi Yukseltme (HEADER_CG_DRAGON_SOUL_REFINE)";
		case HEADER_CG_STATE_CHECKER:
			return "Kanal Durumu (HEADER_CG_STATE_CHECKER)";
#ifdef ENABLE_SHOP_SEARCH_SYSTEM
		case HEADER_CG_SHOP_SEARCH:
			return "Pazar Arama (HEADER_CG_SHOP_SEARCH)";
		case HEADER_CG_SHOP_SEARCH_SUB:
			return "Pazar Arama (HEADER_CG_SHOP_SEARCH_SUB)";
		case HEADER_CG_SHOP_SEARCH_BUY:
			return "Pazar Esya Alma (HEADER_CG_SHOP_SEARCH_BUY)";
#endif
#ifdef ENABLE_BATTLE_ZONE_SYSTEM
		case HEADER_CG_COMBAT_ZONE_REQUEST_ACTION:
			return "Savas Bolgesi (HEADER_CG_COMBAT_ZONE_REQUEST_ACTION)";
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
		case HEADER_CG_EXTEND_INVENTORY:
			return "Envanter Genisletme (HEADER_CG_EXTEND_INVENTORY)";
#endif
#ifdef ENABLE_YOUTUBER_SYSTEM
		case HEADER_CG_YOUTUBE_PANEL:
			return "Youtube Panel (HEADER_CG_YOUTUBE_PANEL)";
		case HEADER_CG_YOUTUBE_VIDEO:
			return "Youtube Video (HEADER_CG_YOUTUBE_VIDEO)";
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
		case HEADER_CG_PetSetName:
			return "Levelli Pet (HEADER_CG_PetSetName)";
#endif
#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
		case HEADER_CG_TARGET_INFO_LOAD:
			return "Mobdan Dusen Itemler (HEADER_CG_TARGET_INFO_LOAD)";
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
		case HEADER_CG_CL:
			return "Yansitma Sistemi (HEADER_CG_CL)";
#endif
#ifdef ENABLE_ACCE_SYSTEM
		case HEADER_CG_SASH:
			return "Kusak Sistemi (ENABLE_ACCE_SYSTEM)";
#endif
#ifdef ENABLE_FISH_JIGSAW_EVENT
		case HEADER_CG_FISH_EVENT_SEND:
			return "Balikcilik Etkinligi (HEADER_CG_FISH_EVENT_SEND)";
#endif
#ifdef ENABLE_GEM_SYSTEM
		case HEADER_CG_GEM_SHOP:
			return "Gaya Sistemi (HEADER_CG_GEM_SHOP)";
#endif
#ifdef ENABLE_OX_RENEWAL
		case HEADER_CG_QUEST_INPUT_LONG_STRING:
			return "Quest Input String (HEADER_CG_QUEST_INPUT_LONG_STRING)";
#endif
#ifdef ENABLE_MAIL_BOX_SYSTEM
		case HEADER_CG_MAILBOX_SEND:
			return "Posta Kutusu (HEADER_CG_MAILBOX_SEND)";
#endif
#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
		case HEADER_CG_SAFEBOX_MONEY:
			return "Depo Para (HEADER_CG_SAFEBOX_MONEY)";
		case HEADER_CG_SAFEBOX_CHEQUE:
			return "Depo Won (HEADER_CG_SAFEBOX_CHEQUE)";
#endif
#ifdef ENABLE_AURA_SYSTEM
		case HEADER_CG_AURA:
			return "Aura Sistemi (HEADER_CG_AURA)";
#endif
#ifdef ENABLE_SHOW_CHEST_DROP_SYSTEM
		case HEADER_CG_CHEST_DROP_INFO:
			return "Sandik Aynisi Sistemi (HEADER_CG_CHEST_DROP_INFO)";
#endif
#ifdef ENABLE_ATTENDANCE_EVENT
		case HEADER_CG_ATTENDANCE_REWARD:
			return "Patron Avcilari (HEADER_CG_ATTENDANCE_REWARD)";
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
		case HEADER_CG_MINI_GAME_CATCH_KING:
			return "Krali Yakala Etkinligi (HEADER_CG_MINI_GAME_CATCH_KING)";
#endif
		default:
			return "Paket Bulunamadi";
	}
}