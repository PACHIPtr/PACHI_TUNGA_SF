import uiScriptLocale

ROOT = "d:/ymir work/ui/public/"

window = {
	"name" : "MiniGameDialog",
	"style" : ("float",),
	
	"x" : SCREEN_WIDTH - 136 - 200-12,
	"y" : 78,
	
	"width" : 200,
	"height" : 258,
	
	"children" :
	(
		{
			"name" : "board",
			"type" : "thinboard",

			"x" : 0,
			"y" : 0,

			"width" : 200,
			"height" : 45,
			
			"children" :
			(
				{
					"name" : "close_button",
					"type" : "button",
					
					"x" : 10,
					"y" : 10,
					
					"text" : uiScriptLocale.CLOSE,
					
					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
			),
		},		
	),	
}
