#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "locale_service.h"
#include "log.h"
#include "desc.h"

ACMD(do_get_warp);
ACMD(do_user_horse_ride);
ACMD(do_user_horse_back);
ACMD(do_user_horse_feed);
ACMD(do_slow);
ACMD(do_stun);
ACMD(do_poison);
ACMD(do_warp);
ACMD(do_goto);
ACMD(do_item);
ACMD(do_mob);
ACMD(do_mob_ld);
ACMD(do_mob_aggresive);
ACMD(do_mob_coward);
ACMD(do_mob_map);
ACMD(do_mob_weaken);
ACMD(do_purge);
ACMD(do_weaken);
ACMD(do_item_purge);
ACMD(do_state);
ACMD(do_notice);
ACMD(do_map_notice);
ACMD(do_big_notice);
ACMD(do_who);
ACMD(do_user);
ACMD(do_disconnect);
ACMD(do_kill);
ACMD(do_emotion_allow);
ACMD(do_emotion);
ACMD(do_transfer);
ACMD(do_set);
ACMD(do_cmd);
ACMD(do_reset);
ACMD(do_greset);
ACMD(do_mount);
ACMD(do_fishing);
ACMD(do_refine_rod);
ACMD(do_max_pick);
ACMD(do_refine_pick);
ACMD(do_fishing_simul);
ACMD(do_console);
ACMD(do_restart);
ACMD(do_advance);
ACMD(do_stat);
ACMD(do_respawn);
ACMD(do_skillup);
ACMD(do_guildskillup);
ACMD(do_pvp);
ACMD(do_point_reset);
ACMD(do_safebox_size);
ACMD(do_safebox_close);
ACMD(do_safebox_password);
ACMD(do_safebox_change_password);
ACMD(do_mall_password);
ACMD(do_mall_close);
ACMD(do_ungroup);
ACMD(do_makeguild);
ACMD(do_deleteguild);
ACMD(do_shutdown);
ACMD(do_group);
ACMD(do_group_random);
ACMD(do_invisibility);
ACMD(do_event_flag);
ACMD(do_get_event_flag);
ACMD(do_private);
ACMD(do_qf);
ACMD(do_clear_quest);
ACMD(do_book);
ACMD(do_reload);
ACMD(do_war);
ACMD(do_nowar);
ACMD(do_setskill);
ACMD(do_setskillother);
ACMD(do_level);
ACMD(do_polymorph);
ACMD(do_polymorph_item);
ACMD(do_close_shop);
ACMD(do_set_walk_mode);
ACMD(do_set_run_mode);
ACMD(do_set_skill_group);
ACMD(do_set_skill_point);
ACMD(do_cooltime);
ACMD(do_detaillog);
ACMD(do_monsterlog);
ACMD(do_gwlist);
ACMD(do_stop_guild_war);
ACMD(do_cancel_guild_war);
ACMD(do_guild_state);
ACMD(do_pkmode);
ACMD(do_messenger_auth);
ACMD(do_getqf);
ACMD(do_setqf);
ACMD(do_delqf);
ACMD(do_set_state);
ACMD(do_forgetme);
ACMD(do_aggregate);
ACMD(do_attract_ranger);
ACMD(do_pull_monster);
ACMD(do_setblockmode);
ACMD(do_priv_empire);
ACMD(do_priv_guild);
ACMD(do_mount_test);
ACMD(do_unmount);
ACMD(do_observer);
ACMD(do_observer_exit);
ACMD(do_socket_item);
ACMD(do_xmas);
ACMD(do_stat_minus);
ACMD(do_stat_reset);
ACMD(do_block_chat);
ACMD(do_vote_block_chat);
ACMD(do_block_chat_list);
ACMD(do_party_request);
ACMD(do_party_request_deny);
ACMD(do_party_request_accept);
ACMD(do_build);
ACMD(do_clear_land);
ACMD(do_horse_state);
ACMD(do_horse_level);
ACMD(do_horse_ride);
ACMD(do_horse_summon);
ACMD(do_horse_unsummon);
ACMD(do_horse_set_stat);
ACMD(do_save_attribute_to_image);
ACMD(do_affect_remove);
ACMD(do_change_attr);
ACMD(do_add_attr);
ACMD(do_add_socket);
ACMD(do_inputall)
{
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("명령어를 모두 입력하세요."));
}
ACMD(do_show_arena_list);
ACMD(do_end_all_duel);
ACMD(do_end_duel);
ACMD(do_duel);
ACMD(do_stat_plus_amount);
ACMD(do_break_marriage);
ACMD(do_oxevent_show_quiz);
ACMD(do_oxevent_log);
ACMD(do_oxevent_get_attender);
ACMD(do_effect);
ACMD(do_hair);
ACMD(do_gift);
ACMD(do_inventory);
ACMD(do_cube);
ACMD(do_temp);
ACMD(do_reset_subskill);
ACMD(do_flush);
ACMD(do_eclipse);
ACMD(do_event_helper);
ACMD(do_in_game_mall);
ACMD(do_get_mob_count);
ACMD(do_dice);
ACMD(do_special_item);
ACMD(do_click_mall);
ACMD(do_click_safebox);
ACMD(do_ride);
ACMD(do_get_item_id_list);
ACMD(do_set_socket);
ACMD(do_set_stat);
ACMD(do_can_dead);
ACMD(do_full_set);
ACMD(do_item_full_set);
ACMD(do_attr_full_set);
ACMD(do_all_skill_master);
ACMD(do_use_item);
ACMD(do_dragon_soul);
ACMD(do_ds_list);
ACMD(do_clear_affect);
ACMD(do_rewarp);
#ifdef ENABLE_GUILD_SPY_SYSTEM
ACMD(do_ajanisik);
#endif
#ifdef ENABLE_GUILD_NOTICE_SYSTEM
ACMD(do_guild_notice);
#endif
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
ACMD(do_view_equip_request);
ACMD(do_view_equip_request_accept);
ACMD(do_view_equip_request_deny);
#endif
#ifdef ENABLE_MINIGAME_RUMI_EVENT
ACMD(do_cards);
#endif
#ifdef ENABLE_GIFTBOX_SYSTEM
ACMD(do_gift_refresh);
ACMD(do_gift_get);
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
ACMD(do_shop_refresh);
ACMD(do_shop_yang);
ACMD(do_shop_won);
ACMD(do_shop_delete);
ACMD(do_shop_open);
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
ACMD(do_CubePetAdd);
ACMD(do_FeedCubePet);
ACMD(do_PetSkill);
ACMD(do_PetEvo);
ACMD(do_PetChangeName);
ACMD(do_IncreasePetSkill);
#endif
#ifdef ENABLE_CHANNEL_SWITCH_SYSTEM
ACMD(do_change_channel);
#endif
#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
ACMD(do_item_comb);
#endif
#ifdef ENABLE_FAST_SOUL_STONE_SYSTEM
ACMD(do_ruhoku);
#endif
#ifdef ENABLE_FAST_SKILL_BOOK_SYSTEM
ACMD(do_bkoku);
#endif
#ifdef ENABLE_EXTENDED_BIOLOG_SYSTEM
ACMD(do_biyolog);
#endif
#ifdef ENABLE_GAME_OPTION_SYSTEM
ACMD(do_health_board_config);
ACMD(do_sash_config);
ACMD(do_costume_config);
ACMD(do_costume_w_config);
ACMD(do_costume_h_config);
ACMD(do_level_view_config);
#endif
#ifdef ENABLE_ATTENDANCE_EVENT
ACMD(do_attendance);
ACMD(do_easter_event);
#endif
#ifdef ENABLE_WOLFMAN_CHARACTER
ACMD(do_bleeding);
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
ACMD(do_evo_points);
ACMD(do_refine_evo_points);
#endif
#ifdef ENABLE_FULL_NOTICE
ACMD(do_notice_test);
ACMD(do_big_notice_test);
ACMD(do_map_big_notice);
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
ACMD(do_open_cz);
ACMD(do_cz_reward);
ACMD(do_cz_check_box);
ACMD(do_revivedialog);
ACMD(do_revive);
ACMD(do_jump_floor);
ACMD(do_next_floor);
#endif
#ifdef ENABLE_GUILD_HISTORY_SYSTEM
ACMD(do_guild_history_req);
#endif
ACMD(do_spawnmetinacords);
#ifdef ENABLE_FAST_SKILL_SELECT_SYSTEM
ACMD(do_skill_select);
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
ACMD(do_catch_king_event);
#endif
#ifdef ENABLE_DANCE_EVENT_SYSTEM
ACMD(do_gunsam_all);
ACMD(do_gunsam_config);
#endif
#ifdef ENABLE_CHEQUE_COUPON_SYSTEM
ACMD(do_use_won_ticket);
#endif
#ifdef ENABLE_PET_ATTR_DETERMINE
ACMD(do_determine_pet);
ACMD(do_change_pet);
#endif
#ifdef ENABLE_CAPTCHA_SYSTEM
ACMD(do_captcha);
#endif
#ifdef ENABLE_DEBUG_MODE_SYSTEM
ACMD(do_set_debug_mode);
#endif
#ifdef ENABLE_RANGE_NPC_SYSTEM
ACMD(do_open_range_npc);
#endif
ACMD(do_set_feature_enabled);
ACMD(do_set_feature_disabled);
ACMD(do_feature_state);
#ifdef ENABLE_AFFECT_POLYMORPH_REMOVE
ACMD(do_remove_polymorph);
#endif
#ifdef ENABLE_SKILL_AFFECT_REMOVE
ACMD(do_remove_skill_affect);
#endif
#ifdef ENABLE_FAST_CHEQUE_GOLD_TRANSFER
ACMD(do_transfer_gold);
ACMD(do_transfer_cheque);
#endif
#ifdef ENABLE_GUILD_RANKING_SYSTEM
ACMD(do_get_guild_rank_list);
#endif
#ifdef ENABLE_SPECIAL_GACHA_SYSTEM
ACMD(do_change_special_gacha);
ACMD(do_get_special_gacha_item);
#endif
#ifdef ENABLE_FAST_SOUL_REFINE
ACMD(do_open_soul_window);
#endif
#ifdef ENABLE_BOSS_MANAGER_SYSTEM
ACMD(do_get_boss_data);
#endif

struct command_info cmd_info[] =
{
	{ "get_warp", do_get_warp, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"!RESERVED!", nullptr, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"who", do_who, 0, POS_DEAD, GM_LOW_WIZARD},
	{"war", do_war, 0, POS_DEAD, GM_PLAYER},
	{"warp", do_warp, 0, POS_DEAD, GM_LOW_WIZARD},
	{"user", do_user, 0, POS_DEAD, GM_LOW_WIZARD},
	{"notice", do_notice, 0, POS_DEAD, GM_LOW_WIZARD},
	{"notice_map", do_map_notice, 0, POS_DEAD, GM_LOW_WIZARD},
	{"big_notice", do_big_notice, 0, POS_DEAD, GM_HIGH_WIZARD},
	{"nowar", do_nowar, 0, POS_DEAD, GM_PLAYER},
	{"purge", do_purge, 0, POS_DEAD, GM_WIZARD},
	{"weaken", do_weaken, 0, POS_DEAD, GM_GOD},
	{"dc", do_disconnect, 0, POS_DEAD, GM_LOW_WIZARD},
	{"transfer", do_transfer, 0, POS_DEAD, GM_LOW_WIZARD},
	{"goto", do_goto, 0, POS_DEAD, GM_LOW_WIZARD},
	{"level", do_level, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"eventflag", do_event_flag, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"geteventflag", do_get_event_flag, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"item", do_item, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"mob", do_mob, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"mob_ld", do_mob_ld, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"ma", do_mob_aggresive, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"mc", do_mob_coward, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"mm", do_mob_map, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"kill", do_kill, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"ipurge", do_item_purge, 0, POS_DEAD, GM_LOW_WIZARD},
	{"group", do_group, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"grrandom", do_group_random, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"set", do_set, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"reset", do_reset, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"greset", do_greset, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"advance",	do_advance, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"book", do_book, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"console", do_console, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"shutdow",	do_inputall, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"shutdown", do_shutdown, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"stat", do_stat, 0, POS_DEAD, GM_PLAYER},
	{"stat-", do_stat_minus, 0, POS_DEAD, GM_PLAYER},
	{"stat_reset", do_stat_reset, 0, POS_DEAD, GM_LOW_WIZARD},
	{"state", do_state, 0, POS_DEAD, GM_LOW_WIZARD},
	{"stun", do_stun, 0, POS_DEAD, GM_LOW_WIZARD},
	{"slow", do_slow, 0, POS_DEAD, GM_LOW_WIZARD},
	{"poison", do_poison, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"respawn", do_respawn, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"makeguild", do_makeguild, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"deleteguild", do_deleteguild, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"mount", do_mount, 0, POS_MOUNTING, GM_PLAYER},
	{"restart_here", do_restart, SCMD_RESTART_HERE, POS_DEAD, GM_PLAYER},
	{"restart_town", do_restart, SCMD_RESTART_TOWN, POS_DEAD, GM_PLAYER},
	{"phase_selec",	do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"phase_select", do_cmd, SCMD_PHASE_SELECT, POS_DEAD, GM_PLAYER},
	{"qui", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"quit", do_cmd, SCMD_QUIT, POS_DEAD, GM_PLAYER},
	{"logou", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"logout", do_cmd, SCMD_LOGOUT, POS_DEAD, GM_PLAYER},
	{"skillup",	do_skillup, 0, POS_DEAD, GM_PLAYER},
	{"gskillup", do_guildskillup, 0, POS_DEAD,	GM_PLAYER},
	{"pvp", do_pvp, 0, POS_DEAD, GM_PLAYER},
	{"safebox",	do_safebox_size, 0, POS_DEAD, GM_HIGH_WIZARD},
	{"safebox_close", do_safebox_close,	0, POS_DEAD, GM_PLAYER},
	{"safebox_passwor", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"safebox_password", do_safebox_password, 0, POS_DEAD, GM_PLAYER},
	{"safebox_change_passwor", do_inputall,	0, POS_DEAD, GM_PLAYER},
	{"safebox_change_password", do_safebox_change_password,	0, POS_DEAD, GM_PLAYER},
	{"mall_passwor", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"mall_password", do_mall_password, 0, POS_DEAD, GM_PLAYER},
	{"mall_close", do_mall_close, 0, POS_DEAD, GM_PLAYER},
	{"ungroup",	do_ungroup, 0, POS_DEAD, GM_PLAYER},
	{"refine_rod", do_refine_rod, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"refine_pick",	do_refine_pick, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"max_pick", do_max_pick, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"fish_simul", do_fishing_simul, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"invisible", do_invisibility, 0, POS_DEAD,	GM_LOW_WIZARD},
	{"qf", do_qf, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"clear_quest",	do_clear_quest, 0, POS_DEAD, GM_HIGH_WIZARD},
	{"close_shop", do_close_shop, 0, POS_DEAD, GM_PLAYER},
	{"set_walk_mode", do_set_walk_mode, 0, POS_DEAD, GM_PLAYER},
	{"set_run_mode", do_set_run_mode, 0, POS_DEAD, GM_PLAYER},
	{"setjob", do_set_skill_group, 0, POS_DEAD,	GM_IMPLEMENTOR},
	{"setskill", do_setskill, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"setskillother", do_setskillother,	0, POS_DEAD, GM_IMPLEMENTOR},
	{"setskillpoint", do_set_skill_point, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"reload", do_reload, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"cooltime", do_cooltime, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"gwlist", do_gwlist, 0, POS_DEAD, GM_LOW_WIZARD},
	{"gwstop", do_stop_guild_war, 0, POS_DEAD, GM_LOW_WIZARD},
	{"gwcancel", do_cancel_guild_war, 0, POS_DEAD, GM_LOW_WIZARD},
	{"gstate", do_guild_state, 0, POS_DEAD,	GM_LOW_WIZARD},
	{"pkmode", do_pkmode, 0, POS_DEAD, GM_PLAYER},
	{"messenger_auth", do_messenger_auth, 0, POS_DEAD, GM_PLAYER},
	{"getqf", do_getqf, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"setqf", do_setqf, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"delqf", do_delqf, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"set_state", do_set_state, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"detaillog", do_detaillog, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"monsterlog", do_monsterlog, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"forgetme", do_forgetme, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"aggregate", do_aggregate, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"attract_ranger", do_attract_ranger, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"pull_monster", do_pull_monster, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"setblockmode", do_setblockmode, 0, POS_DEAD, GM_PLAYER},
	{"polymorph", do_polymorph, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"polyitem", do_polymorph_item,	0, POS_DEAD, GM_IMPLEMENTOR},
	{"priv_empire", do_priv_empire, 0, POS_DEAD, GM_HIGH_WIZARD},
	{"priv_guild", do_priv_guild, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"mount_test", do_mount_test, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"unmount",	do_unmount, 0, POS_DEAD, GM_PLAYER},
	{"private",	do_private, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"party_request", do_party_request, 0, POS_DEAD, GM_PLAYER},
	{"party_request_accept", do_party_request_accept, 0, POS_DEAD, GM_PLAYER},
	{"party_request_deny", do_party_request_deny, 0, POS_DEAD, GM_PLAYER},
	{"observer", do_observer, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"observer_exit", do_observer_exit, 0, POS_DEAD, GM_PLAYER},
	{"socketitem", do_socket_item, 0, POS_DEAD,	GM_IMPLEMENTOR},
	{"saveati",	do_save_attribute_to_image, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"xmas_boom", do_xmas, SCMD_XMAS_BOOM, POS_DEAD, GM_IMPLEMENTOR},
	{"xmas_snow", do_xmas, SCMD_XMAS_SNOW, POS_DEAD, GM_IMPLEMENTOR},
	{"xmas_santa", do_xmas, SCMD_XMAS_SANTA, POS_DEAD, GM_IMPLEMENTOR},
#ifdef ENABLE_MUSIC_EVENT
	{ "xmas_muzik",	do_xmas, SCMD_XMAS_MUZIK, POS_DEAD, GM_HIGH_WIZARD},
#endif
	{"jy", do_block_chat, 0, POS_DEAD, GM_LOW_WIZARD},
	{"vote_block_chat", do_vote_block_chat, 0, POS_DEAD, GM_PLAYER},
	{"block_chat", do_block_chat, 0, POS_DEAD, GM_LOW_WIZARD},
	{"block_chat_list", do_block_chat_list, 0, POS_DEAD, GM_LOW_WIZARD},
	{"build", do_build, 0, POS_DEAD, GM_PLAYER},
	{"clear_land", do_clear_land, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"affect_remove", do_affect_remove, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"horse_state",	do_horse_state, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"horse_level",	do_horse_level, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"horse_ride", do_horse_ride, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"horse_summon", do_horse_summon, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"horse_unsummon", do_horse_unsummon, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"horse_set_stat", do_horse_set_stat, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"emotion_allow", do_emotion_allow, 0, POS_FIGHTING, GM_PLAYER},
	{"kiss", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"slap", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"french_kiss", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"clap", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"cheer1", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"cheer2", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"dance1", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"dance2", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"dance3", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"dance4", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"dance5", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"dance6", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"congratulation", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
	{"forgive", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"angry", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"attractive", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"sad", do_emotion,	0, POS_FIGHTING, GM_PLAYER},
	{"shy", do_emotion,	0, POS_FIGHTING, GM_PLAYER},
	{"cheerup", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"banter", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"joy", do_emotion,	0, POS_FIGHTING, GM_PLAYER},
	{"change_attr", do_change_attr, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"add_attr", do_add_attr, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"add_socket", do_add_socket, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"user_horse_ride",	do_user_horse_ride, 0, POS_FISHING,	GM_PLAYER},
	{"user_horse_back",	do_user_horse_back, 0, POS_FISHING,	GM_PLAYER},
	{"user_horse_feed",	do_user_horse_feed, 0, POS_FISHING,	GM_PLAYER},
	{"show_arena_list",	do_show_arena_list, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"end_all_duel", do_end_all_duel, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"end_duel", do_end_duel, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"duel", do_duel, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"con+", do_stat_plus_amount, POINT_HT, POS_DEAD, GM_PLAYER},
	{"int+", do_stat_plus_amount, POINT_IQ, POS_DEAD, GM_PLAYER},
	{"str+", do_stat_plus_amount, POINT_ST, POS_DEAD, GM_PLAYER},
	{"dex+", do_stat_plus_amount, POINT_DX, POS_DEAD, GM_PLAYER},
	{"break_marriage", do_break_marriage, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"show_quiz", do_oxevent_show_quiz, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"log_oxevent", do_oxevent_log, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"get_oxevent_att", do_oxevent_get_attender, 0,	POS_DEAD, GM_IMPLEMENTOR},
	{"effect", do_effect, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"hair", do_hair, 0, POS_DEAD, GM_PLAYER},
	{"inventory", do_inventory, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"cube", do_cube, 0, POS_DEAD, GM_PLAYER},
	{"temp", do_temp, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"reset_subskill", do_reset_subskill, 0, POS_DEAD, GM_HIGH_WIZARD},
	{"flush", do_flush, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"gift", do_gift, 0,  POS_DEAD, GM_PLAYER},
	{"eclipse", do_eclipse, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"eventhelper", do_event_helper, 0,	POS_DEAD, GM_IMPLEMENTOR},
	{"in_game_mall", do_in_game_mall, 0, POS_DEAD, GM_PLAYER},
	{"get_mob_count", do_get_mob_count, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"dice", do_dice, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"special_item", do_special_item, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"click_mall", do_click_mall, 0, POS_DEAD, GM_PLAYER},
	{"click_safebox", do_click_safebox, 0, POS_DEAD, GM_PLAYER},
	{"ride", do_ride, 0, POS_DEAD, GM_PLAYER},
	{"item_id_list", do_get_item_id_list, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"set_socket", do_set_socket, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"tcon", do_set_stat, POINT_HT,	POS_DEAD, GM_IMPLEMENTOR},
	{"tint", do_set_stat, POINT_IQ,	POS_DEAD, GM_IMPLEMENTOR},
	{"tstr", do_set_stat, POINT_ST,	POS_DEAD, GM_IMPLEMENTOR},
	{"tdex", do_set_stat, POINT_DX,	POS_DEAD, GM_IMPLEMENTOR},
	{"cannot_dead", do_can_dead, 1,	POS_DEAD, GM_IMPLEMENTOR},
	{"can_dead", do_can_dead, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"full_set", do_full_set, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"item_full_set", do_item_full_set, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"attr_full_set", do_attr_full_set, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"all_skill_master", do_all_skill_master, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"use_item", do_use_item, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"dragon_soul", do_dragon_soul, 0, POS_DEAD, GM_PLAYER},
	{"ds_list", do_ds_list, 0, POS_DEAD, GM_PLAYER},
	{"do_clear_affect", do_clear_affect, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"rewarp", do_rewarp, 0, POS_DEAD, GM_LOW_WIZARD},
#ifdef ENABLE_GUILD_SPY_SYSTEM
	{"kovkopegi", do_ajanisik, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_GUILD_NOTICE_SYSTEM
	{"guild_notice", do_guild_notice, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
	{"view_equip", do_view_equip_request, 0, POS_DEAD, GM_PLAYER},
	{"view_equip_accept", do_view_equip_request_accept, 0, POS_DEAD, GM_PLAYER},
	{"view_equip_deny", do_view_equip_request_deny, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_MINIGAME_RUMI_EVENT
	{"cards", do_cards, 0,	POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_GIFTBOX_SYSTEM
	{"gift_refresh", do_gift_refresh, 0, POS_DEAD, GM_PLAYER},
	{"gift_get", do_gift_get, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	{"refresh_shop", do_shop_refresh, 0, POS_DEAD, GM_PLAYER},
	{"shop_yang", do_shop_yang, 0, POS_DEAD, GM_PLAYER},
	{"shop_won", do_shop_won, 0, POS_DEAD, GM_PLAYER},
	{"delete_shop", do_shop_delete, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"shop_open", do_shop_open, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
	{"cubepetadd", do_CubePetAdd, 0, POS_DEAD, GM_PLAYER},
	{"feedcubepet",	do_FeedCubePet, 0, POS_DEAD, GM_PLAYER},
	{"petskills", do_PetSkill, 0,	POS_DEAD, GM_PLAYER},
	{"petvoincrease", do_PetEvo, 0,	POS_DEAD, GM_PLAYER},
	{"pet_change_name", do_PetChangeName, 0, POS_DEAD, GM_PLAYER},
	{"petincreaseskill", do_IncreasePetSkill, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_CHANNEL_SWITCH_SYSTEM
	{"channel", do_change_channel, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	{"item_comb", do_item_comb, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_FAST_SOUL_STONE_SYSTEM
	{"ruhoku", do_ruhoku, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_FAST_SKILL_BOOK_SYSTEM
	{"bkoku", do_bkoku,	0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_EXTENDED_BIOLOG_SYSTEM
	{"biyologver", do_biyolog, 0, POS_DEAD,	GM_PLAYER},
#endif
#ifdef ENABLE_GAME_OPTION_SYSTEM
	{"health_board_config", do_health_board_config, 0, POS_DEAD, GM_PLAYER},
	{"sash_config", do_sash_config, 0, POS_DEAD, GM_PLAYER},
	{"costume_config", do_costume_config, 0, POS_DEAD, GM_PLAYER},
	{"costume_w_config", do_costume_w_config, 0, POS_DEAD, GM_PLAYER},
	{"costume_h_config", do_costume_h_config, 0, POS_DEAD, GM_PLAYER},	
	{"level_view_config", do_level_view_config, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_ATTENDANCE_EVENT
	{"attendance", do_attendance, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"easter_event", do_easter_event, 0, POS_DEAD, GM_IMPLEMENTOR},
#endif
#ifdef ENABLE_WOLFMAN_CHARACTER
	{"bleeding", do_bleeding, 0, POS_DEAD, GM_IMPLEMENTOR},
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
	{"evo_point", do_evo_points, 0, POS_DEAD, GM_LOW_WIZARD},
	{"refine_evo", do_refine_evo_points, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_FULL_NOTICE
	{"big_notice_map", do_map_big_notice, 0, POS_DEAD, GM_HIGH_WIZARD},
	{"notice_test", do_notice_test, 0, POS_DEAD, GM_HIGH_WIZARD},
	{"big_notice_test", do_big_notice_test, 0, POS_DEAD, GM_HIGH_WIZARD},
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	{"open_c", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"open_cz", do_open_cz, 0, POS_DEAD, GM_PLAYER},
	{"cz_rewar", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"cz_reward", do_cz_reward, 0, POS_DEAD, GM_PLAYER},
	{"cz_check_bo", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"cz_check_box", do_cz_check_box, 0, POS_DEAD, GM_PLAYER},
	{"reviv", do_inputall, 0, POS_DEAD,	GM_PLAYER},
	{"revive", do_revive, 0, POS_DEAD, GM_PLAYER},
	{"revivedialo", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"revivedialog", do_revivedialog, 0, POS_DEAD, GM_PLAYER},
	{"jumpfloo", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"jumpfloor", do_jump_floor, 0, POS_DEAD, GM_PLAYER},
	{"nextfloo", do_inputall, 0, POS_DEAD, GM_PLAYER},
	{"nextfloor", do_next_floor, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_GUILD_HISTORY_SYSTEM
	{"guild_history_req", do_guild_history_req, 0, POS_DEAD, GM_PLAYER},
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	{"restart_combat_zone",	do_restart, SCMD_RESTART_COMBAT_ZONE, POS_DEAD,	GM_PLAYER},
#endif
#ifdef ENABLE_FAST_SKILL_SELECT_SYSTEM
	{"skill_select", do_skill_select, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_MINI_GAME_CATCH_KING
	{"catch_king_event", do_catch_king_event, 0, POS_DEAD, GM_IMPLEMENTOR},
#endif
#ifdef ENABLE_DANCE_EVENT_SYSTEM
	{"gunsam_all", do_gunsam_all, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"gunsam_config", do_gunsam_config, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_CHEQUE_COUPON_SYSTEM
	{"use_won_ticket", do_use_won_ticket, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_PET_ATTR_DETERMINE
	{"determine_pet", do_determine_pet, 0, POS_DEAD, GM_PLAYER},
	{"change_pet", do_change_pet, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_CAPTCHA_SYSTEM
	{"captcha", do_captcha, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_EMOTION_SYSTEM
	{"selfie", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"dance7", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"doze", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"exercise", do_emotion, 0, POS_FIGHTING, GM_PLAYER},
	{"pushup", do_emotion, 0, POS_FIGHTING,	GM_PLAYER},
#endif
#ifdef ENABLE_DEBUG_MODE_SYSTEM
	{"sdb", do_set_debug_mode, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_RANGE_NPC_SYSTEM
	{"open_range_npc", do_open_range_npc, 0, POS_DEAD, GM_PLAYER},
#endif
	{"set_feature_enabled", do_set_feature_enabled, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"set_feature_disabled", do_set_feature_disabled, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"feature_state", do_feature_state, 0, POS_DEAD, GM_IMPLEMENTOR},
#ifdef ENABLE_AFFECT_POLYMORPH_REMOVE
	{"remove_polymorph", do_remove_polymorph, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_SKILL_AFFECT_REMOVE
	{"remove_skill_affect", do_remove_skill_affect, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_FAST_CHEQUE_GOLD_TRANSFER
	{"transfer_gold", do_transfer_gold, 0, POS_DEAD, GM_PLAYER},
	{"transfer_cheque", do_transfer_cheque, 0, POS_DEAD, GM_PLAYER},
#endif
#ifdef ENABLE_GUILD_RANKING_SYSTEM
	{ "get_guild_rank_list", do_get_guild_rank_list, 0,	POS_DEAD, GM_PLAYER	},
#endif
#ifdef ENABLE_SPECIAL_GACHA_SYSTEM
	{ "change_special_gacha", do_change_special_gacha, 0, POS_DEAD, GM_PLAYER },
	{ "get_special_gacha_item", do_get_special_gacha_item, 0, POS_DEAD, GM_PLAYER },
#endif
#ifdef ENABLE_FAST_SOUL_REFINE
	{ "open_soul_window", do_open_soul_window, 0, POS_DEAD, GM_PLAYER },
#endif
#ifdef ENABLE_BOSS_MANAGER_SYSTEM
	{ "get_boss_data",		do_get_boss_data,			0,			POS_DEAD,	GM_PLAYER	},
#endif
	{"mm", do_spawnmetinacords, 0, POS_DEAD, GM_IMPLEMENTOR},
	{"\n", nullptr, 0, POS_DEAD, GM_IMPLEMENTOR}  /* 반드시 이 것이 마지막이어야 한다. */
};

void interpreter_set_privilege(const char* cmd, int lvl)
{
	int i;

	for (i = 0; *cmd_info[i].command != '\n'; ++i)
	{
		if (!str_cmp(cmd, cmd_info[i].command))
		{
			cmd_info[i].gm_level = lvl;
			sys_log(0, "Setting command privilege: %s -> %d", cmd, lvl);
			break;
		}
	}
}

void double_dollar(const char* src, size_t src_len, char* dest, size_t dest_len)
{
	const char* tmp = src;
	size_t cur_len = 0;

	// \0 넣을 자리 확보
	dest_len -= 1;

	while (src_len-- && *tmp)
	{
		if (*tmp == '$')
		{
			if (cur_len + 1 >= dest_len)
				break;

			*(dest++) = '$';
			*(dest++) = *(tmp++);
			cur_len += 2;
		}
		else
		{
			if (cur_len >= dest_len)
				break;

			*(dest++) = *(tmp++);
			cur_len += 1;
		}
	}

	*dest = '\0';
}

void interpret_command(LPCHARACTER ch, const char* argument, size_t len)
{
#ifdef ENABLE_ANTI_CMD_FLOOD
	if (ch && !ch->IsGM())
	{
		if (thecore_pulse() > ch->GetCmdAntiFloodPulse() + PASSES_PER_SEC(1))
		{
			ch->SetCmdAntiFloodCount(0);
			ch->SetCmdAntiFloodPulse(thecore_pulse());
		}
		if (ch->IncreaseCmdAntiFloodCount() >= 10)
		{
			ch->GetDesc()->DelayedDisconnect(0);
			return;
		}
	}
#endif
	if (nullptr == ch)
	{
		sys_err("nullptr CHRACTER");
		return;
	}

	char cmd[1024 + 1];  // buffer overflow 문제가 생기지 않도록 일부러 길이를 짧게 잡음
	char new_line[2048 + 1];
	const char* line;
	int icmd;

	if (len == 0 || !*argument)
		return;

	double_dollar(argument, len, new_line, sizeof(new_line));

	size_t cmdlen;
	line = first_cmd(new_line, cmd, sizeof(cmd), &cmdlen);

	for (icmd = 1; *cmd_info[icmd].command != '\n'; ++icmd)
	{
		if (cmd_info[icmd].command_pointer == do_cmd)
		{
			if (!strcmp(cmd_info[icmd].command, cmd)) // do_cmd는 모든 명령어를 쳐야 할 수 있다.
				break;
		}
		else if (!strncmp(cmd_info[icmd].command, cmd, cmdlen))
			break;
	}

	if (ch->GetPosition() < cmd_info[icmd].minimum_position)
	{
		switch (ch->GetPosition())
		{
		case POS_MOUNTING:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("탄 상태에서는 할 수 없습니다."));
			break;

		case POS_DEAD:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("쓰러진 상태에서는 할 수 없습니다."));
			break;

		case POS_SLEEPING:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("꿈속에서 어떻게요?"));
			break;

		case POS_RESTING:
		case POS_SITTING:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("먼저 일어 나세요."));
			break;
			/*
			   case POS_FIGHTING:
			   ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("목숨을 걸고 전투 중 입니다. 집중 하세요."));
			   break;
			 */
		default:
			sys_err("unknown position %d", ch->GetPosition());
			break;
		}

		return;
	}

	if (*cmd_info[icmd].command == '\n')
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("그런 명령어는 없습니다"));
		return;
	}

	if (cmd_info[icmd].gm_level && cmd_info[icmd].gm_level > ch->GetGMLevel())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("그런 명령어는 없습니다"));
		return;
	}

	if (strncmp("phase", cmd_info[icmd].command, 5) != 0) // 히든 명령어 처리
		sys_log(0, "COMMAND: %s: %s", ch->GetName(), cmd_info[icmd].command);

	((*cmd_info[icmd].command_pointer) (ch, line, icmd, cmd_info[icmd].subcmd));

	if (ch->GetGMLevel() >= GM_LOW_WIZARD)
	{
		if (cmd_info[icmd].gm_level >= GM_LOW_WIZARD)
		{
			char buf[1024];
			snprintf(buf, sizeof(buf), "%s", argument);

			LogManager::instance().GMCommandLog(ch->GetPlayerID(), ch->GetName(), ch->GetDesc()->GetHostName(), g_bChannel, buf);
		}
	}
}