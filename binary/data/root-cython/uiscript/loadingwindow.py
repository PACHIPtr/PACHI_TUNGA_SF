import uiScriptLocale

window = {
	"name" : "LoginWindow",
	"sytle" : ("movable",),
	"x" : 0,
	"y" : 0,
	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,
	"children" :
	(
		{
			"name" : "BackGround",
			"type" : "expanded_image",
			"x" : 0,
			"y" : 0,
			"image" : "d:/ymir work/ui/intro/pattern/Line_Pattern.tga",

			"x_scale" : float(SCREEN_WIDTH) / 800.0,
			"y_scale" : float(SCREEN_HEIGHT) / 600.0,
		},
		{
			"name" : "TipBackground",
			"type" : "thinboard",
			
			"x"	: 0,
			"y" : float(SCREEN_HEIGHT) * 500 / 600.0 - 100,
			
			"width" : SCREEN_WIDTH,
			"height" : 100,
			
			"children" :
			(
				{
					"name" : "LoadingTip",
					"type" : "text",
					
					"x"	: float(SCREEN_WIDTH) / 2,
					"y" : 40,				
					
					"fontsize" : "LARGE",
					"text_horizontal_align" : "center",
				},
			),
		},
		{ 
			"name":"ErrorMessage", 
			"type":"text", "x":10, "y":10, 
			"text": uiScriptLocale.LOAD_ERROR, 
		},
		{
			"name": "GageBoard",
			"type": "window",
			#"x": float(SCREEN_WIDTH) * 400 / 800.0 - 200,
			"x": float(SCREEN_WIDTH) * 400 / 800.0 - 420,
			"y": float(SCREEN_HEIGHT) * 500 / 600.0,
			"width": 400,
			"height": 80,

			"children":
				(
					{
						"name": "BackGage",
						"type": "expanded_image",

						"x": 70,
						"y": 25,

						"image": "d:/ymir work/uiloading/gauge_empty.dds",
					},
					{
						"name": "FullGage",
						"type": "expanded_image",

						"x": 70,
						"y": 25,

						"image": "d:/ymir work/uiloading/gauge_full.dds",
					},
				),
		},
		{
			"name": "logo",
			"type": "window",
			"x": (SCREEN_WIDTH - 465) / 2,
			"y": (SCREEN_HEIGHT - 475) / 2,
			"width": 0,
			"height": 0,
			"children": 
			(
				{	
					"name": "logo",
					"type": "image",
					"x": 0,
					"y": 0,
					"image": "d:/ymir work/ui/intro/login/login_logo.tga",
				},
			),
		},
	),
}
