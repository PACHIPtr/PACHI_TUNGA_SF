import uiScriptLocale
import localeInfo

ROOT_PATH = "d:/ymir work/ui/public/"

TEMPORARY_X = +43
BUTTON_TEMPORARY_X = 5
PVP_X = -10

LINE_LABEL_X 	= 30
LINE_DATA_X 	= 90
LINE_BEGIN	= 40
LINE_STEP	= 25
SMALL_BUTTON_WIDTH 	= 45
MIDDLE_BUTTON_WIDTH 	= 65

window = {
	"name" : "GameOptionDialog",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : 300,
	"height" : 358,

	"children" :
	[
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 300,
			"height" : 25*11+8-10+25+25,

			"children" :
			[
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 284,
					"color" : "gray",

					"children" :
					(
						{ "name":"titlename", "type":"text", "x":0, "y":3, 
						"text" : uiScriptLocale.GAMEOPTION_TITLE, 
						"horizontal_align":"center", "text_horizontal_align":"center" },
					),
				},

				{
					"name" : "Board_Name2",
					"type" : "new_board",

					"x" : 7,
					"y" : 30,
	
					"width" : 282,
					"height" : 258,

				},

				{
					"name" : "NextPage",
					"type" : "button",
					"x" : 180,
					"y" : 295,
					"default_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_01.sub",
					"over_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_01.sub",
					"down_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_02.sub",
				},
				{
					"name" : "PrevPage",
					"type" : "button",
					"x" : 105,
					"y" : 295,
					"default_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_01.sub",
					"over_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_01.sub",
					"down_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_02.sub",
				},

				## 이름색
				{
					"name" : "name_color",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 40+2,

					"text" : uiScriptLocale.OPTION_NAME_COLOR,
				},
				{
					"name" : "name_color_normal",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0,
					"y" : 40,

					"text" : uiScriptLocale.OPTION_NAME_COLOR_NORMAL,

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				{
					"name" : "name_color_empire",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1,
					"y" : 40,

					"text" : uiScriptLocale.OPTION_NAME_COLOR_EMPIRE,

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},

				## 타겟창
				{
					"name" : "target_board",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 65+2,

					"text" : uiScriptLocale.OPTION_TARGET_BOARD,
				},
				{
					"name" : "target_board_no_view",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0,
					"y" : 65,

					"text" : uiScriptLocale.OPTION_TARGET_BOARD_NO_VIEW,

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				{
					"name" : "target_board_view",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1,
					"y" : 65,

					"text" : uiScriptLocale.OPTION_TARGET_BOARD_VIEW,

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},

				
				## PvP Mode
				{
					"name" : "pvp_mode",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 90+2,

					"text" : uiScriptLocale.OPTION_PVPMODE,
				},
				{
					"name" : "pvp_peace",
					"type" : "radio_button",

					"x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*0,
					"y" : 90,

					"text" : uiScriptLocale.OPTION_PVPMODE_PEACE,
					"tooltip_text" : uiScriptLocale.OPTION_PVPMODE_PEACE_TOOLTIP,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				{
					"name" : "pvp_revenge",
					"type" : "radio_button",

					"x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*1,
					"y" : 90,

					"text" : uiScriptLocale.OPTION_PVPMODE_REVENGE,
					"tooltip_text" : uiScriptLocale.OPTION_PVPMODE_REVENGE_TOOLTIP,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				{
					"name" : "pvp_guild",
					"type" : "radio_button",

					"x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*2,
					"y" : 90,

					"text" : uiScriptLocale.OPTION_PVPMODE_GUILD,
					"tooltip_text" : uiScriptLocale.OPTION_PVPMODE_GUILD_TOOLTIP,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				{
					"name" : "pvp_free",
					"type" : "radio_button",

					"x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*3,
					"y" : 90,

					"text" : uiScriptLocale.OPTION_PVPMODE_FREE,
					"tooltip_text" : uiScriptLocale.OPTION_PVPMODE_FREE_TOOLTIP,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},

				## Block
				{
					"name" : "block",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 115+2,

					"text" : uiScriptLocale.OPTION_BLOCK,
				},
				{
					"name" : "block_exchange_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0,
					"y" : 115,

					"text" : uiScriptLocale.OPTION_BLOCK_EXCHANGE,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "block_party_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1,
					"y" : 115,

					"text" : uiScriptLocale.OPTION_BLOCK_PARTY,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "block_guild_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*2,
					"y" : 115,

					"text" : uiScriptLocale.OPTION_BLOCK_GUILD,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "block_whisper_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0,
					"y" : 140,

					"text" : uiScriptLocale.OPTION_BLOCK_WHISPER,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "block_friend_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1,
					"y" : 140,

					"text" : uiScriptLocale.OPTION_BLOCK_FRIEND,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "block_party_request_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*2,
					"y" : 140,

					"text" : uiScriptLocale.OPTION_BLOCK_PARTY_REQUEST,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "block_view_equipment_button",
					"type" : "toggle_button",


					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0,
					"y" : 165,


					"text" : uiScriptLocale.OPTION_BLOCK_EQUIPMENT_REQUEST,


					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				## Chat
				{
					"name" : "chat",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 190+2,

					"text" : uiScriptLocale.OPTION_VIEW_CHAT,
				},
				{
					"name" : "view_chat_on_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 190,

					"text" : uiScriptLocale.OPTION_VIEW_CHAT_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "view_chat_off_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 190,

					"text" : uiScriptLocale.OPTION_VIEW_CHAT_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},

				## Always Show Name
				{
					"name" : "always_show_name",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 215+2,

					"text" : uiScriptLocale.OPTION_ALWAYS_SHOW_NAME,
				},
				{
					"name" : "always_show_name_all_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 215,

					"text" : uiScriptLocale.OPTION_ALWAYS_SHOW_NAME_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "always_show_name_item_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 215,

					"text" : uiScriptLocale.OPTION_ALWAYS_SHOW_NAME_ITEM,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "always_show_name_off_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*2,
					"y" : 215,

					"text" : uiScriptLocale.OPTION_ALWAYS_SHOW_NAME_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},

				## Effect On/Off
				{
					"name" : "effect_on_off",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 240+2,

					"text" : uiScriptLocale.OPTION_EFFECT,
				},
				{
					"name" : "show_damage_on_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 240,

					"text" : uiScriptLocale.OPTION_VIEW_CHAT_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "show_damage_off_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 240,

					"text" : uiScriptLocale.OPTION_VIEW_CHAT_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},

				## 판매문구
				{
					"name" : "salestext_on_off",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 265+2,

					"text" : uiScriptLocale.OPTION_SALESTEXT,
				},
				{
					"name" : "salestext_on_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 265,

					"text" : uiScriptLocale.OPTION_SALESTEXT_VIEW_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "salestext_off_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 265,

					"text" : uiScriptLocale.OPTION_SALESTEXT_VIEW_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				##### PAGE 2
				{
					"name" : "show_offline_shop_name",
					"type" : "text",

					"multi_line" : 1,

					"x" : LINE_LABEL_X,
					"y" : 40+2,

					"text" : uiScriptLocale.OPTION_OFFLINE_SHOPS,
				},
				{
					"name" : "show_offline_shop_on",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 40,

					"text" : uiScriptLocale.OPTION_SHOW,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "show_offline_shop_off",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1,
					"y" : 40,

					"text" : uiScriptLocale.OPTION_HIDE,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "show_companions",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 65+2,

					"text" : uiScriptLocale.OPTION_SHOW_COMPANIONS,
				},
				{
					"name" : "show_companions_pets_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X,
					"y" : 65,

					"text" : uiScriptLocale.OPTION_SHOW_COMPANIONS_PETS,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "show_companions_mounts_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 65,

					"text" : uiScriptLocale.OPTION_SHOW_COMPANIONS_MOUNTS,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "show_companions_shaman_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+MIDDLE_BUTTON_WIDTH,
					"y" : 65,

					"text" : uiScriptLocale.OPTION_SHOW_COMPANIONS_SHAMANS,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "show_mob_info",
					"type" : "text",

					"multi_line" : 1,

					"x" : LINE_LABEL_X,
					"y" : 90+2,

					"text" : uiScriptLocale.OPTION_MOB_INFO,
				},
				{
					"name" : "show_mob_level_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X,
					"y" : 90,

					"text" : uiScriptLocale.OPTION_MOB_INFO_LEVEL,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "show_mob_AI_flag_button",
					"type" : "toggle_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 90,

					"text" : uiScriptLocale.OPTION_MOB_INFO_AGGR,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "gold_on_off",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 115+2,

					"text" : uiScriptLocale.OPTION_YANG_SHOW,
				},
				{
					"name" : "gold_on_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 115,

					"text" : uiScriptLocale.OPTION_YANG_SHOW_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "gold_off_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 115,

					"text" : uiScriptLocale.OPTION_YANG_SHOW_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "chat_filter",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 140+2,

					"text" : uiScriptLocale.OPTION_CHAT_FILTER,
				},
				{
					"name" : "chat_filter_dice",
					"type" : "toggle_button",

					"x" : LINE_DATA_X,
					"y" : 140,

					"text" : uiScriptLocale.OPTION_CHAT_FILTER_DICE,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "healthboard_on_off",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 165+2,

					"text" : uiScriptLocale.OPTION_HEALTH_BOARD,
				},
				{
					"name" : "healthboard_on_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 165,

					"text" : uiScriptLocale.OPTION_HEALTH_BOARD_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "healthboard_off_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 165,

					"text" : uiScriptLocale.OPTION_HEALTH_BOARD_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "autopickup_on_off",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 190+2,

					"text" : uiScriptLocale.OPTION_AUTO_PICKUP,
				},
				{
					"name" : "autopickup_speed_on_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 190,

					"text" : uiScriptLocale.OPTION_AUTO_PICKUP_ON_SPEED,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "autopickup_default_on_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 190,

					"text" : uiScriptLocale.OPTION_AUTO_PICKUP_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "autopickup_off_button",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+MIDDLE_BUTTON_WIDTH,
					"y" : 190,

					"text" : uiScriptLocale.OPTION_AUTO_PICKUP_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "keyboardText",
					"type" : "text",
					"x" : LINE_LABEL_X,
					"y" : 215,
					"text" : uiScriptLocale.OPTION_KEYBOARDSETTINGS,
				},
				{
					"name" : "keyboardButton",
					"type" : "button",
					"x" : LINE_DATA_X,
					"y" : 215,
					"text" : uiScriptLocale.OPTION_EDIT,
					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "gunsam_mode_button_text",
					"type" : "text",

					"x" : LINE_LABEL_X,
					"y" : 240+2,

					"text" : uiScriptLocale.OPTION_GUNSAM_MODE,
				},
				{
					"name" : "gunsam_button_on",
					"type" : "radio_button",

					"x" : LINE_DATA_X,
					"y" : 240,

					"text" : uiScriptLocale.OPTION_GUNSAM_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "gunsam_button_off",
					"type" : "radio_button",

					"x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH,
					"y" : 240,

					"text" : uiScriptLocale.OPTION_GUNSAM_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
			],
		},
	],
}