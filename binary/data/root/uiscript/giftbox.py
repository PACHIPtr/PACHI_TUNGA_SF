import uiScriptLocale

ROOT = "d:/ymir work/ui/game/"

Y_ADD_POSITION = 0
window = {
	"name" : "GiftBox",
	
	"x" : 0,
	"y" : SCREEN_HEIGHT - 45,
	
	"width" : 50,
	"height" : 50,
	
	"children" :
	(
		{
			"name" : "GiftBox_Icon",
			"type" : "image",
			
			"x" : 5,
			"y" : -90 + Y_ADD_POSITION,
			
			"width" : 50,
			"height" : 50,
	
			#"image" : "Jin_Han/giftbox.tga",
			
		},
		{
			"name" : "GiftBox_ToolTip",
			
			"x" : 0,
			"y" : 0,
			
			"width" : 50,
			"height" : 10,
			"type" : "window",
		},
	),	
}