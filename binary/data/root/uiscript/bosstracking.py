import uiScriptLocale
BACK_IMG_PATH = "d:/ymir work/ui/pattern/"
ROOT_PATH = "d:/ymir work/ui/game/myshop_deco/"
TWISS_PATH = "d:/ymir work/ui/"
LOCALE_PATH = "d:/ymir work/ui/privatesearch/"

window = {
	"name" : "BossTrackingSystemWindow",
	"x" : (SCREEN_WIDTH -518) / 2,
	"y" : (SCREEN_HEIGHT - 380) / 2,
	"style" : ("movable","float",),
	"width" : 618,
	"height" : 380,
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 618,
			"height" : 380,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 7,

					"width" : 603,
					"color" : "yellow",
					
					"children" :
					(
						{ "name":"title_name_old", "type":"text", "x":0, "y":-1, "text": uiScriptLocale.BOSS_TRACKING_TITLE, "all_align":"center" },
					),
				},

				## menu_list bg
				{
					"name" : "menu_list",
					"type" : "new_board",

					"x" : 14,
					"y" : 36,

					"width" : 169,
					"height" : 326,

					"children" :
					(
						{ "name" : "btnMenuCadi", "type" : "button", "x" : 10, "y" : 15, "text"	:	"Güçlü Buz Cadisi","default_image" : ROOT_PATH + "select_btn_01.sub", "over_image" : ROOT_PATH + "select_btn_02.sub", "down_image" : ROOT_PATH + "select_btn_03.sub", },
						{ "name" : "btnMenuAlevKral", "type" : "button", "x" : 10, "y" : 45, "text"	:	"Alev Kral","default_image" : ROOT_PATH + "select_btn_01.sub", "over_image" : ROOT_PATH + "select_btn_02.sub", "down_image" : ROOT_PATH + "select_btn_03.sub", },
						{ "name" : "btnMenuKOrumcek", "type" : "button", "x" : 10, "y" : 75, "text"	:	"Kralice Örümcek","default_image" : ROOT_PATH + "select_btn_01.sub", "over_image" : ROOT_PATH + "select_btn_02.sub", "down_image" : ROOT_PATH + "select_btn_03.sub", },
						{ "name" : "btnMenuKEjder", "type" : "button", "x" : 10, "y" : 105, "text"	:	"Sari Kaplan Hayaleti","default_image" : ROOT_PATH + "select_btn_01.sub", "over_image" : ROOT_PATH + "select_btn_02.sub", "down_image" : ROOT_PATH + "select_btn_03.sub", },
						{ "name" : "btnMenuDokuzKuyruk", "type" : "button", "x" : 10, "y" : 135, "text"	:	"Dokuz Kuyruk","default_image" : ROOT_PATH + "select_btn_01.sub", "over_image" : ROOT_PATH + "select_btn_02.sub", "down_image" : ROOT_PATH + "select_btn_03.sub", },
						{ "name" : "btnMenuOrumcekBarones", "type" : "button", "x" : 10, "y" : 165, "text"	:	"Dev Çöl Kaplumbagasi","default_image" : ROOT_PATH + "select_btn_01.sub", "over_image" : ROOT_PATH + "select_btn_02.sub", "down_image" : ROOT_PATH + "select_btn_03.sub", },

					),
				},

				{
					"name" : "bosstime_window",
					"type" : "new_board",

					"x" : 191,
					"y" : 35,

					"width" : 312+100,
					"height" : 325,
					
					"children" :
					(
						# {
							# "name" : "Model_Background",
							# "type" : "image",
							# "image" : TWISS_PATH+"model_back.tga",
							# "x" : 10, 
							# "y" : 40,
						# },
						{
							"name" : "map_name",
							"type" : "image",
							"image" :LOCALE_PATH+"private_leftNameImg.sub",
							"x" : 228, 
							"y" : 60,
							"children" :
							(
								{
									"name" : "mapnametext",
									"type" : "text",
									"x" :60,
									"y" : 4,
									"text" : "Bulundugu Harita",
									"text_horizontal_align":"center",
								},
							),
						},
						{
							"name" : "MapNameWindow",
							"type" : "image",
							"x" : 230,
							"y" : 80,
							"image" : LOCALE_PATH+"private_leftSlotImg.sub",
							"children":
							(
								{ "name" : "MapNameSlot", "type" : "text", "x" : 60, "y" : 2, "text_horizontal_align":"center","text" : "Sürgün Magrasi", "color":0xFFFEE3AE, },
							),
						},
						{
							"name" : "sub_titlebar",
							"type" : "image",

							"x" : 0,
							"y" : 260,

							"image" : TWISS_PATH+"tt_bar.tga",
							"children":
							(
								{
									"name" : "durumgui",
									"type" : "text",
									"x" : 170,
									"y" : 2, 
									"text" : "Boss un  Durumu",
								},
							),	
						},
						{
							"name" : "CH1Time_slotbar",
							"type" : "slotbar",
							"x" : 10,
							"y" : 295,
							"width" : 70,
							"height" : 18,
							"children" :
							(
								{
									"name" : "CH1Time",
									"type" : "text",
									"x" : 5,
									"y" : 3,
									"text" : "CH1: 0 Saat 0 Dakika 0 Saniye",
								},
							),
						},
						{
							"name" : "CH2Time_slotbar",
							"type" : "slotbar",
							"x" : 85,
							"y" : 295,
							"width" : 140/2,
							"height" : 18,
							"children" :
							(
								{
									"name" : "CH2Time",
									"type" : "text",
									"x" : 5,
									"y" : 3,
									"text" : "CH2: 0 Saat 0 Dakika 0 Saniye",
								},
							),
						},
						{
							"name" : "CH3Time_slotbar",
							"type" : "slotbar",
							"x" : 85+75,
							"y" : 295,
							"width" : 140/2,
							"height" : 18,
							"children" :
							(
								{
									"name" : "CH3Time",
									"type" : "text",
									"x" : 5,
									"y" : 3,
									"text" : "CH3: 0 Saat 0 Dakika 0 Saniye",
								},
							),
						},
						{
							"name" : "CH4Time_slotbar",
							"type" : "slotbar",
							"x" : 85+150,
							"y" : 295,
							"width" : 70,
							"height" : 18,
							"children" :
							(
								{
									"name" : "CH4Time",
									"type" : "text",
									"x" : 5,
									"y" : 3,
									"text" : "CH4: 0 Saat 0 Dakika 0 Saniye",
								},
							),
						},
						
					),	
				},

				{
					"name" : "list",
					"type" : "window",

					"x" : 191,
					"y" : 35,

					"width" : 306,
					"height" : 21,

					"children" :
					(
						{
							"name" : "sub_titlebar",
							"type" : "image",

							"x" : 0,
							"y" : 0,

							"image" : TWISS_PATH+"tt_bar.tga",
						},
						{ "name" : "title_name", "type" : "text", "x" : 15, "y" : 2, "text" : "Seçilen Menü : Güçlü Buz Cadýsý", "color":0xFFFEE3AE, },
						{ "name" : "clock", "type" : "text", "x" : 246, "y" : 2, "text" : "", "color":0xFFFEE3AE, },
					),
				},
			),
		},
	),
}