import uiScriptLocale

ROOT = "d:/ymir work/ui/game/mailbox/"
PATTERN_PATH = "d:/ymir work/ui/pattern/"
PUBLIC_PATH = "d:/ymir work/ui/public/"

# MailBox.py
#MAILBOX_WINDOW_WIDTH		= 316
#MAILBOX_WINDOW_HEIGHT		= 455

WINDOW_WIDTH	= 274
WINDOW_HEIGHT	= 316

BOARD_WINDOW_WIDTH	= 253
BOARD_WINDOW_HEIGHT	= 248

window = {
	"name" : "PostRead",
	"style" : ("movable", "float",),
	
	#"x" : SCREEN_WIDTH / 2 - MAILBOX_WINDOW_WIDTH / 2 + MAILBOX_WINDOW_WIDTH -4,
	#"y" : SCREEN_HEIGHT / 2 - MAILBOX_WINDOW_HEIGHT / 2,
	
	"x" : SCREEN_WIDTH - 176 - WINDOW_WIDTH,
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
			
			"title" : uiScriptLocale.MAILBOX_POST_READ_MAIN_TITLE,
		},
		
		## main bg
		{
			"name" : "bg_window",
			"type" : "window",

			"x" : 10,
			"y" : 32,
			
			"width" : BOARD_WINDOW_WIDTH,
			"height" : BOARD_WINDOW_HEIGHT,
			
			"children" :
			(
				## bg
				{
					"name" : "BGImg",
					"type" : "image",
					
					"x" : 0,
					"y" : 0,
					"image" : ROOT + "post_bg.sub",
				},
				## from title
				{ 
					"name" : "from_title_window", "type" : "window", "x" : 4, "y" : 4, "width" : 74, "height" : 16, "style" : ("attach",),
					"children" :
					(
						{"name":"from_title_text", "type":"text", "x":0, "y":0, "text":uiScriptLocale.MAILBOX_POST_READ_FROM, "all_align" : "center"},
					),	
				},
				## from text
				{ 
					"name" : "from_text_window", "type" : "window", "x" : 86, "y" : 4, "width" : 115, "height" : 16, "style" : ("attach",),
					"children" :
					(
						{
							"name":"from_text",
							"type":"text",
							"x":0,
							"y":2,
							"width" : 115,
							"height" : 14,							
							"text":"",
						},
					),
				},
				## title text
				{ 
					"name" : "title_window", "type" : "window", "x" : 4, "y" : 25, "width" : 74, "height" : 16, "style" : ("attach",),
					"children" :
					(
						{"name":"title_window_text", "type":"text", "x":0, "y":0, "text":uiScriptLocale.MAILBOX_POST_READ_TITLE, "all_align" : "center"},
					),	
				},
				## title text
				{ 
					"name" : "title_text_window", "type" : "window", "x" : 86, "y" : 25, "width" : 166, "height" : 16, "style" : ("attach",),
					"children" :
					(
						{
							"name":"title_text",
							"type":"text",
							"x":0,
							"y":2,
							"width" : 166,
							"height" : 14,							
							"text":"",
						},
					),
				},
			
				# block button
				{
					"name" : "block_button",
					"type" : "button",

					"x" : 216,
					"y" : 2,

					"default_image" : ROOT + "post_block_button_default.sub",
					"over_image" : ROOT + "post_block_button_over.sub",
					"down_image" : ROOT + "post_block_button_down.sub",
				},
				
				{
					"name" : "message_line",
					"type" : "text",

					"x" : 8,#3,
					"y" : 50,#45,

					"width" : 235,#247,
					"height" : 99,
				},
			),
		},
		# delete button
		{
			"name" : "delete_button",
			"type" : "button",

			"x" : 48,
			"y" : 282,

			"default_image" : "d:/ymir work/ui/public/Large_Button_01.sub",
			"over_image" : "d:/ymir work/ui/public/Large_Button_02.sub",
			"down_image" : "d:/ymir work/ui/public/Large_Button_03.sub",
			"disable_image" : "d:/ymir work/ui/public/Large_Button_03.sub",
			
			"text" : uiScriptLocale.MAILBOX_POST_READ_DELETE,
		},
		# recv button
		{
			"name" : "recv_button",
			"type" : "button",

			"x" : 136,
			"y" : 282,

			"default_image" : "d:/ymir work/ui/public/Large_Button_01.sub",
			"over_image" : "d:/ymir work/ui/public/Large_Button_02.sub",
			"down_image" : "d:/ymir work/ui/public/Large_Button_03.sub",
			"disable_image" : "d:/ymir work/ui/public/Large_Button_03.sub",
			
			"text" : uiScriptLocale.MAILBOX_POST_READ_RECV,
		},
		
		## won text
		{ 
			"name" : "won_text_window", "type" : "window", "x" : 170, "y" : 181, "width" : 85, "height" : 16, "style" : ("attach",),
			"children" :
			(
				{"name":"won_text", "type":"text", "x":0, "y":2, "text":"0", "horizontal_align" : "right", "text_horizontal_align" : "right" },
			),
		},
		## yang text
		{ 
			"name" : "yang_text_window", "type" : "window", "x" : 170, "y" : 199, "width" : 85, "height" : 16, "style" : ("attach",),
			"children" :
			(
				{"name":"yang_text", "type":"text", "x":0, "y":2, "text":"0", "horizontal_align" : "right", "text_horizontal_align" : "right" },
			),	
		},
		# won & yang window
		{
			"name" : "post_won_yang_window",
			"type" : "window",
			
			"x" : 138,
			"y" : 180,
			"width" : 117,
			"height" : 36,
		},
		## ITEM Slot
		{
			"name" : "item_slot",
			"type" : "grid_table",
			"x" : 61,
			"y" : 180,
			"start_index" : 0,
			"x_count" : 1,
			"y_count" : 3,
			"x_step" : 32,
			"y_step" : 32,
		},		
	),	
}
