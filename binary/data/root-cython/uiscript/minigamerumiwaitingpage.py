import uiScriptLocale

ROOT = "d:/ymir work/ui/game/"

WINDOW_WIDTH	= 352
WINDOW_HEIGHT	= 384

BOARD_WIDTH		= 305
BOARD_HEIGHT	= 300

window = {
	"name" : "RumiWaitingPage",
	"style" : ("movable",),
	
	"x" : SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2,
	"y" : SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2,
	
	"width" : WINDOW_WIDTH,
	"height" : WINDOW_HEIGHT,
	
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),
			
			"x" : 0,
			"y" : 0,
			
			"width" : WINDOW_WIDTH,
			"height" : WINDOW_HEIGHT,
			
			"children" :
			(
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 0,
					"y" : 0,

					"width" : WINDOW_WIDTH,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":0, "text": uiScriptLocale.MINI_GAME_RUMI_TITLE, "all_align":"center" },
					),
				},
		
				{
					"name" : "desc_board",
					"type" : "bar",
					
					"x" : 15,
					"y" : 30,

					"width" : BOARD_WIDTH,
					"height" : BOARD_HEIGHT,
					
					"children" :
					(
						{
							"name" : "right_line",
							"type" : "line",

							"x" : BOARD_WIDTH-1,
							"y" : 0,

							"width" : 0,
							"height" : BOARD_HEIGHT,

							"color" : 0xffAAA6A1,
						},
						
						{
							"name" : "bottom_line",
							"type" : "line",

							"x" : 0,
							"y" : BOARD_HEIGHT-1,

							"width" : BOARD_WIDTH,
							"height" : 0,

							"color" : 0xffAAA6A1,
						},
						{
							"name" : "left_line",
							"type" : "line",

							"x" : 0,
							"y" : 0,

							"width" : 0,
							"height" : BOARD_HEIGHT,

							"color" : 0xff2A2521,
						},
						{
							"name" : "top_line",
							"type" : "line",

							"x" : 0,
							"y" : 0,

							"width" : BOARD_WIDTH,
							"height" : 0,

							"color" : 0xff2A2521,
						},
						
					),
				
				},
				
				{
					"name" : "scrollbar",
					"type" : "scrollbar",

					"x" : 29,
					"y" : 30,
					"size" : BOARD_HEIGHT+5,
					"horizontal_align" : "right",
				},
				
				{
					"name" : "game_start_button",
					"type" : "button",
					
					"x" : 40,
					"y" : 40,
					
					"text" : uiScriptLocale.MINI_GAME_RUMI_START_TEXT,
					
					"vertical_align" : "bottom",
					"horizontal_align" : "left",
					
					
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
				
				{
					"name" : "confirm_check_button_text_window",
					"type" : "window",
					"style" : ("attach",),
					
					"x" : 275,
					"y" : 348,
					
					"width" : 29,
					"height" : 18,
					
					"children" :
					(
						{
							"name" : "confirm_check_button_text",
							"type" : "text",
							
							"x" : 0,
							"y" : 0,
							
							"text_horizontal_align" : "right",
							
							"text" : uiScriptLocale.MINI_GAME_RUMI_DISCARD_TEXT,
						},
					),
				},
				
				{
					"name" : "confirm_check_button",
					"type" : "button",
					
					"x" : 285,
					"y" : 344,
					"default_image" : "d:/ymir work/ui/public/Parameter_Slot_07.sub",
					"over_image" : "d:/ymir work/ui/public/Parameter_Slot_07.sub",
					"down_image" : "d:/ymir work/ui/public/Parameter_Slot_07.sub",
				},
				{
					"name" : "check_image",
					"type" : "button",
					
					"x" : 290,
					"y" : 344,
					
					"default_image" : "d:/ymir work/ui/public/check_image.sub",
					"over_image" : "d:/ymir work/ui/public/check_image.sub",
					"down_image" : "d:/ymir work/ui/public/check_image.sub",
				},
				
			),
		},
	),	
}
