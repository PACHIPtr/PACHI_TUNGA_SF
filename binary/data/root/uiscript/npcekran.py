import uiScriptLocale
import localeInfo

BUTTON_ADRESS = "d:/ymir work/ui/game/myshop_deco/"
window = {
	"name" : "GameOptionDialog",
	"style" : ("movable", "float",),

	"x" : (SCREEN_WIDTH/2) - (190/2),
	"y" : (SCREEN_HEIGHT/2) - 100,	

	"width" : 190,
	"height" : 265,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 190,
			"height" : 265,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 7,

					"width" : 190-13,
					"color" : "gray",

					"children" :
					(
						{ "name":"titlename", "type":"text", "x":0, "y":3, 
						"text" : "Menu", 
						"horizontal_align":"center", "text_horizontal_align":"center" },
					),
				},

				{
					"name" : "BlackBoard",
					"type" : "thinboard_circle",
					"x" : 13, "y" : 36, "width" : 190-(13*2), "height" : 220,
					"children":
					(
						{
							"name" : "weapon_shop", "type" : "button",
							"x" : 7, "y" : 9, "text": "Silah Sat\xfdc\xfds\xfd",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "armor_shop", "type" : "button",
							"x" : 7, "y" : 9+30, "text": "Z\xfdrh Sat\xfdc\xfds\xfd",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "shop", "type" : "button",
							"x" : 7, "y" : 9+(30*2), "text": localeInfo.NORMAL_SHOP,

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "bio_shop", "type" : "button",
							"x" : 7, "y" : 9+(30*3), "text": "Sebnemci",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "fish_shop", "type" : "button",
							"x" : 7, "y" : 9+(30*4), "text": "Bal\xfdk\xe7\xfd",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "alchemy_shop", "type" : "button",
							"x" : 7, "y" : 9+(30*5), "text": "Simyac\xfd",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
						{
							"name" : "wedding_shop", "type" : "button",
							"x" : 7, "y" : 9+(30*6), "text": "Seyyar Sat\xfdc\xfd",

							"default_image" : BUTTON_ADRESS+ "select_btn_01.sub",
							"over_image" : BUTTON_ADRESS +"select_btn_02.sub",
							"down_image" : BUTTON_ADRESS +"select_btn_03.sub",
						},
					),
				},									
			),
		},
	),
}
