import uiScriptLocale

BUTTON_ROOT = "d:/ymir work/ui/public/"

window = {
	"name" : "ChangeLookWindow",
	"x" : SCREEN_WIDTH - 176 - 287 - 10,
	"y" : SCREEN_HEIGHT - 37 - 525,
	"style" : ("movable", "float",),
	"width" : 205,
	"height" : 270,
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),
			"x" : 0,
			"y" : 0,
			"width" : 205,
			"height" : 270,
			"children" :
			(
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),
					"x" : 6,
					"y" : 6,
					"width" : 190,
					"color" : "yellow",
					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",
							"x" : 95,
							"y" : 3,
							"text" : "",
							"text_horizontal_align" : "center"
						},
					),
				},
				{
					"name" : "ChangeLook_SlotImg",
					"type" : "image",
					"x" : 9,
					"y" : 35,
					"image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "changelook/ingame_convert.tga",
					"children" :
					(
						{
							"name" : "ChangeLookSlot",
							"type" : "slot",
					
							"x" : 3,
							"y" : 3,
					
							"width" : 190,
							"height" : 200,
					
							"slot" : (
										{
											"index" : 0,
											"x" : 26,
											"y" : 20,
											"width" : 31,
											"height" : 96
										},
										{
											"index" : 1,
											"x" : 125,
											"y" : 20,
											"width" : 31,
											"height" : 96
										},
										{
											"index":2,
											"x":75, 
											"y":126, 
											"width":31, 
											"height":31,
										},
							),
						},
					),
				},
				{
					"name" : "Cost",
					"type" : "text",
					"text" : "",
					"text_horizontal_align" : "center",
					"x" : 102,
					"y" : 215,
				},
				{
					"name" : "AcceptButton",
					"type" : "button",
					"x" : 40,
					"y" : 235,
					"default_image" : BUTTON_ROOT + "AcceptButton00.sub",
					"over_image" : BUTTON_ROOT + "AcceptButton01.sub",
					"down_image" : BUTTON_ROOT + "AcceptButton02.sub",
				},
				{
					"name" : "CancelButton",
					"type" : "button",
					"x" : 114,
					"y" : 235,
					"default_image" : BUTTON_ROOT + "CancleButton00.sub",
					"over_image" : BUTTON_ROOT + "CancleButton01.sub",
					"down_image" : BUTTON_ROOT + "CancleButton02.sub",
				},
				## ChangeLookToolTip Button
				{
					"name" : "ChangeLookToolTIpButton",
					"type" : "button",

					"x" : 155,
					"y" : 8,

					"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
					"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
					"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
				},						
			),
		},
	),
}