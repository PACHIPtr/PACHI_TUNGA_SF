import uiScriptLocale

ROOT = "d:/ymir work/ui/game/"

WINDOW_WIDTH	= 352
WINDOW_HEIGHT	= 384

BOARD_WIDTH		= 323
BOARD_HEIGHT	= 139

NUMBER_BUTTON_WIDTH = 72
NUMBER_BUTTON_HEIGHT = 60

NUMBER_BUTTON_GAP_X = 6
NUMBER_BUTTON_GAP_Y = 5

BOARD_PATH = "d:/ymir work/ui/minigame/attendance/number_board/"

window = {
	"name" : "AttendancePage",
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
						{ "name":"TitleName", "type":"text", "x":0, "y":0, "text": uiScriptLocale.ATTENDANCE_TITLE, "all_align":"center" },
					),
				},
				
				
				{
					"name" : "number_board_window",
					"type" : "window",
					
					"x" : 13,
					"y" : 42,
					
					"width" : 325,
					"height" : 171,
					
					"children" :
					(
						## LeftTop 1
						{
							"name" : "LeftTop",
							"type" : "image",
							"x" : 0,
							"y" : 0,
							"image" : BOARD_PATH + "lefttop.sub",
						},
						## RightTop 2
						{
							"name" : "RightTop",
							"type" : "image",
							"x" : 309,
							"y" : 0,
							"image" : BOARD_PATH + "righttop.sub",
						},
						## LeftBottom 3
						{
							"name" : "LeftBottom",
							"type" : "image",
							"x" : 0,
							"y" : 155,
							"image" : BOARD_PATH + "leftbottom.sub",
						},
						## RightBottom 4
						{
							"name" : "RightBottom",
							"type" : "image",
							"x" : 309,
							"y" : 155,
							"image" : BOARD_PATH + "rightbottom.sub",
						},
						## topcenterImg 5
						{
							"name" : "TopCenterImg",
							"type" : "expanded_image",
							"x" : 16,
							"y" : 0,
							"image" : BOARD_PATH + "topcenterImg.tga",
							"rect" : (0.0, 0.0, 17, 0),
						},
						## leftcenterImg 6
						{
							"name" : "LeftCenterImg",
							"type" : "expanded_image",
							"x" : 0,
							"y" : 16,
							"image" : BOARD_PATH + "leftcenterImg.tga",
							"rect" : (0.0, 0.0, 0, 8),
						},
						## rightcenterImg 7
						{
							"name" : "RightCenterImg",
							"type" : "expanded_image",
							"x" : 308,
							"y" : 16,
							"image" : BOARD_PATH + "rightcenterImg.tga",
							"rect" : (0.0, 0.0, 0, 8),
						},
						## bottomcenterImg 8
						{
							"name" : "BottomCenterImg",
							"type" : "expanded_image",
							"x" : 16,
							"y" : 155,
							"image" : BOARD_PATH + "bottomcenterImg.tga",
							"rect" : (0.0, 0.0, 17, 0),
						},
						## centerImg
						{
							"name" : "CenterImg",
							"type" : "expanded_image",
							"x" : 16,
							"y" : 16,
							"image" : BOARD_PATH + "centerImg.tga",
							"rect" : (0.0, 0.0, 17, 8),
						},
						
						# number button 1
						{
							"name" : "disable_number_button1",
							"type" : "button",

							"x" : 10,
							"y" : 18,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number1.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number1.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number1.sub",
						},
						# number button 2
						{
							"name" : "disable_number_button2",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH + NUMBER_BUTTON_GAP_X,
							"y" : 18,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number2.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number2.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number2.sub",
						},
						# number button 3
						{
							"name" : "disable_number_button3",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH * 2 + NUMBER_BUTTON_GAP_X * 2,
							"y" : 18,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number3.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number3.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number3.sub",
						},
						# number button 4
						{
							"name" : "disable_number_button4",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH * 3 + NUMBER_BUTTON_GAP_X * 3,
							"y" : 18,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number4.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number4.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number4.sub",
						},
						# number button 5
						{
							"name" : "disable_number_button5",
							"type" : "button",

							"x" : 10,
							"y" : 18 + NUMBER_BUTTON_HEIGHT + NUMBER_BUTTON_GAP_Y,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number5.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number5.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number5.sub",
						},
						# number button 6
						{
							"name" : "disable_number_button6",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH + NUMBER_BUTTON_GAP_X,
							"y" : 18 + NUMBER_BUTTON_HEIGHT + NUMBER_BUTTON_GAP_Y,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number6.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number6.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number6.sub",
						},
						# number button 7
						{
							"name" : "disable_number_button7",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH * 2 + NUMBER_BUTTON_GAP_X * 2,
							"y" : 18 + NUMBER_BUTTON_HEIGHT + NUMBER_BUTTON_GAP_Y,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number7.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number7.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number7.sub",
						},
						# number button 8
						{
							"name" : "disable_number_button8",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH * 3 + NUMBER_BUTTON_GAP_X * 3,
							"y" : 18 + NUMBER_BUTTON_HEIGHT + NUMBER_BUTTON_GAP_Y,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number8.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number8.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number8.sub",
						},
						# number button 9
						{
							"name" : "disable_number_button9",
							"type" : "button",

							"x" : 10,
							"y" : 18,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number9.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number9.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number9.sub",
						},
						# number button 10
						{
							"name" : "disable_number_button10",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH + NUMBER_BUTTON_GAP_X,
							"y" : 18,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number10.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number10.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number10.sub",
						},
						# number button 11
						{
							"name" : "disable_number_button11",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH * 2 + NUMBER_BUTTON_GAP_X * 2,
							"y" : 18,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number11.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number11.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number11.sub",
						},
						# number button 12
						{
							"name" : "disable_number_button12",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH * 3 + NUMBER_BUTTON_GAP_X * 3,
							"y" : 18,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number12.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number12.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number12.sub",
						},
						# number button 13
						{
							"name" : "disable_number_button13",
							"type" : "button",

							"x" : 10,
							"y" : 18 + NUMBER_BUTTON_HEIGHT + NUMBER_BUTTON_GAP_Y,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number13.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number13.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number13.sub",
						},
						# number button 14
						{
							"name" : "disable_number_button14",
							"type" : "button",

							"x" : 10 + NUMBER_BUTTON_WIDTH + NUMBER_BUTTON_GAP_X,
							"y" : 18 + NUMBER_BUTTON_HEIGHT + NUMBER_BUTTON_GAP_Y,

							"default_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number14.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number14.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/attendance_disable_number14.sub",
						},
						{
							"name" : "prev_button",
							"type" : "button",

							"x" : 160 - 13,
							"y" : 192 - 42,

							"default_image" : "d:/ymir work/ui/minigame/attendance/prev_btn.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/prev_btn.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/prev_btn.sub",
						},
						{
							"name" : "next_button",
							"type" : "button",

							"x" : 184 - 13,
							"y" : 192 - 42,

							"default_image" : "d:/ymir work/ui/minigame/attendance/next_btn.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/next_btn.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/next_btn.sub",
						},
						{
							"name" : "number_effect",
							"type" : "ani_image",
							
							"x" : 10,
							"y" : 18,
							
							"delay" : 6,

							"images" :
							(
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect2.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect3.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect4.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect5.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect4.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect3.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect2.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect1.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect1.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect1.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect1.sub",
								"d:/ymir work/ui/minigame/attendance/number_effect/attendance_button_effect1.sub",
							),
						},
					),
				},
		
				{
					"name" : "desc_board",
					"type" : "bar",
					
					"x" : 14,
					"y" : 224,

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
							"name" : "desc_prev_button",
							"type" : "button",

							"x" : 250,
							"y" : 118,

							"default_image" : "d:/ymir work/ui/minigame/attendance/desc_prev_btn.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/desc_prev_btn.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/desc_prev_btn.sub",
						},
						{
							"name" : "desc_next_button",
							"type" : "button",

							"x" : 284,
							"y" : 118,

							"default_image" : "d:/ymir work/ui/minigame/attendance/desc_next_btn.sub",
							"over_image" : "d:/ymir work/ui/minigame/attendance/desc_next_btn.sub",
							"down_image" : "d:/ymir work/ui/minigame/attendance/desc_next_btn.sub",
						},
						
					),
				
				},
				
			),
		},
	),	
}
