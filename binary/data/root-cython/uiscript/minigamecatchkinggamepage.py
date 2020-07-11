import uiScriptLocale

ROOT = "d:/ymir work/ui/minigame/catchking/"
PATTERN_PATH = "d:/ymir work/ui/pattern/"

WINDOW_WIDTH	= 392
WINDOW_HEIGHT	= 336

MENU_BOARD_WINDOW_WIDTH		= 99
MENU_BOARD_WINDOW_HEIGHT	= 194
MENU_BOARD_PATTERN_X_COUNT	= 4
MENU_BOARD_PATTERN_Y_COUNT	= 10

HAND_BOARD_WINDOW_WIDTH		= 370
HAND_BOARD_WINDOW_HEIGHT	= 64
HAND_BOARD_PATTERN_X_COUNT	= 21
HAND_BOARD_PATTERN_Y_COUNT	= 1

window = {
	"name" : "MiniGameCatchKingGamePage",
	"style" : ("movable", "float",),
	
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
				## field bg
				{
					"name" : "field_bg",
					"type" : "image",
					"x" : 8,
					"y" : 31,
					"image" : ROOT + "bg.sub",
				},
				
				## right - info menu window
				{
					"name" : "menu_window",
					"type" : "window",
					
					"style" : ("attach", "ltr",),

					"x" : 282,
					"y" : 34,
					
					"width" : MENU_BOARD_WINDOW_WIDTH,
					"height" : MENU_BOARD_WINDOW_HEIGHT,
					
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
							
							"x" : MENU_BOARD_WINDOW_WIDTH - 16,
							"y" : 0,
							"image" : PATTERN_PATH + "border_A_right_top.tga",
						},
						## LeftBottom 3
						{
							"name" : "LeftBottom",
							"type" : "image",
							"style" : ("ltr",),
							
							"x" : 0,
							"y" : MENU_BOARD_WINDOW_HEIGHT - 16,
							"image" : PATTERN_PATH + "border_A_left_bottom.tga",
						},
						## RightBottom 4
						{
							"name" : "RightBottom",
							"type" : "image",
							"style" : ("ltr",),
							
							"x" : MENU_BOARD_WINDOW_WIDTH - 16,
							"y" : MENU_BOARD_WINDOW_HEIGHT - 16,
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
							"rect" : (0.0, 0.0, MENU_BOARD_PATTERN_X_COUNT, 0),
						},
						## leftcenterImg 6
						{
							"name" : "LeftCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 0,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_left.tga",
							"rect" : (0.0, 0.0, 0, MENU_BOARD_PATTERN_Y_COUNT),
						},
						## rightcenterImg 7
						{
							"name" : "RightCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : MENU_BOARD_WINDOW_WIDTH - 16,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_right.tga",
							"rect" : (0.0, 0.0, 0, MENU_BOARD_PATTERN_Y_COUNT),
						},
						## bottomcenterImg 8
						{
							"name" : "BottomCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 16,
							"y" : MENU_BOARD_WINDOW_HEIGHT - 16,
							"image" : PATTERN_PATH + "border_A_bottom.tga",
							"rect" : (0.0, 0.0, MENU_BOARD_PATTERN_X_COUNT, 0),
						},
						## centerImg
						{
							"name" : "CenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 16,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_center.tga",
							"rect" : (0.0, 0.0, MENU_BOARD_PATTERN_X_COUNT, MENU_BOARD_PATTERN_Y_COUNT),
						},
						
						## my number text window
						{ 
							"name" : "my_number_text_window", "type" : "window", "x" : 284 - 281, "y" : 37 - 34, "width" : 93, "height" : 19, "style" : ("attach",),
							"children" :
							(
								{"name":"my_number_text_bg", "type":"image", "x":0, "y":0, "image" : ROOT + "menu_text_bg.sub"},
								{"name":"my_number_text", "type":"text", "x":0, "y":0, "text":uiScriptLocale.MINI_GAME_CATCHKING_MYNUMBER, "all_align" : "center"},
							),	
						},
						## my number - card bg
						{
							"name" : "my_number_card_bg",
							"type" : "image",
							"x" : 299 - 281,
							"y" : 57 - 34,
							"image" : ROOT + "card_bg_gold.sub",
						},
						## selection number text window
						{ 
							"name" : "selection_number_text_window", "type" : "window", "x" : 284 - 281, "y" : 112 - 34, "width" : 93, "height" : 19, "style" : ("attach",),
							"children" :
							(
								{"name":"selection_number_text_bg", "type":"image", "x":0, "y":0, "image" : ROOT + "menu_text_bg.sub"},
								{"name":"selection_number_text", "type":"text", "x":0, "y":0, "text":uiScriptLocale.MINI_GAME_CATCHKING_SELECTION_NUMBER, "all_align" : "center"},
							),	
						},
						## selection number - card bg
						{
							"name" : "selection_number_card_bg",
							"type" : "image",
							"x" : 299 - 281,
							"y" : 132 - 34,
							"image" : ROOT + "card_bg_gold.sub",
						},
						## score window
						{
							"name" : "score_window", "type" : "window", "x" : 3, "y" : 154, "width" : 93, "height" : 37, "style" : ("attach",),
							
							"children" :
							(
								## score title text window
								{ 
									"name" : "score_title_text_window", "type" : "window", "x" : 0, "y" : 0, "width" : 93, "height" : 19, "style" : ("not_pick",),
									"children" :
									(
										{"name":"score_title_text_bg", "type":"image", "style" : ("not_pick",), "x":0, "y":0, "image" : ROOT + "menu_text_bg.sub"},
										{"name":"score_title_text", "type":"text", "style" : ("not_pick",), "x":0, "y":0, "text":uiScriptLocale.MINI_GAME_CATCHKING_SCORE, "all_align" : "center"},
									),	
								},
								## score text window
								{ 
									"name" : "score_text_window", "type" : "window", "x" : 0, "y" : 19, "width" : 93, "height" : 18, "style" : ("not_pick",),
									"children" :
									(
										{"name":"score_text_bg", "type":"image", "style" : ("not_pick",), "x":0, "y":0, "image" : ROOT + "score_bg.sub"},
										{"name":"score_text", "type":"text", "style" : ("not_pick",), "x":0, "y":0, "text":"0", "all_align" : "center"},
									),	
								},
							),
						},
					),
				},
				
				## goal effect
				{
					"name" : "success_effect1",
					"type" : "ani_image",
					
					"x" : 8 + 135 - 128/2 - 64,
					"y" : 31 + 58 - 20,
					
					"delay" : 6,

					"images" :
					(
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff1.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff2.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff3.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff4.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff7.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff8.sub",
					),
				},
				
				{
					"name" : "success_effect2",
					"type" : "ani_image",
					
					"x" : 8 + 135 - 128/2,
					"y" : 31 + 58 - 20,
					
					"delay" : 6,

					"images" :
					(
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff1.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff2.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff3.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff4.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff7.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff8.sub",
					),
				},
				
				{
					"name" : "success_effect3",
					"type" : "ani_image",
					
					"x" : 8 + 135,
					"y" : 31 + 58 - 20,
					
					"delay" : 6,

					"images" :
					(
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff1.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff2.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff3.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff4.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff7.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff8.sub",
					),
				},
				
				{
					"name" : "success_text_effect",
					"type" : "ani_image",
					
					"x" : 8 + 50,
					"y" : 31 + 58,
					
					"delay" : 6,

					"images" :
					(
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect1.sub",
						#"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						#"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						#"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect7.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect8.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect9.sub",
					),
				},
				
				## reward button
				{
					"name" : "reward_button",
					"type" : "button",
					
					"x" : 88+20,
					"y" : 32,
					
					"text" : uiScriptLocale.MINI_GAME_CATCHKING_REWARD,							
					
					"vertical_align" : "bottom",
					"horizontal_align" : "right",
					
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
				
				## high score title text window
				{ 
					"name" : "high_score_title_text_window", "type" : "window", "x" : 11, "y" : 32, "width" : 93, "height" : 19, "vertical_align" : "bottom", "style" : ("not_pick",),
					"children" :
					(
						{"name":"high_score_title_text_bg", "type":"image", "style" : ("not_pick",), "x":0, "y":0, "image" : ROOT + "menu_text_bg.sub"},
						{"name":"high_score_title_text", "type":"text", "style" : ("not_pick",), "x":0, "y":0, "text":uiScriptLocale.MINI_GAME_CATCHKING_HIGH_SCORE, "all_align" : "center"},
					),	
				},
				## high score text window
				{ 
					"name" : "high_score_text_window", "type" : "window", "x" : 11+93+1, "y" : 32, "width" : 32, "height" : 18, "vertical_align" : "bottom", "style" : ("not_pick",),
					"children" :
					(
						{"name":"high_score_text_bg", "type":"image", "style" : ("not_pick",), "x":0, "y":0, "image" : ROOT + "high_score_bg.sub"},
						{"name":"high_score_text", "type":"text", "style" : ("not_pick",), "x":0, "y":0, "text":"0", "all_align" : "center"},
					),	
				},

				## bottom - hand card window
				{
					"name" : "hand_card_window",
					"type" : "window",
					
					"style" : ("attach", "ltr",),

					"x" : 11,
					"y" : 234,
					
					"width" : HAND_BOARD_WINDOW_WIDTH,
					"height" : HAND_BOARD_WINDOW_HEIGHT,
					
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
							
							"x" : HAND_BOARD_WINDOW_WIDTH - 16,
							"y" : 0,
							"image" : PATTERN_PATH + "border_A_right_top.tga",
						},
						## LeftBottom 3
						{
							"name" : "LeftBottom",
							"type" : "image",
							"style" : ("ltr",),
							
							"x" : 0,
							"y" : HAND_BOARD_WINDOW_HEIGHT - 16,
							"image" : PATTERN_PATH + "border_A_left_bottom.tga",
						},
						## RightBottom 4
						{
							"name" : "RightBottom",
							"type" : "image",
							"style" : ("ltr",),
							
							"x" : HAND_BOARD_WINDOW_WIDTH - 16,
							"y" : HAND_BOARD_WINDOW_HEIGHT - 16,
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
							"rect" : (0.0, 0.0, HAND_BOARD_PATTERN_X_COUNT, 0),
						},
						## leftcenterImg 6
						{
							"name" : "LeftCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 0,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_left.tga",
							"rect" : (0.0, 0.0, 0, HAND_BOARD_PATTERN_Y_COUNT),
						},
						## rightcenterImg 7
						{
							"name" : "RightCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : HAND_BOARD_WINDOW_WIDTH - 16,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_right.tga",
							"rect" : (0.0, 0.0, 0, HAND_BOARD_PATTERN_Y_COUNT),
						},
						## bottomcenterImg 8
						{
							"name" : "BottomCenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 16,
							"y" : HAND_BOARD_WINDOW_HEIGHT - 16,
							"image" : PATTERN_PATH + "border_A_bottom.tga",
							"rect" : (0.0, 0.0, HAND_BOARD_PATTERN_X_COUNT, 0),
						},
						## centerImg
						{
							"name" : "CenterImg",
							"type" : "expanded_image",
							"style" : ("ltr",),
							
							"x" : 16,
							"y" : 16,
							"image" : PATTERN_PATH + "border_A_center.tga",
							"rect" : (0.0, 0.0, HAND_BOARD_PATTERN_X_COUNT, HAND_BOARD_PATTERN_Y_COUNT),
						},
						
						## hand card bg1
						{
							"name" : "hand_card_bg1",
							"type" : "image",
							"x" : 20-11+58*0+1*0,
							"y" : 246-234,
							"image" : ROOT + "card_bg_silver.sub",
						},
						## hand card bg2
						{
							"name" : "hand_card_bg2",
							"type" : "image",
							"x" : 20-11+58*1+1*1,
							"y" : 246-234,
							"image" : ROOT + "card_bg_silver.sub",
						},
						## hand card bg3
						{
							"name" : "hand_card_bg3",
							"type" : "image",
							"x" : 20-11+58*2+1*2,
							"y" : 246-234,
							"image" : ROOT + "card_bg_silver.sub",
						},
						## hand card bg4
						{
							"name" : "hand_card_bg4",
							"type" : "image",
							"x" : 20-11+58*3+1*3,
							"y" : 246-234,
							"image" : ROOT + "card_bg_silver.sub",
						},
						## hand card bg5
						{
							"name" : "hand_card_bg5",
							"type" : "image",
							"x" : 20-11+58*4+1*4,
							"y" : 246-234,
							"image" : ROOT + "card_bg_silver.sub",
						},
						## hand card bg6
						{
							"name" : "hand_card_bg6",
							"type" : "image",
							"x" : 20-11+58*5+1*5,
							"y" : 246-234,
							"image" : ROOT + "card_bg_silver.sub",
						},
					),
				},
				## confirm check
				{ 
					"name" : "confirm_check_text_window", "type" : "window", "x" : 11+93+1+32+8, "y" : 32, "width" : 93, "height" : 19, "vertical_align" : "bottom", "style" : ("not_pick",),
					"children" :
					(
						{"name":"confirm_check_text_bg", "type":"image", "style" : ("not_pick",), "x":0, "y":0, "image" : ROOT + "menu_text_bg.sub"},
						{"name":"confirm_check_text", "type":"text", "style" : ("not_pick",), "x":0, "y":0, "text":uiScriptLocale.MINI_GAME_CATCHKING_POPUP_CONFIRM, "all_align" : "center"},
					),	
				},
				{
					"name" : "confirm_check_button",
					"type" : "expanded_image",
					
					"x" : 11+93+1+32+5+93+5,
					"y" : 32,
					"vertical_align" : "bottom",
					"image" : "d:/ymir work/ui/public/Parameter_Slot_07.sub",
				},
				{
					"name" : "check_image",
					"type" : "expanded_image",
					"style" : ("not_pick",),
					
					"x" : 11+93+1+32+5+93+5+5,
					"y" : 32,
					"vertical_align" : "bottom",
					"image" : "d:/ymir work/ui/public/check_image.sub",
				},
			),
		},
		{
			"name" : "field_mask_area",
			"type" : "window",
			
			"style" : ("not_pick",),
			"x" : 16,
			"y" : 39,
			
			"width" : 50*5 + 1*4,
			"height" : 36*5 + 1*4,
		},
	),	
}
