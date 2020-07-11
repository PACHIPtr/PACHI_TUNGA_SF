import uiScriptLocale

window = {
	"name" : "ShopDialog",

	"x" : SCREEN_WIDTH - 400,
	"y" : 10,

	"style" : ("movable", "float",),

	"width" : 184,
	"height" : 385,
	
	"children" :
	(
		{
			"name" : "BattleShopSubBoard",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 354-5,

			"width" : 184,
			"height" : 40,
			
			"children" :
			(
			{ "name":"BattleShopSubInfoImage", "type":"image", "x":20, "y":13, "image":"d:/ymir work/ui/public/battle/icon_my_point.sub", },
			{ "name":"BattleShopSubInfo1", "type":"text", "x":40, "y":15, "text":"My Point : 0"},
			{ "name":"BattleShopSubInfo2", "type":"text", "x":20, "y":35, "text":"Limit : 0/100"},
			)
		},
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 184,
			"height" : 385,

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
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 12,
					"y" : 34,

					"start_index" : 0,
					"x_count" : 5,
					"y_count" : 9,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
				},
				
				###############
				{
					"name" : "Kackisibakmis",
					"type" : "slotbar",

					"x" : -7+6,
					"y" : 327,

					"width" : 120+45+9,
					"height" : 18,

					"horizontal_align" : "center",

					"children" :
					(
						{
							"name" : "Bakanlar",
							"type" : "text",

							"x" : 15+25,
							"y" : 3,

							"width" : 120,
							"height" : 18,
						},
					),
				},
				###############

				## Buy
				{
					"name" : "BuyButton",
					"type" : "toggle_button",

					"x" : 21,
					"y" : 357,

					"width" : 61,
					"height" : 21,

					"text" : uiScriptLocale.SHOP_BUY,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},

				## Sell
				{
					"name" : "SellButton",
					"type" : "toggle_button",

					"x" : 104,
					"y" : 357,

					"width" : 61,
					"height" : 21,

					"text" : uiScriptLocale.SHOP_SELL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},

				## Close
				{
					"name" : "CloseButton",
					"type" : "button",

					"x" : 0,
					"y" : 327,

					"horizontal_align" : "center",

					"text" : uiScriptLocale.PRIVATE_SHOP_CLOSE_BUTTON,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},

				## MiddleTab1
                {
                    "name" : "MiddleTab1",
                    "type" : "radio_button",

                    "x" : 21,
                    "y" : 295,

                    "width" : 61,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
                },

                ## MiddleTab2
                {
                    "name" : "MiddleTab2",
                    "type" : "radio_button",

                    "x" : 104,
                    "y" : 295,

                    "width" : 61,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
                },

                ## SmallTab1
                {
                    "name" : "SmallTab1",
                    "type" : "radio_button",

                    "x" : 21,
                    "y" : 295,

                    "width" : 43,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/small_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/small_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/small_button_03.sub",
                },

                ## SmallTab2
                {
                    "name" : "SmallTab2",
                    "type" : "radio_button",

                    "x" : 71,
                    "y" : 295,

                    "width" : 43,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/small_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/small_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/small_button_03.sub",
                },

                ## SmallTab3
                {
                    "name" : "SmallTab3",
                    "type" : "radio_button",

                    "x" : 120,
                    "y" : 295,

                    "width" : 43,
                    "height" : 21,

                    "default_image" : "d:/ymir work/ui/public/small_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/small_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/small_button_03.sub",
                },

			),
		},
	),
}