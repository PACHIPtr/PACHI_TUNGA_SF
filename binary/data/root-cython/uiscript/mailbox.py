import uiScriptLocale

ROOT = "d:/ymir work/ui/game/mailbox/"
PATTERN_PATH = "d:/ymir work/ui/pattern/"
PUBLIC_PATH = "d:/ymir work/ui/public/"

WINDOW_WIDTH	= 316
WINDOW_HEIGHT	= 434	#455

POST_WRITE_READ_WINDOW_WIDTH	= 274
POST_WRITE_READ_WINDOW_HEIGHT	= 316

BOARD_WINDOW_WIDTH	= 295
BOARD_WINDOW_HEIGHT = 361	#382

PATTERN_X_COUNT = 16
PATTERN_Y_COUNT = 20

window = {
	"name" : "MailBox",
	"style" : ("movable", "float",),
	
	#"x" : SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2,
	#"y" : SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2,
	
	"x" : SCREEN_WIDTH - 176 - WINDOW_WIDTH - POST_WRITE_READ_WINDOW_WIDTH + 5,
	"y" : SCREEN_HEIGHT - 37 - 565,
	
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
			
			"title" : uiScriptLocale.MAILBOX_TITLE,
		},
		
		## main bg
		{
			"name" : "board_window",
			"type" : "window",
			
			"style" : ("attach",),

			"x" : 10,
			"y" : 32,
			
			"width" : BOARD_WINDOW_WIDTH,
			"height" : BOARD_WINDOW_HEIGHT,
			
			"children" :
			(
				## LeftTop 1
				{
					"name" : "LeftTop",
					"type" : "image",
					
					"x" : 0,
					"y" : 0,
					"image" : PATTERN_PATH + "border_A_left_top.tga",
				},
				## RightTop 2
				{
					"name" : "RightTop",
					"type" : "image",
					
					"x" : BOARD_WINDOW_WIDTH - 16,
					"y" : 0,
					"image" : PATTERN_PATH + "border_A_right_top.tga",
				},
				## LeftBottom 3
				{
					"name" : "LeftBottom",
					"type" : "image",
					
					"x" : 0,
					"y" : BOARD_WINDOW_HEIGHT - 16,
					"image" : PATTERN_PATH + "border_A_left_bottom.tga",
				},
				## RightBottom 4
				{
					"name" : "RightBottom",
					"type" : "image",
					
					"x" : BOARD_WINDOW_WIDTH - 16,
					"y" : BOARD_WINDOW_HEIGHT - 16,
					"image" : PATTERN_PATH + "border_A_right_bottom.tga",
				},
				## topcenterImg 5
				{
					"name" : "TopCenterImg",
					"type" : "expanded_image",
					
					"x" : 16,
					"y" : 0,
					"image" : PATTERN_PATH + "border_A_top.tga",
					"rect" : (0.0, 0.0, PATTERN_X_COUNT, 0),
				},
				## leftcenterImg 6
				{
					"name" : "LeftCenterImg",
					"type" : "expanded_image",
					
					"x" : 0,
					"y" : 16,
					"image" : PATTERN_PATH + "border_A_left.tga",
					"rect" : (0.0, 0.0, 0, PATTERN_Y_COUNT),
				},
				## rightcenterImg 7
				{
					"name" : "RightCenterImg",
					"type" : "expanded_image",
					
					"x" : BOARD_WINDOW_WIDTH - 16,
					"y" : 16,
					"image" : PATTERN_PATH + "border_A_right.tga",
					"rect" : (0.0, 0.0, 0, PATTERN_Y_COUNT),
				},
				## bottomcenterImg 8
				{
					"name" : "BottomCenterImg",
					"type" : "expanded_image",
					
					"x" : 16,
					"y" : BOARD_WINDOW_HEIGHT - 16,
					"image" : PATTERN_PATH + "border_A_bottom.tga",
					"rect" : (0.0, 0.0, PATTERN_X_COUNT, 0),
				},
				## centerImg
				{
					"name" : "CenterImg",
					"type" : "expanded_image",
					
					"x" : 16,
					"y" : 16,
					"image" : PATTERN_PATH + "border_A_center.tga",
					"rect" : (0.0, 0.0, PATTERN_X_COUNT, PATTERN_Y_COUNT),
				},
			),
		},

	
		## prev post list button
		{
			"name" : "prev_button",
			"type" : "button",

			"x" : 246,
			"y" : 373,	#394

			"default_image" : PUBLIC_PATH + "public_intro_btn/prev_btn_01.sub",
			"over_image" : PUBLIC_PATH + "public_intro_btn/prev_btn_02.sub",
			"down_image" : PUBLIC_PATH + "public_intro_btn/prev_btn_01.sub",
		},
		## next post list button
		{
			"name" : "next_button",
			"type" : "button",

			"x" : 275,
			"y" : 373,	#394

			"default_image" : PUBLIC_PATH + "public_intro_btn/next_btn_01.sub",
			"over_image" : PUBLIC_PATH + "public_intro_btn/next_btn_02.sub",
			"down_image" : PUBLIC_PATH + "public_intro_btn/next_btn_01.sub",
		},
		
		# post list window
		{
			"name" : "post_list_window",
			"type" : "window",
			"x" : 15, 
			"y" : 37,	#58
			"width" : 285, 
			"height" : 34*9+3*8, 
			"style" : ("attach",),
		},
		
		# post all delete button
		{
			"name" : "post_all_delete_button",
			"type" : "button",

			"x" : 13,
			"y" : 395,	#416

			"default_image" : "d:/ymir work/ui/public/Large_Button_01.sub",
			"over_image" : "d:/ymir work/ui/public/Large_Button_02.sub",
			"down_image" : "d:/ymir work/ui/public/Large_Button_03.sub",
			"disable_image" : "d:/ymir work/ui/public/Large_Button_03.sub",
			
			"text" : uiScriptLocale.MAILBOX_POST_ALL_DELETE_BUTTON,
		},
		# post all receive button
		{
			"name" : "post_all_receive_button",
			"type" : "button",

			"x" : 101,
			"y" : 395,	#416

			"default_image" : "d:/ymir work/ui/public/Large_Button_01.sub",
			"over_image" : "d:/ymir work/ui/public/Large_Button_02.sub",
			"down_image" : "d:/ymir work/ui/public/Large_Button_03.sub",
			"disable_image" : "d:/ymir work/ui/public/Large_Button_03.sub",
			
			"text" : uiScriptLocale.MAILBOX_POST_ALL_RECEIVE,
		},
		
		# post write button
		{
			"name" : "post_write_button",
			"type" : "button",

			"x" : 245,
			"y" : 397,	#418

			"default_image" : ROOT + "post_write_button_default.sub",
			"over_image" : ROOT + "post_write_button_over.sub",
			"down_image" : ROOT + "post_write_button_down.sub",
		},
	),	
}
