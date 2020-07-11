import uiScriptLocale

window = {
	"name" : "KeyChange_Window",
	
	"x" : SCREEN_WIDTH - 136 - 100,
	"y" : 15,
	
	"style" : ("movable", "float",),
	
	"width" : 800,
	"height" : 500,
	
	"children" :
	(
		##Board
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),
	
			"x" : 0,
			"y" : 0,
			
			"width" : 800,
			"height" : 500,
			
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : 790,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":400, "y":3, "text":uiScriptLocale.KEYCHANGE_KEY_SETTING, "text_horizontal_align":"center" },
					),
				},
				## 기본동작 bar
				{
					"name" : "HorizontalBar1",
					"type" : "horizontalbar",
					"x" : 20,
					"y" : 45,
					"width" : 380,				
				
					"children":
					(
						## 기본동작 Text
						{
							"name" : "BaseInfo", "type" : "text", "x" : 8, "y" : 2, "text" : uiScriptLocale.KEYCHANGE_BASEACTION,
						},				
					),
				},
				## 슬롯 단축키 bar
				{
					"name" : "HorizontalBar1",
					"type" : "horizontalbar",
					"x" : 28+360+30,
					"y" : 45,
					"width" : 165,
				
					"children":
					(
						## 슬롯 단축키 Text
						{
							"name" : "BaseInfo", "type" : "text", "x" : 8, "y" : 2, "text" : uiScriptLocale.KEYCHANGE_SLOT_KET,
						},				
					),
				},
				## 인터페이스 bar
				{
					"name" : "HorizontalBar1",
					"type" : "horizontalbar",
					"x" : 28+540+30,
					"y" : 45,
					"width" : 165,
				
					"children":
					(
						## 인터페이스 Text
						{
							"name" : "BaseInfo", "type" : "text", "x" : 8, "y" : 2, "text" : uiScriptLocale.KEYCHANGE_INTERFACE,
						},				
					),
				},				
				## Button
				{
					"name" : "ClearButton",
					"type" : "button",

					"x" : 345-40,
					"y" : 10 + 380 + 70 + 7,

					"text" : uiScriptLocale.KEYCHANGE_CLEAR,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				
				},
				{
					"name" : "SaveButton",
					"type" : "button",

					"x" : 415-40,
					"y" : 10 + 380 + 70 + 7,

					"text" : uiScriptLocale.KEYCHANGE_SAVE,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				
				},				
				{
					"name" : "CancelButton",
					"type" : "button",

					"x" : 485-40,
					"y" : 10 + 380 + 70 + 7,

					"text" : uiScriptLocale.KEYCHANGE_CANCLE,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				
				},				

				## 기본동작
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_UP1,    "text_horizontal_align" : "left", "x" : 28, "y" : 75,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_DOWN1,      "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 20,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_LEFT1,      "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 40,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_RIGHT1,    "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 60,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_RIGHT1,   "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 80,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_LEFT1,     "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 100,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_ZOOM_IN1,   "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 120,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_ZOOM_OUT1,   "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 140,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_DOWN1,   "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 160,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_UP1,    "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 180,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_ROOT1,     "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 200,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_ATTACK,  "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 220,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_RIDE_HORSE,    "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 240,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_RIDE_PEED,   "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 260,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION1,  "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 280,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION2, "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 300,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION3, "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 320,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION4, "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 340,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SUPPORT_GUI,   "text_horizontal_align" : "left", "x" : 28, "y" : 75 + 360,	},
				
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_UP2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_DOWN2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 20,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_LEFT2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 40,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_RIGHT2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 60,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_RIGHT2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 80,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_LEFT2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 100,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_ZOOM_IN2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 120,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_ZOOM_OUT2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 140,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_DOWN2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 160,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CAM_UP2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 180,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_ROOT2, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 200,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_RIDE, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 220,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_RIDE_BYE, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 240,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION5, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 260,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION6, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 280,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION7, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 300,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION8, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 320,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_EMOTION9, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 340,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SPECIAL_INVENTORY, "text_horizontal_align" : "left", "x" : 28+180+10, "y" : 75 + 360,	},
				##슬롯 단축키
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SLOT1,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SLOT2,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 20,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SLOT3,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 40,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SLOT4,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 60,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SLOT5,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 80,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SLOT6,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 100,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SLOT7,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 120,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SLOT8,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 140,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_QUICKSLOT1,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 160,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_QUICKSLOT2,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 180,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_QUICKSLOT3,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 200,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_QUICKSLOT4,         "text_horizontal_align" : "left", "x" : 28+360+35, "y" : 75 + 220,	},
				##인터페이스
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_STATUS_WINDOW,  "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SKILL_WINDOW,  "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 20,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_QUEST_WINDOW,  "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 40,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_INVENTORY_WINDOW,  "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 60,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_DDS_WINDOW,  "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 80,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_ATLASMAP_WINDOW,  "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 100,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_CHAT_WINDOW, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 120,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_QUEST_SCROLL_ONOFF, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 140,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_GUILD_WINDOW, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 160,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_MESSENGER_WINDOW, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 180,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_HELP_WINDOW, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 200,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_ACTION_WINDOW, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 220,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_MINIMAP_PLUS, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 240,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_MINIMAP_MINER, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 260,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SCREENSHOT, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 280,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SHOW_NAME, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 300,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SHOW_MINIMAP, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 320,	},
				{ "name" : "Main", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_SHOW_CHAT, "text_horizontal_align" : "left", "x" : 28+540+35, "y" : 75 + 340,	},
			),
		},
	),
}