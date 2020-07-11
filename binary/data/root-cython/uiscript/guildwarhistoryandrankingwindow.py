
window = {
	"name" : "GuildWarHistoryAndRankingWindow",
	
	"style"		: ("movable", "float", ),
	
	"x" 		: 0,
	"y" 		: 0,
	"width" 	: 835-100-63,
	"height" 	: 294,
	
	"children" : 
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",
			"style" : ("attach", ),
			
			"x" : 0,
			"y" : 0,
			
			"width" : 835-100-63,
			"height" : 294,
			
			"title" : "GuildWarHistoryAndRankingWindow",
			
			"children" : 
			(			
				{
					"name"		: "sub_board",
					"type"		: "thinboard_circle",
					
					"x"			: 6,
					"y"			: 28,
					
					"width"		: 823-100-63,
					"height"	: 265-7,
					
					"children" :
					(
						{
							"name" : "Background1",
							"type" : "expanded_image",
							
							"x" : 1,
							"y" : 6,
							
							"image" : "d:/ymir work/ui/special_guild/yunkai2rankguild.tga",
						},
						
						{
							"name" : "Background2",
							"type" : "expanded_image",
							
							"x" : 1,
							"y" : 6,
							
							"image" : "d:/ymir work/ui/special_guild/yunkai2rankguild.tga",
						},						
					),
				},

				{
					"name" : "ToolTipButton",
					"type" : "button",

					"x" : 789-162,
					"y" : 8,

					"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
					"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
					"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
				},					
			),
		},
	),
}