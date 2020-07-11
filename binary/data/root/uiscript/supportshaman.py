import uiScriptLocale

GOLD_COLOR	= 0xFFFEE3AE
WHITE_COLOR = 0xFFFFFFFF
ORANGE_COLOR = 0xFFF2A505

window = {
	"name" : "SYardimcisiDialog",
	
	"x" : 0,
	"y" : 0,
	
	"style" : ("movable", "float",),
	
	"width" : 290,
	"height" : 125,
	
	"children":
	(
		{
			"name" : "Board",
			"type" : "board_with_titlebar",
			"style" : ("attach",),
			
			"x" : 0,
			"y" : 0,
			
			"width" : 290,
			"height" : 125,
			
			"title" : "Þamanýn Yardýmcýsý",
			
			"children" :
			(	
				# Yazilar
				{
					"name" : "yazilar",
					"type" : "thinboard",

					"x" : 0,
					"y" : 32,
					"width" : 270,
					"height" : 82,
					"horizontal_align" : "center",

					"children" :
					(
						{ 
							"name" : "YardimciWindow",
							"type" : "button",
							
							"x" : -50,
							"y" : -180,
							
							"default_image" : "d:/ymir work/ui/game/buffi/arkacizgi.tga",
							"over_image" : "d:/ymir work/ui/game/buffi/arkacizgi.tga",
							"down_image" : "d:/ymir work/ui/game/buffi/arkacizgi.tga",

							"children" :
							(
								{
									"name":"DereceBaslik",
									"type":"text",
									
									"x":-40,
									"y":-11,
									
									"text": "Derece: ",
									"color":GOLD_COLOR,
									"all_align" : "center"
								},
								{
									"name":"YardimciDerece",
									"type":"text",
									
									"x":-05,
									"y":-11,
									
									"text": "",
									"color":WHITE_COLOR,
									"all_align" : "center"
								},
								{
									"name":"LevelBaslik",
									"type":"text",
									
									"x":55,
									"y":-11,
									
									"text": "Seviye: ",
									"color":GOLD_COLOR,
									"all_align" : "center"
								},
								{
									"name":"YardimciLevel",
									"type":"text",
									
									"x":77,
									"y":-11,
									
									"text": "",
									"color":WHITE_COLOR,
									"all_align" : "center"
								},
							),
						},
						{ 
							"name" : "YardimciArkaWindow",
							"type" : "button",
							
							"x" : -47,
							"y" : -155,
							
							"default_image" : "d:/ymir work/ui/game/buffi/arkaplan.tga",
							"over_image" : "d:/ymir work/ui/game/buffi/arkaplan.tga",
							"down_image" : "d:/ymir work/ui/game/buffi/arkaplan.tga",

							"children" :
							(
								{
									"name" : "YuzukIcon",
									"type" : "slot",
									
									"x" : 56,
									"y" : 190,
									
									"width" : 32,
									"height" : 32,
									
									"slot" : ({"index":0, "x":0, "y":0, "width":32, "height":32},),
								},
								{
									"name":"ZekaOraniBaslik",
									"type":"text",
									
									"x": -47,
									"y" : 0,
									
									"text": "Zeka Oraný: ",
									"color":GOLD_COLOR,
									"all_align" : "center"
								},
								{ 
									"name" : "ZekaOraniWindow",
									"type" : "button",
									
									"x" : 158,
									"y" : 200,
									
									"default_image" : "d:/ymir work/ui/public/Parameter_Slot_01.sub",
									"over_image" : "d:/ymir work/ui/public/Parameter_Slot_01.sub",
									"down_image" : "d:/ymir work/ui/public/Parameter_Slot_01.sub",

									"children" :
									(
										{
											"name":"ZekaOrani",
											"type":"text",

											"x":0,
											"y":0,

											"text": "",

											"color":WHITE_COLOR,
											"all_align" : "center"
										},
									),
								},
								{
									"name" : "YardimciTecrube_Arkaplan",
									"type" : "window",
									"style": ("ltr",),

									"x" : 225,
									"y" : 210,
									
									"width"		: 16 * 5 + 2 * 4,
									"height"	: 16,

									"children" :
									(
										{
											"name" : "YardimciTecrube_Baslik",
											"type" : "button",

											"x" : -138,
											"y" : -218,

											"default_image" : "d:/ymir work/ui/game/buffi/arkabuton.tga",
											"over_image" : "d:/ymir work/ui/game/buffi/arkabuton.tga",
											"down_image" : "d:/ymir work/ui/game/buffi/arkabuton.tga",
											"children" :
											(
												{
													"name":"Tecrube_Baslik",
													"type":"text",
													
													"x":0,
													"y":-2,
													
													"text": "Deneyim",
													"color":WHITE_COLOR,
													"all_align" : "center"
												},
											),
										},
										{
											"name" : "YardimciTecrube_Arkaplan",
											"type" : "button",

											"x" : -233,
											"y" : -181,

											"default_image" : "d:/ymir work/ui/game/buffi/tecrube.tga",
											"over_image" : "d:/ymir work/ui/game/buffi/tecrube.tga",
											"down_image" : "d:/ymir work/ui/game/buffi/tecrube.tga",
										},
										{
											"name" : "YardimciTecrube_01",
											"type" : "expanded_image",

											"x" : 0,
											"y" : 0,

											"image" : "d:/ymir work/ui/game/buffi/exp_gauge/exp_on.sub",
										},
										{
											"name" : "YardimciTecrube_02",
											"type" : "expanded_image",

											"x" : 16 + 2 + 1,
											"y" : 0,

											"image" : "d:/ymir work/ui/game/buffi/exp_gauge/exp_on.sub",
										},
										{
											"name" : "YardimciTecrube_03",
											"type" : "expanded_image",

											"x" : 16 * 2 + 2 * 2 + 3,
											"y" : 0,

											"image" : "d:/ymir work/ui/game/buffi/exp_gauge/exp_on.sub",
										},
										{
											"name" : "YardimciTecrube_04",
											"type" : "expanded_image",

											"x" : 16 * 3 + 2 * 3 + 4,
											"y" : 0,

											"image" : "d:/ymir work/ui/game/buffi/exp_gauge/exp_on.sub",
										},
									),
								}, 
							),
						},
					),
				},
			),
		},
	),
}
