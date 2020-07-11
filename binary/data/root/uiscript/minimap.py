ROOT = "d:/ymir work/ui/minimap/"
ROOT_PATH = "d:/ymir work/ui/public/"
import app
import constInfo
import serverInfo
import uiScriptLocale
import localeInfo

window = {
	"name" : "MiniMap",

	"x" : SCREEN_WIDTH - 136,
	"y" : 0,

	"width" : 136,
	"height" : 142,

	"children" :
	[
		## OpenWindow
		{
			"name" : "OpenWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 136,
			"height" : 142,

			"children" :
			[
				{
					"name" : "OpenWindowBGI",
					"type" : "image",
					"x" : 0,
					"y" : 0,
					"image" : ROOT + "minimap.sub",
				},
				## MiniMapWindow
				{
					"name" : "MiniMapWindow",
					"type" : "window",

					"x" : 4,
					"y" : 5,

					"width" : 128,
					"height" : 128,
				},
				## ScaleUpButton
				{
					"name" : "ScaleUpButton",
					"type" : "button",

					"x" : 101,
					"y" : 116,

					"default_image" : ROOT + "minimap_scaleup_default.sub",
					"over_image" : ROOT + "minimap_scaleup_over.sub",
					"down_image" : ROOT + "minimap_scaleup_down.sub",
				},
				## ScaleDownButton
				{
					"name" : "ScaleDownButton",
					"type" : "button",

					"x" : 115,
					"y" : 103,

					"default_image" : ROOT + "minimap_scaledown_default.sub",
					"over_image" : ROOT + "minimap_scaledown_over.sub",
					"down_image" : ROOT + "minimap_scaledown_down.sub",
				},
				## MiniMapHideButton
				{
					"name" : "MiniMapHideButton",
					"type" : "button",

					"x" : 111,
					"y" : 6,

					"default_image" : ROOT + "minimap_close_default.sub",
					"over_image" : ROOT + "minimap_close_over.sub",
					"down_image" : ROOT + "minimap_close_down.sub",
				},
				## AtlasShowButton
				# {
					# "name" : "cerceve1",
					# "type" : "image",
					# "image" : ROOT+"cerceve.tga",
					# "x" : -11,
					# "y" : 83,
				# },
				# {
					# "name" : "cerceve2",
					# "type" : "image",
					# "image" : ROOT+"cerceve.tga",
					# "x" : 27,
					# "y" : 122,
				# },
				## BUNLARI BUTON OLARAK DEGISTIRCEN
				{
					"name" : "l_button",
					"type" : "button",
					"x" : -8,
					"y" : 84,
					"default_image" : ROOT + "dragoneye.dds",
					"over_image" : ROOT + "dragoneye.dds",
					"down_image" : ROOT + "dragoneye.dds",
					#"tooltip_text" : localeInfo.MINIMAP_SWITCH_BOT,
				},
				{
					"name" : "p_button",
					"type" : "button",
					"x" : 33,
					"y" : 127,
					"default_image" : "d:/ymir work/ui/boss_follow/boss_open_default.tga",
					"over_image" : "d:/ymir work/ui/boss_follow/boss_open_over.tga",
					"down_image" : "d:/ymir work/ui/boss_follow/boss_open_down.tga",
					#"tooltip_text" : localeInfo.MINIMAP_BOSS,
				},

				## AtlasShowButton
				{
					"name" : "AtlasShowButton",
					"type" : "button",

					"x" : -15,
					"y" : 57,

					"default_image" : ROOT + "atlas_open_default.sub",
					"over_image" : ROOT + "atlas_open_over.sub",
					"down_image" : ROOT + "atlas_open_down.sub",
				},
				## BattleButton
				{
					"name" : "BattleButton",
					"type" : "button",

					"x" : 6,
					"y" : 105,

					"default_image" : ROOT + "battle_open_default.tga",
					"over_image" : ROOT + "battle_open_over.tga",
					"down_image" : ROOT + "battle_open_down.tga",
				},
				## ServerInfo
				{
					"name" : "ServerInfo",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 155,

					"text" : "",
				},

				## PositionInfo
				{
					"name" : "PositionInfo",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 200,

					"text" : "",
				},
				## ObserverCount
				{
					"name" : "ObserverCount",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 185,

					"text" : "",
				},
				{
					"name" : "MailBoxButton",
					"type" : "image",

					"x" : 126-15,
					"y" : 30,

					"image" : "d:/ymir work/ui/game/mailbox/post_minimap.sub",
					#"over_image" : "d:/ymir work/ui/game/mailbox/post_minimap.sub",
					#"down_image" : "d:/ymir work/ui/game/mailbox/post_minimap.sub",
				},
				{
					"name" : "MailBoxEffect",
					"type" : "ani_image",
								
					"x" : 126-15,
					"y" : 30,
								
					"delay" : 6,

					"images" :
					(
						"d:/ymir work/ui/game/mailbox/minimap_flash/2.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/3.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/4.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/5.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/4.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/3.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/2.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
						"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
					),
				},
				
				{
					"name" : "Hour",
					"type" : "text",
					"text_horizontal_align" : "center",
					"outline" : 1,
					"x" : 70,
					"y" : 147+8,
					"text" : "",
				},
				{
					"name" : "History",
					"type" : "text",
					"text_horizontal_align" : "center",
					"outline" : 1,
					"x" : 70,
					"y" : 162+8,
					"text" : "",
				},
			],
		},
		{
			"name" : "MastWindow",
			"type" : "thinboard",

			"x" : 35,
			"y" : 160,

			"width" : 105,
			"height" : 37,
			"children" :
			(
				{
					"name" : "MastText",
					"type" : "text",

					"text_horizontal_align" : "center",

					"x" : 35,
					"y" : 8,

					"text" : uiScriptLocale.DEFANCE_WAWE_MAST_TEXT,
				},
				{
					"name" : "MastHp",
					"type" : "gauge",

					"x" : 10,
					"y" : 23,

					"width" : 85,
					"color" : "red",
					
					"tooltip_text" : uiScriptLocale.DEFANCE_WAWE_GAUGE_TOOLTIP,
				},
			),
		},
		{
			"name" : "CloseWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 132,
			"height" : 48,

			"children" :
			[
				## ShowButton
				{
					"name" : "MiniMapShowButton",
					"type" : "button",

					"x" : 100,
					"y" : 4,

					"default_image" : ROOT + "minimap_open_default.sub",
					"over_image" : ROOT + "minimap_open_default.sub",
					"down_image" : ROOT + "minimap_open_default.sub",
				},
			],
		},
	],
}
