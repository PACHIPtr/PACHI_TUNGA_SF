import uiScriptLocale

ROOT_PATH = "d:/ymir work/ui/public/battle/"
BACK_IMG_PATH = "d:/ymir work/ui/pattern/"

window = {
	"name" : "CombatZoneWindow",
	"x" : (SCREEN_WIDTH -639) / 2,
	"y" : (SCREEN_HEIGHT - 400) / 2,
	"style" : ("movable","float",),
	"width" : 639,
	"height" : 400,
	"children" :
	(
		{
			"name" : "PotionCollectWindow",
			"type" : "board",
			"style" : ("attach", "float"),
			
			"x" : 505,
			"y" : 250,
			
			"width" : 138,
			"height" : 90,
			"children" :
			(
				## Slot
				{
					"name" : "itemSlotWindow",
					"type" : "grid_table",
					"x" : 15,
					"y" : 52-36,
					"start_index" : 0,
					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},

				## Button
				{
					"name" : "btnRequest",
					"type" : "button",
					
					"x" : 30,
					"y" : 52-36+45,
					
					"text" : uiScriptLocale.BATTLE_FIELD_REQUEST_POTION,
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},

				{
					"name" : "lineBG",
					"type" : "line",
					
					"x" : 60,
					"y" : 10,
					
					"width" : 0,
					"height" : 43,
					"color" : 0xff646d6d,
				},

				{
					"name" : "slotbarBG",
					"type" : "slotbar",
					
					"x" : 66,
					"y" : 57-24,
					
					"width" : 60,
					"height" : 21,
					"children" :
					(
						{
							"name" : "monstersKilled",
							"type" : "text",
							"x" : 0,
							"y" : 0,
							"all_align" : "center",
							"text" : "0/0",
						},
					),
				},
				
				{
					"name" : "time",
					"type" : "slotbar",
					
					"x" : 66,
					"y" : 57-47,
					
					"width" : 60,
					"height" : 21,
					"children" :
					(
						{
							"name" : "timeRemaining",
							"type" : "text",
							"x" : 0,
							"y" : 0,
							"all_align" : "center",
							"text" : "00:00:00",
						},
					),
				},
			),
		},
	
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 518,
			"height" : 400,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 7,

					"width" : 503,
					"color" : "yellow",

					"children" :
					(
						{ "name":"title_name", "type":"text", "x":0, "y":-1, "text": uiScriptLocale.BATTLE_FIELD_INFO, "all_align":"center" },
					),
				},

				## Explanation bg
				{
					"name" : "explanation",
					"type" : "window",

					"x" : 14,
					"y" : 36,

					"width" : 169,
					"height" : 326,

					"children" :
					(
						## LeftTop
						{
							"name" : "left_top",
							"type" : "image",
							"x" : 0,
							"y" : 0,
							"image" : BACK_IMG_PATH+"border_A_left_top.tga",
						},
						## RightTop
						{
							"name" : "right_top",
							"type" : "image",
							"x" : 169-16,
							"y" : 0,
							"image" : BACK_IMG_PATH+"border_A_right_top.tga",
						},
						## LeftBottom
						{
							"name" : "left_bottom",
							"type" : "image",
							"x" : 0,
							"y" : 326-16,
							"image" : BACK_IMG_PATH+"border_A_left_bottom.tga",
						},
						## RightBottom
						{
							"name" : "right_bottom",
							"type" : "image",
							"x" : 169-16,
							"y" : 326-16,
							"image" : BACK_IMG_PATH+"border_A_right_bottom.tga",
						},
						## leftcenterImg
						{
							"name" : "left_center_img",
							"type" : "expanded_image",
							"x" : 0,
							"y" : 15,
							"image" : BACK_IMG_PATH+"border_A_left.tga",
							"rect" : (0.0, 0.0, 0, 18),
						},
						## rightcenterImg
						{
							"name" : "right_center_img",
							"type" : "expanded_image",
							"x" : 169-16,
							"y" : 15,
							"image" : BACK_IMG_PATH+"border_A_right.tga",
							"rect" : (0.0, 0.0, 0, 18),
						},
						## topcenterImg
						{
							"name" : "top_center_img",
							"type" : "expanded_image",
							"x" : 15,
							"y" :  0,
							"image" : BACK_IMG_PATH+"border_A_top.tga",
							"rect" : (0.0, 0.0, 8, 0),
						},
						## bottomcenterImg
						{
							"name" : "bottom_center_img",
							"type" : "expanded_image",
							"x" : 15,
							"y" : 326-16,
							"image" : BACK_IMG_PATH+"border_A_bottom.tga",
							"rect" : (0.0, 0.0, 8, 0),
						},
						## centerImg
						{
							"name" : "center_img",
							"type" : "expanded_image",
							"x" : 15,
							"y" : 15,
							"image" : BACK_IMG_PATH+"border_A_center.tga",
							"rect" : (0.0, 0.0, 8, 18),
						},

						{
							"name" : "scrollbar",
							"type" : "scrollbar",

							"x" : 149,
							"y" : 132,
							"size" : 190,
						},
					),
				},
				
				{
					"name" : "text_board",
					"type" : "window",
					"x" : 14,
					"y" : 160,
					"width" : 169-16,
					"height" : 326-16,
				},
				
						## separator timezone
						{
							"name" : "separator_center_img",
							"type" : "expanded_image",
							"x" : 16,
							"y" : 150,
							"image" : BACK_IMG_PATH+"border_A_bottom.tga",
							"rect" : (0.0, 0.0, 9.3, 0),
						},

				## RankingList bg
				{
					"name" : "ranking_list",
					"type" : "window",

					"x" : 191,
					"y" : 66,

					"width" : 312,
					"height" : 296,
					
					"children" :
					(
						## LeftTop
						{
							"name" : "left_top",
							"type" : "image",
							"x" : 0,
							"y" : 0,
							"image" : BACK_IMG_PATH+"border_A_left_top.tga",
						},
						
						## RightTop
						{
							"name" : "right_top",
							"type" : "image",
							"x" : 312-16,
							"y" : 0,
							"image" : BACK_IMG_PATH+"border_A_right_top.tga",
						},
						## LeftBottom
						{
							"name" : "left_bottom",
							"type" : "image",
							"x" : 0,
							"y" : 296-16,
							"image" : BACK_IMG_PATH+"border_A_left_bottom.tga",
						},
						## RightBottom
						{
							"name" : "right_bottom",
							"type" : "image",
							"x" : 312-16,
							"y" : 296-16,
							"image" : BACK_IMG_PATH+"border_A_right_bottom.tga",
						},
						## leftcenterImg
						{
							"name" : "left_center_img",
							"type" : "expanded_image",
							"x" : 0,
							"y" : 15,
							"image" : BACK_IMG_PATH+"border_A_left.tga",
							"rect" : (0.0, 0.0, 0, 16),
						},
						## rightcenterImg
						{
							"name" : "right_center_img",
							"type" : "expanded_image",
							"x" : 312-16,
							"y" : 15,
							"image" : BACK_IMG_PATH+"border_A_right.tga",
							"rect" : (0.0, 0.0, 0, 16),
						},
						## topcenterImg
						{
							"name" : "top_center_img",
							"type" : "expanded_image",
							"x" : 15,
							"y" :  0,
							"image" : BACK_IMG_PATH+"border_A_top.tga",
							"rect" : (0.0, 0.0, 17, 0),
						},
						## bottomcenterImg
						{
							"name" : "bottom_center_img",
							"type" : "expanded_image",
							"x" : 15,
							"y" : 296-16,
							"image" : BACK_IMG_PATH+"border_A_bottom.tga",
							"rect" : (0.0, 0.0, 17, 0),
						},
						## centerImg
						{
							"name" : "center_img",
							"type" : "expanded_image",
							"x" : 15,
							"y" : 15,
							"image" : BACK_IMG_PATH+"border_A_center.tga",
							"rect" : (0.0, 0.0, 17, 16),
						},
					),	
				},

				## Tab Area
				{
					"name" : "tab_control",
					"type" : "window",

					"x" : 191,
					"y" : 36,

					"width" : 312,
					"height" : 33,

					"children" :
					(
						## Tab
						{
							"name" : "tab_01",
							"type" : "image",

							"x" : 0,
							"y" : 0,

							"width" : 312,
							"height" : 33,

							"image" : ROOT_PATH+"tab_current_rank.sub",
						},
						{
							"name" : "tab_02",
							"type" : "image",
							"x" : 0,
							"y" : 0,
							"width" : 312,
							"height" : 33,
							"image" : ROOT_PATH+"tab_accum_rank.sub",
						},
						## RadioButton ##
						{
							"name" : "tab_button_01",
							"type" : "radio_button",
							"x" : 0,
							"y" : 0,
							"tooltip_text" : uiScriptLocale.WEEKLY_RANKING,
							"tooltip_x" : 0,
							"tooltip_y" : -10,
							"width" : 120,
							"height" : 33,
						},
						{
							"name" : "tab_button_02",
							"type" : "radio_button",
							"x" : 124,
							"y" : 0,
							"tooltip_text" : uiScriptLocale.TOTAL_RANKING,
							"tooltip_x" : 0,
							"tooltip_y" : -10,
							"width" : 120,
							"height" : 33,
						},
					),
				},

				## List Column Titlebar
				{
					"name" : "list",
					"type" : "window",

					"x" : 194,
					"y" : 69,

					"width" : 306,
					"height" : 21,

					"children" :
					(
						{
							"name" : "sub_titlebar",
							"type" : "image",

							"x" : 0,
							"y" : 0,

							"image" : ROOT_PATH + "column_titlebar.sub",

							"children" :
							(
								{ "name":"column_rank", "type":"text", "x":227-194, "y":74-69, "text":uiScriptLocale.BATTLE_FILED_RANK, "r":1.0, "g":1.0, "b":1.0, "a":1.0, "text_horizontal_align":"center" },
								{ "name":"column_name", "type":"text", "x":320-194, "y":74-69, "text":uiScriptLocale.BATTLE_FILED_RANK_NAME, "r":1.0, "g":1.0, "b":1.0, "a":1.0, "text_horizontal_align":"center" },
								{ "name":"column_empire", "type":"text", "x":400-194, "y":74-69, "text":uiScriptLocale.BATTLE_FILED_RANK_EMPIRE, "r":1.0, "g":1.0, "b":1.0, "a":1.0, "text_horizontal_align":"center" },
								{ "name":"column_point", "type":"text", "x":455-194, "y":74-69, "text":uiScriptLocale.BATTLE_FILED_RANK_POINT, "r":1.0, "g":1.0, "b":1.0, "a":1.0, "text_horizontal_align":"center" },

							),
						},
					),
				},

				## Battle info
				{
					"name" : "battle_info",
					"type" : "window",

					"x" : 0,
					"y" : 360,

					"width" : 518,
					"height" : 40,

					"children" :
					(
						{
							"name" : "point_icon",
							"type" : "image",
							"x" : 17,
							"y" : 10,
							"image" : ROOT_PATH+"icon_my_point.sub",
						},
						
						{ 
							"name": "my_point", 
							"type": "text",
							"x": 41, 
							"y": 18, 
							"text": "999", 
							"text_vertical_align": "center" 
						},
						
						{
							"name" : "notice_icon",
							"type" : "image",
							"x" : 133,
							"y" : 10,
							"image" : ROOT_PATH + "icon_notice.sub",
						},
						
						{ "name":"notice", "type":"text", "x":160, "y":18, "text": uiScriptLocale.BATTLE_FIELD_OPERATION_OFFLINE, "text_vertical_align":"center" },
						
						{
							"name" : "enter_button",
							"type" : "button",

							"x" : 416,
							"y" : 8,

							"text" : uiScriptLocale.ENTER_BATTLE_FIELD,
							"horizontal_align" : "left",
							"vertical_align" : "top",

							"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
						},
					),
				},
			),
		},
	),
}