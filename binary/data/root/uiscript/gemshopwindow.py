import uiScriptLocale
BUTTON_ROOT = "d:/ymir work/ui/public/"
ROOT_PATH = "d:/ymir work/ui/game/windows/"
GEM_PATH = "d:/ymir work/ui/gemshop/"
GEM_ICON = "d:/ymir work/ui/gemshop/gemshop_gemicon.sub"
BOARD_WIDTH = 168
BOARD_HEIGHT = 244
window = {
	"name" : "GemShopWindow",

	"x" : (SCREEN_WIDTH/2) - (220/2),
	"y" : (SCREEN_HEIGHT/2) - 250,


	"style" : ("movable", "float",),

	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : BOARD_WIDTH-15,					
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":BOARD_WIDTH / 2 - 5, "y":3, "text": uiScriptLocale.GEMSHOP_TITLE, "text_horizontal_align":"center" },
					),
				},

				## πÈ ¿ÃπÃ¡ˆ
				{ 
					"name":"gemshopbackimg", 
					"type":"image", 
					"x":14,
					"y":31, 
					"image": GEM_PATH+"gemshop_backimg.sub",
				},
				

				## ∆» æ∆¿Ã≈€ ΩΩ∑‘
				{
					"name" : "SellItemSlot",
					"type" : "slot",
			
					"x" : 0,
					"y" : 60,
			
					"width" : 190,
					"height" : 200,
					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
			
					"slot" : (
						{"index":0, "x":22, "y":0, "width":32, "height":32},  # slot_1
						{"index":1, "x":67, "y":0, "width":32, "height":32},  # slot_2
						{"index":2, "x":112, "y":0, "width":32, "height":32},  # slot_3

						{"index":3, "x":22, "y":58, "width":32, "height":32},  # slot_4
						{"index":4, "x":67, "y":58, "width":32, "height":32},  # slot_5
						{"index":5, "x":112, "y":58, "width":32, "height":32},  # slot_6

						{"index":6, "x":22, "y":116, "width":32, "height":32},  # slot_7
						{"index":7, "x":67, "y":116, "width":32, "height":32},  # slot_8
						{"index":8, "x":112, "y":116, "width":32, "height":32},  # slot_9
					),
				},

				{ "name":"slot_1_price", "type":"text", "x" : 19+28, "y":100, "text":"999", "text_horizontal_align":"center" },
				{ "name":"slot_1_Icon", "type":"image", "x":19, "y":100, "image":GEM_ICON, },	
				
				{ "name":"slot_2_price", "type":"text", "x" : 64+28, "y":100, "text":"2", "text_horizontal_align":"center" },
				{ "name":"slot_2_Icon", "type":"image", "x":64, "y":100, "image":GEM_ICON, },	
				
				{ "name":"slot_3_price", "type":"text", "x" : 109+28, "y":100, "text":"33", "text_horizontal_align":"center" },
				{ "name":"slot_3_Icon", "type":"image", "x":109, "y":100, "image":GEM_ICON, },	
								
				{ "name":"slot_4_price", "type":"text", "x" : 19+28, "y":158, "text":"4", "text_horizontal_align":"center" },
				{ "name":"slot_4_Icon", "type":"image", "x":19, "y":158, "image":GEM_ICON, },	
				
				{ "name":"slot_5_price", "type":"text", "x" : 64+28, "y":158, "text":"5", "text_horizontal_align":"center" },
				{ "name":"slot_5_Icon", "type":"image", "x":64, "y":158, "image":GEM_ICON, },	
				
				{ "name":"slot_6_price", "type":"text", "x" : 109+28, "y":158, "text":"6", "text_horizontal_align":"center" },
				{ "name":"slot_6_Icon", "type":"image", "x":109, "y":158, "image":GEM_ICON, },	
				
				{ "name":"slot_7_price", "type":"text", "x" : 19+28, "y":216, "text":"7", "text_horizontal_align":"center" },
				{ "name":"slot_7_Icon", "type":"image", "x":19, "y":216, "image":GEM_ICON, },	
				
				{ "name":"slot_8_price", "type":"text", "x" : 64+28, "y":216, "text":"8", "text_horizontal_align":"center" },
				{ "name":"slot_8_Icon", "type":"image", "x":64, "y":216, "image":GEM_ICON, },	
				
				{ "name":"slot_9_price", "type":"text", "x" : 109+28, "y":216, "text":"9", "text_horizontal_align":"center" },
				{ "name":"slot_9_Icon", "type":"image", "x":109, "y":216, "image":GEM_ICON, },	
				{
					"name" : "backPage1",
					"type" : "button",

					"x" : 13,
					"y" : 30,
					
					"text"	:	"",
					
					"default_image" : GEM_PATH+"gemshop_refreshbutton_up.sub",
					"over_image" : GEM_PATH+"gemshop_refreshbutton_over.sub",
					"down_image" : GEM_PATH+"gemshop_refreshbutton_down.sub",
				},
				{
					"name" : "pageText1",
					"type" : "text",

					"x" : 13 + 50 - 1,
					"y" : 30 + 4,
					
					"text" : "",
				},
				{
					"name" : "nextPage1",
					"type" : "button",

					"x" : 133,
					"y" : 30,
					
					"text"	:	"",
					
					"default_image" : GEM_PATH+"gem_next_button_normal.dds",
					"over_image" : GEM_PATH+"gem_next_button_hover.dds",
					"down_image" : GEM_PATH+"gem_next_button_pressed.dds",
				},
			),
		},
	),
}

