import uiScriptLocale

window = {
	"name" : "KasmirMarketiDialog",

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

			"title" : uiScriptLocale.KASMIR_MARKET_BASLIK,

			"children":
			(
				{
					"name" : "KamirMarketModel",
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
							"name" : "Npc_Title",
							"type" : "text",

							"x" : 80,
							"y" : 7,

							"text" : "DarkMMO",
						},
						{
							"name" : "market_view",
							"type" : "expanded_image",

							"x" : 3,
							"y" : 25,

							"image" : "d:/ymir work/ui/game/myshop_deco/model_view_bg.sub",
						},
					),
				},
				{
					"name" : "KamirMarketName",
					"type" : "thinboard_circle",

					"x" : 15,
					"y" : 35,

					"width" : 170,
					"height" : 265,

					"children" :
					(
						{
							"name" : "privateshop_0",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 5,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_MARKET_PRIVATESHOP_0,
						},
						{
							"name" : "privateshop_1",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 30,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_MARKET_PRIVATESHOP_1,
						},
						{
							"name" : "privateshop_2",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 55,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_MARKET_PRIVATESHOP_2,
						},
						{
							"name" : "privateshop_3",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 80,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_MARKET_PRIVATESHOP_3,
						},
						{
							"name" : "privateshop_4",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 105,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_MARKET_PRIVATESHOP_4,
						},
						{
							"name" : "privateshop_5",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 130,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_MARKET_PRIVATESHOP_5,
						},
						{
							"name" : "privateshop_6",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 155,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_MARKET_PRIVATESHOP_6,
						},
						{
							"name" : "privateshop_7",
							"type" : "radio_button",
							
							"x" : 10,
							"y" : 180,
							
							"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
							"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
							"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",

							"text" : uiScriptLocale.KASMIR_MARKET_PRIVATESHOP_7,
						},
					),
				},
				{
					"name" : "DevamButton",
					"type" : "button",

					"x" : 195,
					"y" : 275,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",

					"text" : "Devam",
				},
				{
					"name" : "CancelButton",
					"type" : "button",

					"x" : 295,
					"y" : 275,

					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",

					"text" : "Kapat",
				},
			),
		},
	),
}