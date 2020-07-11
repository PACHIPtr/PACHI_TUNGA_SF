import uiScriptLocale
ROOT_PATH = "d:/ymir work/ui/public/"


window = {
	"name" : "BiyologDialog",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : 300,
	"height" : 160,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 300,
			"height" : 160,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 284,
					"color" : "gray",

					"children" :
					(
						{ "name":"titlename", "type":"text", "x":0, "y":3, 
						"text" : "Biyolog Ödül", 
						"horizontal_align":"center", "text_horizontal_align":"center" },
					),
				},
				
				{
					"name" : "bilgitext",
					"type" : "text",

					"x" : 65,
					"y" : 130,

					"text" : "Bonusu seçtikten sonra gönder tuþuna bas.",

				},
				
				{
					"name" : "odul1",
					"type" : "radio_button",

					"x" : 225,
					"y" : 40,

					"text" : "Seç ",

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "odul2",
					"type" : "radio_button",

					"x" : 225,
					"y" : 60,

					"text" : "Seç ",

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},	

				{
					"name" : "odul3",
					"type" : "radio_button",

					"x" : 225,
					"y" : 80,

					"text" : "Seç ",

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},					
				
				{
					"name" : "odulver",
					"type" : "button",

					"x" : 120,
					"y" : 107,

					"text" : "Gönder ",

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},	
				
				{
					"name" : "af1",
					"type" : "slotbar",

					"x" : -30,
					"y" : 40,
					"width" : 200,
					"height" : 18,
					"horizontal_align" : "center",


					"children" :
					(
						{
							"name" : "afv1",
							"type" : "text",

							"x" : 3,
							"y" : 3,

							"text" : "Affect 1 ",
						},
					),
				},
				
				{
					"name" : "af2",
					"type" : "slotbar",

					"x" : -30,
					"y" : 60,
					"width" : 200,
					"height" : 18,
					"horizontal_align" : "center",


					"children" :
					(
						{
							"name" : "afv2",
							"type" : "text",

							"x" : 3,
							"y" : 3,

							"text" : "Affect 2 ",
						},
					),
				},
				
				{
					"name" : "af3",
					"type" : "slotbar",

					"x" : -30,
					"y" : 80,
					"width" : 200,
					"height" : 18,
					"horizontal_align" : "center",


					"children" :
					(
						{
							"name" : "afv3",
							"type" : "text",

							"x" : 3,
							"y" : 3,

							"text" : "Affect 3 ",
						},
					),
				},
				
				
				
			),
		},
	),
}
