import uiScriptLocale

window = {
	"name" : "KasmirPaketiDialog",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : 400,
	"height" : 315,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 400,
			"height" : 315,

			"title" : uiScriptLocale.KASMIR_PAKET_BASLIK,

			"children":
			(
				{
					"name" : "KamirPaketModel",
					"type" : "thinboard_circle",

					"x" : 190,
					"y" : 35,

					"width" : 196,
					"height" : 265,

					"children" :
					(
						{
							"name" : "model_view_title",
							"type" : "expanded_image",

							"x" : 3,
							"y" : 3,

							"image" : "d:/ymir work/ui/game/myshop_deco/model_view_title.sub",
						},
						{
							"name" : "Baslik_Title",
							"type" : "text",

							"x" : 80,
							"y" : 7,

							"text" : "DarkMMO",
						},
						{
							"name" : "baslik_view",
							"type" : "expanded_image",

							"x" : 3,
							"y" : 25,

							"image" : "d:/ymir work/ui/game/myshop_deco/model_view_bg.sub",
						},
					),
				},
				{
					"name" : "KamirPaketName",
					"type" : "thinboard_circle",

					"x" : 15,
					"y" : 35,

					"width" : 170,
					"height" : 265,

					"children" :
					(
						{
							"name" : "Normal",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 5,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_PAKET_STANDART,
						},
						{
							"name" : "Fire",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 30,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_PAKET_FIRE,
						},
						{
							"name" : "Ice",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 55,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_PAKET_ICE,
						},
						{
							"name" : "Paper",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 80,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_PAKET_PAPER,
						},
						{
							"name" : "Ribon",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 105,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_PAKET_RIBON,
						},
						{
							"name" : "Wing",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 130,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_PAKET_WING,
						},
					),
				},
				{
					"name" : "GeriButton",
					"type" : "button",

					"x" : 195,
					"y" : 275,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",

					"text" : "Geri",
				},
				{
					"name" : "AgreeButton",
					"type" : "button",

					"x" : 295,
					"y" : 275,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",

					"text" : "Bitir",
				},
			),
		},
	),
}