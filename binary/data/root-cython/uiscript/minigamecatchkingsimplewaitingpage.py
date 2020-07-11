import uiScriptLocale

ROOT = "d:/ymir work/ui/minigame/catchking/"
PUBLIC_PATH = "d:/ymir work/ui/public/"
PATTERN_PATH = "d:/ymir work/ui/pattern/"

WINDOW_WIDTH	= 392
WINDOW_HEIGHT	= 336

BOARD_WIDTH		= 362
BOARD_HEIGHT	= 167

BOTTOM_BOARD_HEIGHT = 80
BOTTOM_BARD_COUNT_X	= 20
BOTTOM_BARD_COUNT_Y	= 2

window = {
	"name" : "MiniGameCatchKingSimpleWaitingPage",
	"style" : ("movable",),
	
	"x" : SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2,
	"y" : SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2,
	
	"width" : WINDOW_WIDTH,
	"height" : WINDOW_HEIGHT,
	
	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",
			
			"x" : 0,
			"y" : 0,
			
			"width" : WINDOW_WIDTH,
			"height" : WINDOW_HEIGHT,
			
			"title" : uiScriptLocale.MINI_GAME_CATCHKING_TITLE,
			
			"children" :
			(		
				{
					"name" : "desc_board",
					"type" : "bar",
					
					"x" : 15,
					"y" : 40,

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
						
						{
							"name" : "prev_button",
							"type" : "button",

							"x" : BOARD_WIDTH - 55,
							"y" : BOARD_HEIGHT - 20,

							"default_image" : PUBLIC_PATH + "public_intro_btn/prev_btn_01.sub",
							"over_image" : PUBLIC_PATH + "public_intro_btn/prev_btn_02.sub",
							"down_image" : PUBLIC_PATH + "public_intro_btn/prev_btn_01.sub",
						},
						
						{
							"name" : "next_button",
							"type" : "button",

							"x" : BOARD_WIDTH - 30,
							"y" : BOARD_HEIGHT - 20,

							"default_image" : PUBLIC_PATH + "public_intro_btn/next_btn_01.sub",
							"over_image" : PUBLIC_PATH + "public_intro_btn/next_btn_02.sub",
							"down_image" : PUBLIC_PATH + "public_intro_btn/next_btn_01.sub",
						},
					),
				
				},
				
				## bottom menu
				{
					"name" : "menu_window",
					"type" : "window",
					
					"style" : ("attach", "ltr",),

					"x" : 15,
					"y" : 212,
					
					"width" : BOARD_WIDTH,
					"height" : BOTTOM_BOARD_HEIGHT,
					
					"children" :
					(
						## LeftTop 1
						{
							"name" : "LeftTop",
							"type" : "image",
							"style" : ("ltr",),
							
							"x" : 0,
							"y" : 0,
							"image" : PATTERN_PATH + "border_A_left_top.tga",
						},
						## RightTop 2
						{
							"name" : "RightTop",
							"type" : "image",
							"style" : ("ltr",),
							
							"x" : BOARD_WIDTH - 16,
							"y" : 0,
							"image" : PATTERN_PATH + "border_A_right_top.tga",
						},
						## LeftBottom 3
						{
							"name" : "LeftBottom",
							"type" : "image",
							"style" : ("ltr",),
							
							"x" : 0,
							"y" : BOTTOM_BOARD_HEIGHT - 16,
							"image" : PATTERN_PATH + "border_A_left_bottom.tga",
						},
						## RightBottom 4
						{
							"name" : "RightBottom",
							"type" : "image",
							"style" : ("ltr",),
							
							"x" : BOARD_WIDTH - 16,
							"y" : BOTTOM_BOARD_HEIGHT - 16,
							"image" : PATTERN_PATH + "border_A_right_bottom.tga",
						},
						## topcenterImg 5
						{
							"name" : "TopCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 16,
							"y" : 0,
							"image" : PATTERN_PATH + "border_A_top.tga",
							"rect" : (0.0, 0.0, BOTTOM_BARD_COUNT_X, 0),
						},
						## leftcenterImg 6
						{
							"name" : "LeftCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 0,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_left.tga",
							"rect" : (0.0, 0.0, 0, BOTTOM_BARD_COUNT_Y),
						},
						## rightcenterImg 7
						{
							"name" : "RightCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : BOARD_WIDTH - 16,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_right.tga",
							"rect" : (0.0, 0.0, 0, BOTTOM_BARD_COUNT_Y),
						},
						## bottomcenterImg 8
						{
							"name" : "BottomCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 16,
							"y" : BOTTOM_BOARD_HEIGHT - 16,
							"image" : PATTERN_PATH + "border_A_bottom.tga",
							"rect" : (0.0, 0.0, BOTTOM_BARD_COUNT_X, 0),
						},
						## centerImg
						{
							"name" : "CenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 16,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_center.tga",
							"rect" : (0.0, 0.0, BOTTOM_BARD_COUNT_X, BOTTOM_BARD_COUNT_Y),
						},
						
						## challenge text
						{ 
							"name" : "challenge_text_window", "type" : "window", "x" : 68, "y" : 1, "width" : 222, "height" : 21,
							"children" :
							(
								{"name":"challenge_text_bg", "type":"image", "x":0, "y":0, "image" : ROOT + "challenge_text_bg.sub", "style" : ("not_pick",),},
								{"name":"challenge_text", "type":"text", "x":0, "y":0, "text":uiScriptLocale.MINI_GAME_CATCHKING_CHALLENGE, "all_align" : "center", "style" : ("not_pick",),},
							),
						},
						
						## challenge item slot bg
						{
							"name" : "ChallengeItemSlotBG",
							"type" : "image",
							"x" : 124,
							"y" : 27,
							"image" : ROOT + "challenge_slot_bg.sub",
						},
						## challenge item slot
						{
							"name" : "ChallengeItemSlot",
							"type" : "slot",

							"x" : 131,
							"y" : 34,
							"width" : 32,
							"height" : 32,
							
							"slot" : ({"index":0, "x":0, "y":0, "width":32, "height":32,},),
						},
						
						## challenge count text bg
						{ 
							"name" : "challenge_count_text_window", "type" : "window", "x" : 182, "y" : 40, "width" : 32, "height" : 18, "style" : ("not_pick",),
							"children" :
							(
								{"name":"challenge_count_text_bg", "type":"image", "style" : ("not_pick",), "x":0, "y":0, "image" : ROOT + "high_score_bg.sub"},
								{"name":"challenge_count_text", "type":"text", "style" : ("not_pick",), "x":0, "y":0, "text":"1", "all_align" : "center"},
							),	
						},
						## up arrow
						{
							"name" : "up_arrow_button",
							"type" : "button",
							
							"x" : 215,
							"y" : 34,
							
							"default_image" : "d:/ymir work/ui/public/arrow/large_up_arrow_default.sub",
							"over_image" : "d:/ymir work/ui/public/arrow/large_up_arrow_over.sub",
							"down_image" : "d:/ymir work/ui/public/arrow/large_up_arrow_down.sub",
						},
						## down arrow
						{
							"name" : "down_arrow_button",
							"type" : "button",
							
							"x" : 215,
							"y" : 51,
							
							"default_image" : "d:/ymir work/ui/public/arrow/large_down_arrow_default.sub",
							"over_image" : "d:/ymir work/ui/public/arrow/large_down_arrow_over.sub",
							"down_image" : "d:/ymir work/ui/public/arrow/large_down_arrow_down.sub",
						},
					),
				},
				
				{
					"name" : "game_start_button",
					"type" : "button",
					
					"x" : 44,
					"y" : 35,
					
					"text" : uiScriptLocale.MINI_GAME_CATCHKING_START_TEXT,
					
					"vertical_align" : "bottom",
					"horizontal_align" : "center",
					
					
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
				{
					"name" : "help_button",
					"type" : "button",
					
					"x" : -44,
					"y" : 35,
					
					"text" : uiScriptLocale.MINI_GAME_CATCHKING_HELP_TEXT,
					
					"vertical_align" : "bottom",
					"horizontal_align" : "center",
					
					
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
			),
		},
	),	
}
