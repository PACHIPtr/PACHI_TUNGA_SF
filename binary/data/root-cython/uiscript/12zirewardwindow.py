import uiScriptLocale
import item
import app

ROOT_PATH = "d:/ymir work/ui/game/12zi/reward/"
MISSION_COUNT = "50"
JACHUC_START_X = 32
JACHUC_Y = -8
GAP_X = 40
GAP_START_Y = 140

window = {
	"name" : "12zi_Reward_Window",

	"x" : (SCREEN_WIDTH -500) / 2,
	"y" : (SCREEN_HEIGHT - 467) / 2,

	"style" : ("movable", "float",),

	"width" : 500,
	"height" : 467,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 500,
			"height" :467,
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",

					"x" : 6,
					"y" : 6,

					"width" : 487,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":250, "y":3, "text" : uiScriptLocale.CZ_REWARD_WINDOW_TITLE, "text_horizontal_align":"center" },
					),
				},
				## Background Window
				{
					"name" : "BgWindow",

					"x" : 10,
					"y" : 32,

					"width" : 490,
					"height" : 437,
					
					"children" :
					(				
						{
							"name":"BgImage",
							"type":"image",
							
							"x":0,
							"y":0,

							"image" : ROOT_PATH + "bg_12zi_reward.sub",

						},
				
						{"name":"bg_weekly_row", "type":"image", "x":114, "y":0, "image" : ROOT_PATH + "bg_weekly_row.sub"},
						{"name":"bg_weekly_row_1", "type":"image", "x":142, "y":0, "image" : ROOT_PATH + "bg_weekly_row_1.sub"},
						{"name":"bg_weekly_row_2", "type":"image", "x":198, "y":0, "image" : ROOT_PATH + "bg_weekly_row_2.sub"},
						{"name":"bg_weekly_row_3", "type":"image", "x":254, "y":0, "image" : ROOT_PATH + "bg_weekly_row_3.sub"},
						{"name":"bg_weekly_row_4", "type":"image", "x":310, "y":0, "image" : ROOT_PATH + "bg_weekly_row_4.sub"},
						{"name":"bg_weekly_row_5", "type":"image", "x":366, "y":0, "image" : ROOT_PATH + "bg_weekly_row_5.sub"},
						{"name":"bg_weekly_row_6", "type":"image", "x":422, "y":0, "image" : ROOT_PATH + "bg_weekly_row_6.sub"},
						
						{"name":"bg_charm_12zi", "type":"image", "x":114, "y":30, "image" : ROOT_PATH + "bg_charm_12zi.sub"},
						{"name":"bg_charm_12zi_1", "type":"image", "x":142, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_1.sub"},
						{"name":"bg_charm_12zi_2", "type":"image", "x":170, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_2.sub"},
						{"name":"bg_charm_12zi_3", "type":"image", "x":198, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_3.sub"},
						{"name":"bg_charm_12zi_4", "type":"image", "x":226, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_4.sub"},
						{"name":"bg_charm_12zi_5", "type":"image", "x":254, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_5.sub"},
						{"name":"bg_charm_12zi_6", "type":"image", "x":282, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_6.sub"},
						{"name":"bg_charm_12zi_7", "type":"image", "x":310, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_7.sub"},
						{"name":"bg_charm_12zi_8", "type":"image", "x":338, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_8.sub"},
						{"name":"bg_charm_12zi_9", "type":"image", "x":366, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_9.sub"},
						{"name":"bg_charm_12zi_10", "type":"image", "x":394, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_10.sub"},
						{"name":"bg_charm_12zi_11", "type":"image", "x":422, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_11.sub"},
						{"name":"bg_charm_12zi_12", "type":"image", "x":450, "y":30, "image" : ROOT_PATH + "bg_charm_12zi_12.sub"},
						
						{"name":"bg_count_slot_row", "type":"image", "x":114, "y":58, "image" : ROOT_PATH + "bg_count_slot_row.sub"},
						{"name":"bg_need_count_slot_row", "type":"image", "x":114, "y":86, "image" : ROOT_PATH + "bg_need_count_slot_row.sub"},
						
						{"name":"bg_weekly_column", "type":"image", "x":0, "y":86, "image" : ROOT_PATH + "bg_weekly_column.sub"},
						{"name":"bg_weekly_column_1", "type":"image", "x":0, "y":114, "image" : ROOT_PATH + "bg_weekly_column_1.sub"},
						{"name":"bg_weekly_column_2", "type":"image", "x":0, "y":170, "image" : ROOT_PATH + "bg_weekly_column_2.sub"},
						{"name":"bg_weekly_column_3", "type":"image", "x":0, "y":226, "image" : ROOT_PATH + "bg_weekly_column_3.sub"},
						{"name":"bg_weekly_column_4", "type":"image", "x":0, "y":282, "image" : ROOT_PATH + "bg_weekly_column_4.sub"},
						{"name":"bg_weekly_column_5", "type":"image", "x":0, "y":338, "image" : ROOT_PATH + "bg_weekly_column_5.sub"},
						{"name":"bg_weekly_column_6", "type":"image", "x":30, "y":114, "image" : ROOT_PATH + "bg_weekly_column_6.sub"},
						
						{"name":"bg_charm_10gan", "type":"image", "x":58, "y":86, "image" : ROOT_PATH + "bg_charm_10gan.sub"},
						{"name":"bg_charm_10gan_1", "type":"image", "x":58, "y":114, "image" : ROOT_PATH + "bg_charm_10gan_1.sub"},
						{"name":"bg_charm_10gan_2", "type":"image", "x":58, "y":142, "image" : ROOT_PATH + "bg_charm_10gan_2.sub"},
						{"name":"bg_charm_10gan_3", "type":"image", "x":58, "y":170, "image" : ROOT_PATH + "bg_charm_10gan_3.sub"},
						{"name":"bg_charm_10gan_4", "type":"image", "x":58, "y":198, "image" : ROOT_PATH + "bg_charm_10gan_4.sub"},
						{"name":"bg_charm_10gan_5", "type":"image", "x":58, "y":226, "image" : ROOT_PATH + "bg_charm_10gan_5.sub"},
						{"name":"bg_charm_10gan_6", "type":"image", "x":58, "y":254, "image" : ROOT_PATH + "bg_charm_10gan_6.sub"},
						{"name":"bg_charm_10gan_7", "type":"image", "x":58, "y":282, "image" : ROOT_PATH + "bg_charm_10gan_7.sub"},
						{"name":"bg_charm_10gan_8", "type":"image", "x":58, "y":310, "image" : ROOT_PATH + "bg_charm_10gan_8.sub"},
						{"name":"bg_charm_10gan_9", "type":"image", "x":58, "y":338, "image" : ROOT_PATH + "bg_charm_10gan_9.sub"},
						{"name":"bg_charm_10gan_10", "type":"image", "x":58, "y":366, "image" : ROOT_PATH + "bg_charm_10gan_10.sub"},
						
						{"name":"bg_count_slot_column", "type":"image", "x":86, "y":86, "image" : ROOT_PATH + "bg_count_slot_column.sub"},
						{"name":"bg_need_count_slot_column", "type":"image", "x":114, "y":86, "image" : ROOT_PATH + "bg_need_count_slot_column.sub"},
						
						{"name":"bg_check", "type":"image", "x":142, "y":114, "image" : ROOT_PATH + "bg_check.sub"},

						{ "name":"12zi1", "type":"text", "x":155, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi2", "type":"text", "x":155+28*1, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi3", "type":"text", "x":155+28*2, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi4", "type":"text", "x":155+28*3, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi5", "type":"text", "x":155+28*4, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi6", "type":"text", "x":155+28*5, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi7", "type":"text", "x":155+28*6, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi8", "type":"text", "x":155+28*7, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi9", "type":"text", "x":155+28*8, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi10", "type":"text", "x":155+28*9, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi11", "type":"text", "x":155+28*10, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"12zi12", "type":"text", "x":155+28*11, "y":125-32, "text" : MISSION_COUNT, "text_horizontal_align":"center" },

						{ "name":"10gan1", "type":"text", "x":127, "y":153-32+28*0, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan2", "type":"text", "x":127, "y":153-32+28*1, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan3", "type":"text", "x":127, "y":153-32+28*2, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan4", "type":"text", "x":127, "y":153-32+28*3, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan5", "type":"text", "x":127, "y":153-32+28*4, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan6", "type":"text", "x":127, "y":153-32+28*5, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan7", "type":"text", "x":127, "y":153-32+28*6, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan8", "type":"text", "x":127, "y":153-32+28*7, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan9", "type":"text", "x":127, "y":153-32+28*8, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						{ "name":"10gan10", "type":"text", "x":127, "y":153-32+28*9, "text" : MISSION_COUNT, "text_horizontal_align":"center" },
						
					),
				},

				## Check				
				{
					"name" : "CheckButtonWindow",
					"type" : "window",

					"x" : 153,
					"y" : 147,
					"width" : 336,
					"height" :280,
				},
				
				## Button
				{
					"name" : "YellowInactive",
					"type" : "image",

					"x" : 11,
					"y" : 430,

					"image" : ROOT_PATH + "btn_yellow_inactive.sub",
				},
				{
					"name" : "YellowButton",
					"type" : "button",

					"x" : 11,
					"y" : 430,

					"default_image" : ROOT_PATH + "btn_yellow_default.sub",
					"over_image" : ROOT_PATH + "btn_yellow_over.sub",
					"down_image" : ROOT_PATH + "btn_yellow_down.sub",
				},
				{
					"name" : "YellowRewardCount",
					"type" : "numberline",
					
					"x" : 11+60,
					"y" : 430+12,
				},
				{
					"name" : "GreenInactive",
					"type" : "image",

					"x" : 99,
					"y" : 430,

					"image" : ROOT_PATH + "btn_green_inactive.sub",
				},
				{
					"name" : "GreenButton",
					"type" : "button",

					"x" : 99,
					"y" : 430,

					"default_image" : ROOT_PATH + "btn_green_default.sub",
					"over_image" : ROOT_PATH + "btn_green_over.sub",
					"down_image" : ROOT_PATH + "btn_green_down.sub",
				},				
				{
					"name" : "GreenRewardCount",
					"type" : "numberline",
					
					"x" : 99+60,
					"y" : 430+12,
				},
				{
					"name" : "AllClearButton",
					"type" : "button",

					"x" : 391,
					"y" : 430,

					"default_image" : ROOT_PATH + "btn_gold_default.sub",
					"over_image" : ROOT_PATH + "btn_gold_over.sub",
					"down_image" : ROOT_PATH + "btn_gold_down.sub",
				},
			),
		},
	),
}

