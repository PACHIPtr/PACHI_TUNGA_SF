import uiScriptLocale
#ChatBlock-MOD © musicinstructor, 2010
window = {
	"name" : "ChatBlockDialog",

	"x" : 100,
	"y" : 100,

	"style" : ("movable", "float",),

	"width" : 280,
	"height" : 90,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",

			"x" : 0,
			"y" : 0,

			"width" : 280,
			"height" : 90,
			"title" : "ChatBlock",

			"children" :
			(

				## Stunden
				{
					"name" : "hour_slot",
					"type" : "image",

					"x" : 20,
					"y" : 34,

					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",

					"children" :
					(
						{
							"name" : "hour_value",
							"type" : "editline",

							"x" : 3,
							"y" : 2,

							"width" : 40,
							"height" : 18,

							"input_limit" : 6,
							"only_number" : 1,

							"text" : "3",
						},
						{
							"name" : "h",
							"type" : "text",

							"x" : 63,
							"y" : 3,

							"text" : "h",
						},
					),
				},


				## Minuten
				{
					"name" : "min_slot",
					"type" : "image",

					"x" : 100,
					"y" : 34,

					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",

					"children" :
					(
						{
							"name" : "min_value",
							"type" : "editline",

							"x" : 3,
							"y" : 2,

							"width" : 40,
							"height" : 18,

							"input_limit" : 6,
							"only_number" : 1,

							"text" : "0",
						},
						{
							"name" : "m",
							"type" : "text",

							"x" : 63,
							"y" : 3,

							"text" : "m",
						},
					),
				},


				## Sekunden
				{
					"name" : "sec_slot",
					"type" : "image",

					"x" : 180,
					"y" : 34,

					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",

					"children" :
					(
						{
							"name" : "sec_value",
							"type" : "editline",

							"x" : 3,
							"y" : 2,

							"width" : 40,
							"height" : 18,

							"input_limit" : 6,
							"only_number" : 1,

							"text" : "0",
						},
						{
							"name" : "s",
							"type" : "text",

							"x" : 63,
							"y" : 3,

							"text" : "s",
						},
					),
				},

				## Button
				{
					"name" : "accept_button",
					"type" : "button",

					"x" : 280/2 - 61 - 5,
					"y" : 58,

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "cancel_button",
					"type" : "button",

					"x" : 280/2 + 5,
					"y" : 58,

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
			),
		},
	),
}