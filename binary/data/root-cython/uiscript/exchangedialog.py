import uiScriptLocale

ROOT = "d:/ymir work/ui/game/"

window = {
	"name" : "ExchangeDialog",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : 346,
	"height" : 220,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 346,
			"height" : 220,

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,
 
					"width" : 330,
					"color" : "gray",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":173, "y":3, "text":uiScriptLocale.EXCHANGE_TITLE, "text_horizontal_align":"center" },
					),
				},

				## MiddleBar
				{
					"name" : "Middle_Bar",
					"type" : "image",

					"x" : 171,
					"y" : 31,

					"image" : ROOT + "windows/middlebar.sub",
				},
				
				{
					"name" : "Middle_Bar",
					"type" : "image",

					"x" : 171,
					"y" : 79,

					"image" : ROOT + "windows/middlebar.sub",
				},
				
				## Owner
				{
					"name" : "Owner",
					"type" : "window",

					"x" : 176,
					"y" : 31,

					"width" : 150,
					"height" : 210,

					"children" :
					(
						{
							"name" : "Owner_Slot",
							"type" : "grid_table",

							"start_index" : 0,

							"x" : 0,
							"y" : 0,

							"x_count" : 5,
							"y_count" : 4,
							"x_step" : 32,
							"y_step" : 32,
							"x_blank" : 0,
							"y_blank" : 0,

							"image" : "d:/ymir work/ui/public/slot_base.sub",
						},
						{
							"name" : "Owner_Money",
							"type" : "button",

							"x" : 0,
							"y" : 134,

							#"image" : "d:/ymir work/ui/public/parameter_slot_02.sub",

							"default_image" : "d:/ymir work/ui/public/parameter_slot_03.sub",
							"over_image" : "d:/ymir work/ui/public/parameter_slot_03.sub",
							"down_image" : "d:/ymir work/ui/public/parameter_slot_03.sub",

							"children" :
							(
								{
									"name" : "Owner_Money_Value",
									"type" : "text",

									"x" : 85,
									"y" : 2,

									"text" : "1234567",

									"text_horizontal_align" : "right",
								},
							),
						},
						{
							"name" : "Owner_Accept_Light",
							"type" : "button",

							"x" : 0,
							"y" : 0,

							"default_image" : "d:/ymir work/ui/greenbox.png",
							"over_image" : "d:/ymir work/ui/greenbox.png",
							"down_image" : "d:/ymir work/ui/greenbox.png",
						},
						{
							"name":"Owner_Money_Icon",
							"type":"image",
							"x":92,
							"y":135,
							"image":"d:/ymir work/ui/game/windows/money_icon.sub",
						},
						{
							"name" : "Owner_Accept_Button",
							"type" : "toggle_button",

							"x" : 117,
							"y" : 157,

							"text" : uiScriptLocale.EXCHANGE_ACCEPT,

							"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
							"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
							"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
						},
						{
							"name" : "Owner_Cheque",
							"type" : "button",

							"x" : 0,
							"y" : 157,

							#"image" : "d:/ymir work/ui/public/parameter_slot_02.sub",

							"default_image" : "d:/ymir work/ui/public/parameter_slot_03.sub",
							"over_image" : "d:/ymir work/ui/public/parameter_slot_03.sub",
							"down_image" : "d:/ymir work/ui/public/parameter_slot_03.sub",

							"children" :
							(
								{
									"name" : "Owner_Cheque_Value",
									"type" : "text",

									"x" : 85,
									"y" : 2,

									"text" : "1234567",

									"text_horizontal_align" : "right",
								},
							),
						},
						{
							"name":"Owner_Cheque_Icon",
							"type":"image",
							"x":92,
							"y":158,
							"image":"d:/ymir work/ui/game/windows/cheque_icon.sub",
						},
					),
				},

				## Target
				{
					"name" : "Target",
					"type" : "window",

					"x" : 10,
					"y" : 33,

					"width" : 150,
					"height" : 210,

					"children" :
					(
						{
							"name" : "Target_Slot",
							"type" : "grid_table",

							"start_index" : 0,

							"x" : 0,
							"y" : 0,

							"x_count" : 5,
							"y_count" : 4,
							"x_step" : 32,
							"y_step" : 32,
							"x_blank" : 0,
							"y_blank" : 0,

							"image" : "d:/ymir work/ui/public/slot_base.sub",
						},
						{
							"name" : "Target_Money",
							"type" : "image",

							"x" : 0,
							"y" : 134,

							"image" : "d:/ymir work/ui/public/parameter_slot_04.sub",

							"children" :
							(
								{
									"name" : "Target_Money_Value",
									"type" : "text",

									"x" : 112,
									"y" : 2,

									"text" : "1234567",

									"text_horizontal_align" : "right",
								},
							),
						},
						{
							"name" : "Target_Accept_Light",
							"type" : "button",

							"x" : 0,
							"y" : 0,

							"default_image" : "d:/ymir work/ui/greenbox.png",
							"over_image" : "d:/ymir work/ui/greenbox.png",
							"down_image" : "d:/ymir work/ui/greenbox.png",
						},
						{
							"name":"Target_Money_Icon",
							"type":"image",
							"x":122,
							"y":135,
							"image":"d:/ymir work/ui/game/windows/money_icon.sub",
						},
						{
							"name" : "Target_Cheque",
							"type" : "image",

							"x" : 0,
							"y" : 157,

							"image" : "d:/ymir work/ui/public/parameter_slot_04.sub",

							"children" :
							(
								{
									"name" : "Target_Cheque_Value",
									"type" : "text",

									"x" : 112,
									"y" : 2,

									"text" : "1234567",

									"text_horizontal_align" : "right",
								},
							),
						},
						{
							"name":"Target_Cheque_Icon",
							"type":"image",
							"x":122,
							"y":158,
							"image":"d:/ymir work/ui/game/windows/cheque_icon.sub",
						},
					),
				},
				
				{
					"name" : "InformationButton",
					"type" : "button",

					"x" : 296,
					"y" : 8,

					"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
					"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
					"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
				},	
			),
		},
	),
}