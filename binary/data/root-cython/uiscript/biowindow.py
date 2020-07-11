import uiScriptLocale
ROOT_PATH = "d:/ymir work/ui/public/"

window = {
	"name" : "BioWindow",
	"style" : ("movable", "float",),

	"x" : (SCREEN_WIDTH -518+400+800) / 2,
	"y" : (SCREEN_HEIGHT - 380+250) / 2,
	"width" : 148,
	"height" : 139+25,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,
			"width" : 148,
			"height" : 139+25,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 132,
					"color" : "gray",

					"children" :
					(
						{ "name":"titlename", "type":"text", "x":0, "y":3, 
						"text" : "Biyolog Ekraný", 
						"horizontal_align":"center", "text_horizontal_align":"center" },
					),
				},
				{
					"name" : "BeltInventorySlot",
					"type" : "grid_table",

					"x" : 5+50,
					"y" : 5+35,

					"start_index" : 999,
					"x_count" : 1,
					"y_count" : 1,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},
				{
					"name" : "countslot",
					"type" : "image",
					"x" : 40,
					"y" :80,
					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",
					"children" :
					(
						{
							"name" : "count",
							"type":"text",
							"text":"0/0",
							"x":0,
							"y":0,
							"r":1.0,
							"g":1.0,
							"b":1.0,
							"a":1.0,
							"all_align" : "center",
						},
					),
				},
				{
					"name" : "timeslot",
					"type" : "image",
					"x" : 40,
					"y" :80+20,
					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",
					"children" :
					(
						{
							"name" : "time",
							"type":"text",
							"text":"59:45",
							"x":0,
							"y":0,
							"r":1.0,
							"g":1.0,
							"b":1.0,
							"a":1.0,
							"all_align" : "center",
						},
					),
				},
				{
					"name" : "biobutton",
					"type" : "button",
					"x" : 42,
					"y" : 105+20,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
					"text" : "Ver",
				},
			),
		},
	),
}
