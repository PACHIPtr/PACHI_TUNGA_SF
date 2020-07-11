import uiScriptLocale
import localeInfo
import app

if app.ENABLE_CHEQUE_SYSTEM :
	BOARD_HEIGHT = 150
	window = {
		"name" : "InputDialog",

		"x" : 0,
		"y" : 0,

		"style" : ("movable", "float",),

		"width" : 200,
		"height" : BOARD_HEIGHT,

		"children" :
		(
			{
				"name" : "board",
				"type" : "board_with_titlebar",

				"x" : 0,
				"y" : 0,

				"width" : 200,
				"height" : BOARD_HEIGHT,

				"title" : "",

				"children" :
				(
					## Input Slot Cheque
					{
						"name" : "InputSlot_Cheque",
						"type" : "slotbar",

						"x" : 20+15,
						"y" : 34,
						"width" : 24,
						"height" : 18,
						#"horizontal_align" : "center",

						"children" :
						(
							{
								"name":"Cheque_Icon",
								"type":"image",

								"x":-18,
								"y":2,

								"image":"d:/ymir work/ui/game/windows/cheque_icon.sub",
							},
							{
								"name" : "InputValue_Cheque",
								"type" : "editline",

								"x" : 3,
								"y" : 3,

								"width" : 24,
								"height" : 18,

								"input_limit" : 2,
								"only_number" : 1,
								
								"text" : "0",
							},
						),
					},
					
					## Input Slot Money
					{
						"name" : "InputSlot",
						"type" : "slotbar",

						"x" : 50+40,
						"y" : 34,
						"width" : 90,
						"height" : 18,
						#"horizontal_align" : "center",

						"children" :
						(
							{
								"name":"Money_Icon",
								"type":"image",

								"x":-18,
								"y":2,

								"image":"d:/ymir work/ui/game/windows/money_icon.sub",
							},
							{
								"name" : "InputValue",
								"type" : "editline",

								"x" : 3,
								"y" : 3,

								"width" : 90,
								"height" : 18,

								"input_limit" : 12,
								"only_number" : 1,
							},
						),
					},
					
					## Input Slot Money
					{
						"name" : "SellInfoText",
						"type" : "text",

						"x" : 0,
						"y" : BOARD_HEIGHT - 86,
						"text" : localeInfo.CHEQUE_SYSTEM_SELL_PRICE,
						"text_horizontal_align" : "center",
						"horizontal_align" : "center",
					},

					## Input Slot Money
					{
						"name" : "ChequeValue",
						"type" : "text",

						"x" : 0,
						"y" : BOARD_HEIGHT - 68,
						"text" : "99",
						"text_horizontal_align" : "center",
						"horizontal_align" : "center",
					},
					
					## Input Slot Money
					{
						"name" : "MoneyValue",
						"type" : "text",

						"x" : 0,
						"y" : BOARD_HEIGHT - 50,
						"text" : "999999999",
						"text_horizontal_align" : "center",
						"horizontal_align" : "center",
					},

					## Button
					{
						"name" : "AcceptButton",
						"type" : "button",

						"x" : - 61 - 5 + 30,
						"y" : BOARD_HEIGHT - 32,
						"horizontal_align" : "center",

						"text" : uiScriptLocale.OK,

						"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
						"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
						"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
					},
					{
						"name" : "CancelButton",
						"type" : "button",

						"x" : 5 + 30,
						"y" : BOARD_HEIGHT - 32,
						"horizontal_align" : "center",

						"text" : uiScriptLocale.CANCEL,

						"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
						"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
						"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
					},
				),
			},
		),
	}
else :
	window = {
		"name" : "InputDialog",

		"x" : 0,
		"y" : 0,

		"style" : ("movable", "float",),

		"width" : 200,
		"height" : 110,

		"children" :
		(
			{
				"name" : "board",
				"type" : "board_with_titlebar",

				"x" : 0,
				"y" : 0,

				"width" : 200,
				"height" : 110,

				"title" : "",

				"children" :
				(

					## Input Slot
					{
						"name" : "InputSlot",
						"type" : "slotbar",

						"x" : 0,
						"y" : 34,
						"width" : 90,
						"height" : 18,
						"horizontal_align" : "center",

						"children" :
						(
							{
								"name" : "InputValue",
								"type" : "editline",

								"x" : 3,
								"y" : 3,

								"width" : 90,
								"height" : 18,

								"input_limit" : 12,
							},
						),
					},

					## Input Slot
					{
						"name" : "MoneyValue",
						"type" : "text",

						"x" : 0,
						"y" : 59,
						"text" : "999999999",
						"text_horizontal_align" : "center",
						"horizontal_align" : "center",
					},

					## Button
					{
						"name" : "AcceptButton",
						"type" : "button",

						"x" : - 61 - 5 + 30,
						"y" : 78,
						"horizontal_align" : "center",

						"text" : uiScriptLocale.OK,

						"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
						"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
						"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
					},
					{
						"name" : "CancelButton",
						"type" : "button",

						"x" : 5 + 30,
						"y" : 78,
						"horizontal_align" : "center",

						"text" : uiScriptLocale.CANCEL,

						"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
						"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
						"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
					},
				),
			},
		),
	}