import uiScriptLocale
BOARD_X = 416+7+10+1
BOARD_Y = 533+2+2


window = {
	"name" : "BossTrackingInfoWindow",
	
	"style" : ("movable", "float", ),
	
	
	"x"		: 0,
	"y"		: 0,
	
	"width"	: BOARD_X,
	"height" : BOARD_Y,
	
	"children" : 
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",
			"style" : ("attach", ),
			
			"x" : 0,
			"y" : 0,
			
			"width" : BOARD_X,
			"height" : BOARD_Y,
			
			"title" : uiScriptLocale.BOSS_TRACKING_INFO_WINDOW_TITLE,
			
			"children" : 
			(
				{
					"name" : "Background",
					"type" : "expanded_image",
					
					"x" : 9,
					"y" : 29,
					
					"image" : "d:/ymir work/ui/boss_follow/base_boss_follow_gui.tga",
					
					"children" : 
					(
						{
							"name" : "AvatarImage",
							"type" : "image",
							
							"x" : 165,
							"y" : 46+1,
							
							"image" : "d:/ymir work/ui/boss_follow/1192.tga",
						},
						
						{
							"name" : "Info1",
							"type" : "text",
							
							"x" : 100,
							"y" : 139+1,
							
							"text" : "",
						},
						
						#{
						#	"name" : "Info2",
						#	"type" : "text",
						#	
						#	"x" : 115+210-6-13,
						#	"y" : 155+60-12-8,
						#	
						#	"text" : "",
						#},
						
						{
							"name" : "CH1RespawnInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 307+1,
							
							"text" : "",
						},
						
						{
							"name" : "CH2RespawnInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 329+1,
							
							"text" : "",
						},
						
						{
							"name" : "CH3RespawnInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 372+1,
							
							"text" : "",							
						},
						
						{
							"name" : "CH4RespawnInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 394+1,
							
							"text" : "",							
						},

						{
							"name" : "CH5RespawnInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 437+1,
							
							"text" : "",							
						},				

						{
							"name" : "CH6RespawnInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 459+1,
							
							"text" : "",							
						},
						
						{
							"name" : "ChannelInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 263+1+1,
							
							"text" :"",							
						},					
						
						{
							"name" : "ElementInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 241+1+1,
							
							"text" : "",
						},
						
						{
							"name" : "MapInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 198+1,
							
							"text" : "",							
						},	
						
						{
							"name" : "MobLevelInfo",
							"type" : "text",
							
							"x" : 105,
							"y" : 176+1,
							
							"text" : "",							
						},
					),
				},
			
				{
					"name" : "ToolTipButton",
					"type" : "button",

					"x" : BOARD_X-45,
					"y" : 8,

					"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
					"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
					"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
				},			
			),
		},
	),
}