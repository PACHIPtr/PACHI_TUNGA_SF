import uiScriptLocale

IN_GAME_UI_WIDTH	= 240
IN_GAME_UI_HEIGHT	= 180
REWARD_UI_HEIGHT	= 67

SLOT_WIDTH = 32
SLOT_HEIGHT = 32

window = {
	"name" : "LuckyBoxWindow",
	"style" : ("movable", "float",),

	"x" : SCREEN_WIDTH - 176 - IN_GAME_UI_WIDTH + 5,
	"y" : SCREEN_HEIGHT - 37 - 565 + 209 + 32 - IN_GAME_UI_HEIGHT + 5,

	"width" : IN_GAME_UI_WIDTH,
	"height" : IN_GAME_UI_HEIGHT,

	"children" :
	[
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,
			
			"width" : IN_GAME_UI_WIDTH,
			"height" : IN_GAME_UI_HEIGHT,

			"children" :
			[
				{
					"name" : "LuckyBox_TitleBar", "type" : "titlebar", "x" : 0, "y" : 5, "width" : IN_GAME_UI_WIDTH - 5, "height" : 21, "style" : ("attach",),
					"children" :
					(
						{"name":"TitleName", "type":"text", "x":0, "y":0, "text":uiScriptLocale.LUCKY_BOX_TITLE_NAME,  "all_align" : "center",},
					),
				},
				
				{ "name":"LuckyBox_Reward", "type":"horizontalbar", "x":10, "y":IN_GAME_UI_HEIGHT / 2 - SLOT_HEIGHT - 20, "width":IN_GAME_UI_WIDTH - 20, },
				{ "name":"LuckyBox_Name", "type":"text", "x":15, "y":IN_GAME_UI_HEIGHT / 2 - SLOT_HEIGHT - 20 + 2, "text":uiScriptLocale.LUCKY_BOX_RECV_NAME,},
								
				{					
					"name" : "RewardItemSlot",
					"type" : "slot",
					
					"x" : 15,
					"y" : IN_GAME_UI_HEIGHT / 2 - SLOT_HEIGHT + 12,
					"width" : SLOT_WIDTH,
					"height" : IN_GAME_UI_HEIGHT,
					
					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
					
					"slot" : 
					(
						{"index":0, "x":0, "y":0, "width":SLOT_WIDTH, "height":SLOT_HEIGHT},
					),						
				},
				
				## Button
				{
					"name" : "RetryButton",
					"type" : "button",

					"x" : 15 + SLOT_WIDTH + 5,
					"y" : IN_GAME_UI_HEIGHT / 2,

					"text" : uiScriptLocale.LUCKY_BOX_RETRY_BUTTON,

					"default_image" : "d:/ymir work/ui/event/long_button_01.sub",
					"over_image" : "d:/ymir work/ui/event/long_button_02.sub",
					"down_image" : "d:/ymir work/ui/event/long_button_03.sub",
				},
				{
					"name" : "RecvButton",
					"type" : "button",

					"x" : 15 + SLOT_WIDTH + 5,
					"y" : IN_GAME_UI_HEIGHT / 2 - (SLOT_HEIGHT),

					"text" : uiScriptLocale.LUCKY_BOX_RECV_BUTTON,

					"default_image" : "d:/ymir work/ui/event/long_button_01.sub",
					"over_image" : "d:/ymir work/ui/event/long_button_02.sub",
					"down_image" : "d:/ymir work/ui/event/long_button_03.sub",
				},
				
				{ "name":"Notice_Yang", "type":"horizontalbar", "x":10, "y":IN_GAME_UI_HEIGHT - 55, "width":IN_GAME_UI_WIDTH - 20, },
				{ "name":"RecycleTitleName", "type":"text", "x":15, "y":IN_GAME_UI_HEIGHT - 55 + 2, "text":uiScriptLocale.LUCKY_BOX_NOTICE_YANG,},				
				
				{
					"name":"NeedMoneySlot",
					"type":"button",

					"x" : 15 + SLOT_WIDTH + 5,
					"y" : IN_GAME_UI_HEIGHT - 40,

					"default_image" : "d:/ymir work/ui/public/gold_slot.sub",
					"over_image" : "d:/ymir work/ui/public/gold_slot.sub",
					"down_image" : "d:/ymir work/ui/public/gold_slot.sub",

					"children" :
					(
						{
							"name" : "NeedMoney",
							"type" : "text",

							"x" : -75,
							"y" : 3,

							"horizontal_align" : "right",
							"text_horizontal_align" : "right",

							"text" : "50,000",
						},
					),
				},
			],
		},
	],
}