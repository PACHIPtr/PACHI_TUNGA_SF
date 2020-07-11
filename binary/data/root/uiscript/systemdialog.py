import uiScriptLocale

ROOT = "d:/ymir work/ui/public/"

window = {
	"name" : "SystemDialog",
	"style" : ("float",),

	"x" : (SCREEN_WIDTH  - 398) /2,
	"y" : (SCREEN_HEIGHT - 291) /2,

	"width" : 398,
	"height" : 291,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 398,
			"height" : 291,

			"children" :
			(
				{
					"name" : "thinboard_circle",
					"type" : "thinboard_circle",
					"x" : 5,
					"y" : 5,
					"width" : 190,
					"height" : 278,
					"children" :
					(
						{
							"name" : "image",
							"type" : "image",
							"x" : 2,
							"y" : 15,
							"image" : "d:/ymir work/ui/game/arka.tga",
							"children" :
							(
								{
									"name" : "tt",
									"type" : "text",
									"text" : " Oyun Ayarlari",
									"x" : 61,
									"y" : 4,
								},
							),
						},
						{
							"name" : "image",
							"type" : "image",
							"x" : 2,
							"y" : 127,
							"image" : "d:/ymir work/ui/game/arka.tga",
							"children" :
							(
								{
									"name" : "tt",
									"type" : "text",
									"text" : " Hesap Ayarlari",
									"x" : 61,
									"y" : 4,
								},
							),
						},
					),
				},
				{
					"name" : "thinboard_circle",
					"type" : "thinboard_circle",
					"x" : 199,
					"y" : 5,
					"width" : 190,
					"height" : 278,
					"children" :
					(
						{
							"name" : "image",
							"type" : "image",
							"x" : 2,
							"y" : 15,
							"image" : "d:/ymir work/ui/game/arka.tga",
							"children" :
							(
								{
									"name" : "tt",
									"type" : "text",
									"text" : " Duyuru Panosu",
									"x" : 59,
									"y" : 4,
								},
							),
						},
					),
				},
				{
					"name" : "thinboard_circle",
					"type" : "thinboard_circle",
					"x" : 199+5,
					"y" : 45,
					"width" : 165+20,
					"height" : 185,
					"children" :
					(
						{
							"name" : "text",
							"type" : "text",
							"text" : "Merhaba, Arven2'ye hoþgeldiniz.",
							"x" : 10,
							"y" : 7,
						},
						{
							"name" : "text",
							"type" : "text",
							"text" : "Kitaplarý, taþlar ve yükseltme",
							"x" : 10,
							"y" : 27,
						},
						{
							"name" : "text",
							"type" : "text",
							"text" : "eþyalarýný görmek için K tuþuna bas.",
							"x" : 10,
							"y" : 42,
						},
						{
							"name" : "text",
							"type" : "text",
							"text" : "Her 10 seviyede Çýrak",
							"x" : 10,
							"y" : 62,
						},
						{
							"name" : "text",
							"type" : "text",
							"text" : "Sandýðýný açmayý unutma.",
							"x" : 10,
							"y" : 77,
						},
						{
							"name" : "text",
							"type" : "text",
							"text" : "Arven Masasýný ziyaret et.",
							"x" : 10,
							"y" : 97,
						},
						{
							"name" : "text",
							"type" : "text",
							"text" : "Aklýna takýlan sorular için",
							"x" : 10,
							"y" : 117,
						},
						{
							"name" : "text",
							"type" : "text",
							"text" : "destek sistemimizi ziyaret et.",
							"x" : 10,
							"y" : 132,
						},
						{
							"name" : "text",
							"type" : "text",
							"text" : "Arven2 Ekibi sizi seviyor <3",
							"x" : 10,
							"y" : 157,
						},
						{
							"name" : "text",
							"type" : "text",
							#"text" : "Facebook : fb.com/Metin2BoardFan",
							"x" : 5,
							"y" : 75,
						},
					),	
				},
				{
					"name" : "movechannel_button",
					"type" : "button",

					"x" : 10,
					"y" : 217,

					"text" : uiScriptLocale.SYSTEM_MOVE_CHANNEL,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "system_option_button",
					"type" : "button",

					"x" : 10,
					"y" : 45,

					"text" : uiScriptLocale.SYSTEMOPTION_TITLE,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "game_option_button",
					"type" : "button",

					"x" : 10,
					"y" : 75,

					"text" : uiScriptLocale.GAMEOPTION_TITLE,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "change_button",
					"type" : "button",

					"x" : 10,
					"y" : 105,

					"text" : uiScriptLocale.SYSTEM_CHANGE,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "logout_button",
					"type" : "button",

					"x" : 10,
					"y" : 217-60,

					"text" : uiScriptLocale.SYSTEM_LOGOUT,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "exit_button",
					"type" : "button",

					"x" : 10,
					"y" : 217-30,

					"text" : uiScriptLocale.SYSTEM_EXIT,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "cancel_button",
					"type" : "button",

					"x" : 10,
					"y" : 247,

					"text" : uiScriptLocale.CANCEL,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "mall_button",
					"type" : "button",
					"x" : 204+3,
					"y" : 247,
					"text" : uiScriptLocale.SYSTEM_MALL,
					"text_color" : 0xffF8BF24,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "help_button",
					"type" : "button",
					"x" : 0,
					"y" : 0,
					#"text" : uiScriptLocale.SYSTEM_HELP,

					#"default_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_01.sub",
					#"over_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_02.sub",
					#"down_image" : "d:/ymir work/ui/game/myshop_deco/select_btn_03.sub",
				},
			),
		},
	),
}