import uiScriptLocale

BUTTON_ROOT = "d:/ymir work/ui/public/"
BUTTON_ROOTT = "d:/ymir work/ui/game/"

window = {
	"name" : "ChestDropWindow",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : 193,
	"height" : 354,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 193,
			"height" : 354,
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : 193 -11,					
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x": 120-34, "y":3, "text": uiScriptLocale.CHEST_SEARCH_TITLE, "text_horizontal_align":"center" },
					),
				},
				
				
				
				{
					"name" : "OpenCountController",
					"type" : "sliderbar",

					"x" : 10,
					"y" : 213+95,
				},	
									
				{
					"name" : "OpenChestButtonSingle",
					"type" : "button",

					"x" : 35,
					"y" : 175+150,
					
					"default_image" : BUTTON_ROOTT + "buttons/button_bank_1.tga",
					"over_image" : BUTTON_ROOTT + "buttons/button_bank_2.tga",
					"down_image" : BUTTON_ROOTT + "buttons/button_bank_3.tga",
				},
				
				{
					"name" : "OpenChestButtonMultiple",
					"type" : "button",

					"x" : 37,
					"y" : 213+112,
					
					"default_image" : BUTTON_ROOTT + "buttons/button_bank_1.tga",
					"over_image" : BUTTON_ROOTT + "buttons/button_bank_2.tga",
					"down_image" : BUTTON_ROOTT + "buttons/button_bank_3.tga",
				},
				
				{
					"name" : "prev_button", 
					"type" : "button",
					
					"x" : 10, 
					"y" : 200+95, 

					"default_image" : "d:/ymir work/ui/public/public_page_button/page_first_prev_btn_01.sub",
					"over_image" 	: "d:/ymir work/ui/public/public_page_button/page_first_prev_btn_02.sub",
					"down_image" 	: "d:/ymir work/ui/public/public_page_button/page_first_prev_btn_01.sub",
				},
				
				{
					"name" : "CurrentPageBack",
					"type" : "thinboard_circle",
					
					"x" : 28, 
					"y" : 175+95,
					
					"width" : 30,
					"height" : 20,
					
					"children" :
					(
						{
							"name" : "CurrentPage",
							"type" : "text",
							
							"x" : 0,
							"y" : 0,
							
							"vertical_align" : "center",
							"horizontal_align" : "center",
							
							"text_vertical_align" : "center",
							"text_horizontal_align" : "center",

							"text" : "1",
						},
					),
				},
				
				{
					"name" : "next_button", 
					"type" : "button",

					"x" : 62, 
					"y" : 200+95, 

					"default_image" : "d:/ymir work/ui/public/public_page_button/page_last_next_btn_01.sub",
					"over_image" 	: "d:/ymir work/ui/public/public_page_button/page_last_next_btn_02.sub",
					"down_image" 	: "d:/ymir work/ui/public/public_page_button/page_last_next_btn_01.sub",
				},
			),
		},
	),
}