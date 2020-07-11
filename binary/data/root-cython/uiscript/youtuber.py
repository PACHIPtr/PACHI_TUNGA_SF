import uiScriptLocale
import translate

window = {
	"name" : "NewPetWinow",
	"style" : ("movable", "float",),
	
	"x" : SCREEN_WIDTH - 200,
	"y" : SCREEN_HEIGHT - 595,

	"width" : 300,
	"height" : 215,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 300,
			"height" : 215,

			"children" :
			(
				{
					"name" : "TitleYoutube",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 7,

					"width" : 288,
					"color" : "gray",
					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":0, "text":"Youtube Link", "all_align":"center" },
					),
				},
				{
					"name" : "title2",
					"type" : "text",

					"x" : 25,
					"y" : 36,

					"text" : "Video Baþlýðý : ",
				},
				{
					"name" : "InputSlot",
					"type" : "slotbar",

					"x" : -1,
					"y" : 56,

					"width" : 235,
					"height" : 25,

					"horizontal_align" : "center",

					"children" :
					(

						{
							"name" : "InputValue",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 235,
							"height" : 25,

							"input_limit" : 48,
						},
					),
				},
				{
					"name" : "title3",
					"type" : "text",

					"x" : 25,
					"y" : 91,

					"text" : "Not: Video sonundaki video id giriniz :",
				},
				{
					"name" : "title4",
					"type" : "text",

					"x" : 25,
					"y" : 106,

					"text" : uiScriptLocale.YOUTUBER_ORNEK,
				},
				{
					"name" : "InputSlot2",
					"type" : "slotbar",

					"x" : -1,
					"y" : 126,

					"width" : 235,
					"height" : 25,

					"horizontal_align" : "center",

					"children" :
					(

						{
							"name" : "InputValue2",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 235,
							"height" : 25,
							"input_limit" : 32,
						},
					),
				},
				{
					"name" : "Favori",
					"type" : "toggle_button",

					"x" : 70,
					"y" : 158,

					"default_image" : "d:/ymir work/ui/game/checkbox/new_checkbox_default.sub",
					"over_image" : "d:/ymir work/ui/game/checkbox/new_checkbox_over.sub",
					"down_image" : "d:/ymir work/ui/game/checkbox/new_checkbox_down.sub",

					"children" :
					(
						{
							"name" : "FavoriText",
							"type" : "text",

							"x" : 20,
							"y" : 0,

							"text" : "Favori videolar listesine ekle.",
						},
					),
				},
				{
					"name" : "viewButton",
					"type" : "button",

					"x" : 59,
					"y" : 183,

					"text" : "Önizleme",

					"default_image" : "d:/ymir work/ui/dragonsoul/m_button01.tga",
					"over_image" : "d:/ymir work/ui/dragonsoul/m_button02.tga",
					"down_image" : "d:/ymir work/ui/dragonsoul/m_button03.tga",
				},
				{
					"name" : "okButton",
					"type" : "button",

					"x" : 127,
					"y" : 183,

					"text" : "Oluþtur",
					"text_color" : 0xffF8BF24,

					"default_image" : "d:/ymir work/ui/dragonsoul/m_button01.tga",
					"over_image" : "d:/ymir work/ui/dragonsoul/m_button02.tga",
					"down_image" : "d:/ymir work/ui/dragonsoul/m_button03.tga",
				},
				{
					"name" : "FavList",
					"type" : "button",

					"x" : 195,
					"y" : 183,

					"text" : "Fav.Listem",

					"default_image" : "d:/ymir work/ui/dragonsoul/m_button01.tga",
					"over_image" : "d:/ymir work/ui/dragonsoul/m_button02.tga",
					"down_image" : "d:/ymir work/ui/dragonsoul/m_button03.tga",
				},
				{
					"name" : "button3",
					"type" : "button",

					"x" : 276,
					"y" : 10,

					"tooltip_text" : uiScriptLocale.CLOSE, 

					"default_image" : "d:/ymir work/ui/public/close_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/close_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/close_button_03.sub",
				},
			),
		},
	),
}
