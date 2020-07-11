import item
import app
import uiScriptLocale
import localeInfo

window = {
	"name" : "Aura_LevelUpWindow",

	"x" : SCREEN_WIDTH - 176 - 287 - 10,
	"y" : SCREEN_HEIGHT - 37 - 525,

	"style" : ("movable", "float",),

	"width" : 248,
	"height" : 294,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 248,
			"height" : 294,
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : 235,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":117, "y":3, "text_horizontal_align":"center" },
					),
				},
				
				## Slot
				{
					"name" : "Aura_Levelup",
					"type" : "image",
					
					"x" : 13,
					"y" : 35,
					
					"image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "aura/aura_levelup_bg.sub",
					
					"children" :
					(
						## Main
						{"name" : "Main", "type" : "text", "x" : 66, "y" : 10, "text":uiScriptLocale.AURA_MAIN, "text_horizontal_align" : "center"},
						## Sub
						{"name" : "Main", "type" : "text", "x" : 158, "y" : 10, "text":uiScriptLocale.AURA_SUB, "text_horizontal_align" : "center"},
						## Slot
						{
							"name" : "AuraSlot",
							"type" : "slot",
					
							"x" : 3,
							"y" : 3,
					
							"width" : 190,
							"height" : 200,
					
							"slot" : (
								{"index":0, "x":46, "y":28, "width":31, "height":31},  # 메인
								{"index":1, "x":138, "y":28, "width":31, "height":31},  # 서브
							),
						},
					),
				},

				## Line
				{
					"name" : "Aura_ToopTip_Line",
					"type" : "image",
					
					"x" : 27,
					"y" : 176,
					
					"image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "aura/aura_levelup_line.sub",
				},

				## ToolTips				
				{"name" : "AuraToolTip1", "type" : "text", "x" : 34, "y" : 122, "text_horizontal_align" : "left"},
				{"name" : "AuraToolTip2", "type" : "text", "x" : 34, "y" : 140, "text_horizontal_align" : "left"},
				{"name" : "AuraToolTip3", "type" : "text", "x" : 34, "y" : 158, "text_horizontal_align" : "left"},
				{"name" : "AuraToolTip4", "type" : "text", "x" : 34, "y" : 190, "text_horizontal_align" : "left"},
				{"name" : "AuraToolTip5", "type" : "text", "x" : 34, "y" : 208, "text_horizontal_align" : "left"},
				{"name" : "AuraToolTip6", "type" : "text", "x" : 34, "y" : 226, "text_horizontal_align" : "left"},

				## Button
				{
					"name" : "AcceptButton",
					"type" : "button",

					"x" : 36,
					"y" : 260,

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
				{
					"name" : "CancelButton",
					"type" : "button",

					"x" : 124,
					"y" : 260,

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},				
			),
		},
	),
}

