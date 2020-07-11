import uiScriptLocale


EXP_Y_ADD_POSITION = 0

window = {
	"name" : "PetFeedWindow",
	"style" : ("movable", "float",),
	
	"x" : SCREEN_WIDTH - 500 + 328 -148,
	"y" : SCREEN_HEIGHT - 595 + 240,	

	"width" : 195,
	"height" : 225,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 195,
			"height" : 225,

			"children" :
			(
				## Pet Feed Title
				{
					"name" : "PetFeed_TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 0,
					"y" : 0,

					"width" : 193,
					
					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":0, "text":"Evcil Hayvan Besle", "all_align":"center" },
					),
				},
				## Pet Feed Area
				{
					"name" : "Pet_Feed_Area",
					"type" : "window",
					"style" : ("attach",),
					
					"x" : 0,
					"y" : 26,
					
					"width" : 192,
					"height" : 208,
					"children" :
					(								
						## Pet Feed Slot
						{
							"name" : "FeedItemSlot",
							"type" : "grid_table",

							"x" : 17,
							"y" : 4,

							"start_index" : 0,
							"x_count" : 5,
							"y_count" : 5,
							"x_step" : 32,
							"y_step" : 32,

							"image" : "d:/ymir work/ui/public/Slot_Base.sub"
						},
						
						## Pet Feed Button
						{
							"name" : "FeedButton",
							"type" : "button",

							"x" : 65,
							"y" : 169,

							"text" : "Onayla",

							"default_image" : "d:/ymir work/ui/dragonsoul/m_button01.tga",
							"over_image" : "d:/ymir work/ui/dragonsoul/m_button02.tga",
							"down_image" : "d:/ymir work/ui/dragonsoul/m_button03.tga",
						},
					),						
					
				}, ## End of Pet Feed Area
			),				
		},
	),
}
