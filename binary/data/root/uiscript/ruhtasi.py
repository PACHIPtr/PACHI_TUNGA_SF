import uiScriptLocale

ROOT_PATH = "d:/ymir work/ui/public/"

window = {
	"name" : "RuhTasiDialog",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : 300,
	"height" : 195,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 300,
			"height" : 195,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 284,
					"color" : "gray",

					"children" :
					(
						{ "name":"titlename", "type":"text", "x":0, "y":3, 
						"text" : "Grandmaster Eðitimi", 
						"horizontal_align":"center", "text_horizontal_align":"center" },
					),
				},
				
				{
					"name" : "derece2",
					"type" : "text",

					"x" : 70,
					"y" : 40,

					"text" : "Mevcut Derece : -20000",

				},
				
				{
					"name" : "derece",
					"type" : "text",

					"x" : 190,
					"y" : 40,

					"text" : "Zalim",

				},

				{
					"name" : "ruhslot",
					"type" : "grid_table",

					"start_index" : 0,

					"x" : 80,
					"y" : 60,

					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,

					"image" : "d:/ymir work/ui/public/slot_base.sub",
				},	

				{
					"name" : "zenslot",
					"type" : "grid_table",

					"start_index" : 1,

					"x" : 115,
					"y" : 60,

					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,

					"image" : "d:/ymir work/ui/public/slot_base.sub",
				},
				{
					"name" : "kotuslot",
					"type" : "grid_table",

					"start_index" : 2,

					"x" : 150,
					"y" : 60,

					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,

					"image" : "d:/ymir work/ui/public/slot_base.sub",
				},	
				{
					"name" : "munzevislot",
					"type" : "grid_table",

					"start_index" : 3,

					"x" : 185,
					"y" : 60,

					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,
					"x_blank" : 0,
					"y_blank" : 0,

					"image" : "d:/ymir work/ui/public/slot_base.sub",
				},	

				
				{
					"name" : "skillslot",
					"type" : "slot",

					"x" : 68-20,
					"y" : 100,

					"width" : 223,
					"height" : 223,
					"image" : "d:/ymir work/ui/public/slot_base.sub",

					"slot" :	(
									{"index": 0, "x": 0, "y":  0, "width":35, "height":35},
									{"index": 1, "x": 35, "y":  0, "width":35, "height":35},
									{"index": 2, "x": 70, "y":  0, "width":35, "height":35},
									{"index": 3, "x": 105, "y":  0, "width":35, "height":35},
									{"index": 4, "x": 140, "y":  0, "width":35, "height":35},
									{"index": 5, "x": 175, "y":  0, "width":35, "height":35},

							),
				},
				
				{
					"name" : "tek",
					"type" : "button",

					"x" : 80,
					"y" : 150,
					
					"text"	:	"Tek",

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},	

{
					"name" : "hepsi",
					"type" : "button",

					"x" : 155,
					"y" : 150,
					
					"text"	:	"Hepsi",

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},				
								
				
				
				
			),
		},
	),
}
