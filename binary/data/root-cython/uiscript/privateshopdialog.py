import uiScriptLocale

BOARD_WIDTH = 345#184
BOARD_HEIGHT = 354
COUNT_X = 5
COUNT_Y = 8
window = {
	"name" : "PrivateShopDialog",

	"x" : SCREEN_WIDTH - 400,
	"y" : 10,

	"style" : ("movable", "float",),

	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 169,
					"color" : "gray",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":84, "y":4, "text":uiScriptLocale.SHOP_TITLE, "text_horizontal_align":"center" },
					),
				},

				## Item Slot
				{
					"name" : "ItemSlot1",
					"type" : "grid_table",

					"x" : 12,
					"y" : 34,

					"start_index" : 0,
					"x_count" : COUNT_X,
					"y_count" : COUNT_Y,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
				},
				
				# Item Slot2
				{
					"name" : "ItemSlot2",
					"type" : "grid_table",

					"x" : 12 + 160 + 1,
					"y" : 34,

					"start_index" : COUNT_X*COUNT_Y,
					"x_count" : COUNT_X,
					"y_count" : COUNT_Y,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
				},
				
				## MyshopDeco Tab1
				{
					"name" : "tab1",
					"type" : "radio_button",
					"text" : "I",
					
					"x" : 12 + 32 + 32,
					"y" : 295,
					
					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
				},
				
				## MyshopDeco Tab2
				{
					"name" : "tab2",
					"type" : "radio_button",
					"text" : "II",
					
					"x" : 12 + 160 + 1 + 32 + 32,
					"y" : 295,
					
					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
				},
				
				## Close
				{
					"name" : "CloseButton",
					"type" : "button",

					"x" : 0,
					"y" : BOARD_HEIGHT - 33,

					#"horizontal_align" : "center",

					"text" : uiScriptLocale.PRIVATE_SHOP_CLOSE_BUTTON,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
			),
		},
	),
}