import uiScriptLocale

window = {
	"name" : "CostumeEquipmentDialog",

	"x" : 0,
	"y" : 0,

	"width" : 140,
	"height" : 180 + 47 + 42 + 35,

	"children" :
	(
		{
			"name" : "ExpandButton",
			"type" : "button",

			"x" : 2,
			"y" : 15,

			"default_image" : "d:/ymir work/ui/game/belt_inventory/btn_expand_normal.tga",
			"over_image" : "d:/ymir work/ui/game/belt_inventory/btn_expand_over.tga",
			"down_image" : "d:/ymir work/ui/game/belt_inventory/btn_expand_down.tga",
			"disable_image" : "d:/ymir work/ui/game/belt_inventory/btn_expand_disabled.tga",
		},

		{
			"name" : "CostumeEquipmentLayer",

			"x" : 5,
			"y" : 0,

			"width" : 140,
			"height" : 180 + 47 + 42 + 35,

			"children" :
			(
				{
					"name" : "MinimizeButton",
					"type" : "button",

					"x" : 2,
					"y" : 15,

					"default_image" : "d:/ymir work/ui/game/belt_inventory/btn_minimize_normal.tga",
					"over_image" : "d:/ymir work/ui/game/belt_inventory/btn_minimize_over.tga",
					"down_image" : "d:/ymir work/ui/game/belt_inventory/btn_minimize_down.tga",
					"disable_image" : "d:/ymir work/ui/game/belt_inventory/btn_minimize_disabled.tga",
				},

				{
					"name" : "CostumeEquipmentBoard",
					"type" : "board",
					"style" : ("attach", "float"),

					"x" : 10,
					"y" : -25,

					"width" : 132,
					"height" : 180 + 47 + 42,

					"children" :
					(
						{
							"name" : "Costume_Base",
							"type" : "image",

							"x" : 8,
							"y" : 8,

							"image" : "d:/ymir work/ui/costume_bg.jpg",

							"children" :
							(
								{
									"name" : "CostumeEquipmentSlot",
									"type" : "slot",

									"x" : 3,
									"y" : 3,

									"width" : 127,
									"height" : 188+35,

									"slot" : (
										{"index":11, "x":61, "y":45, "width":32, "height":64}, ## costume head
										{"index":12, "x":61, "y": 8, "width":32, "height":32}, ## costume hair
										{"index":13, "x":11, "y": 14, "width":32, "height":96}, ## costume weapon
										{"index":14, "x":12, "y": 126, "width":32, "height":32}, ## sash
										{"index":15, "x":62, "y": 126, "width":32, "height":32}, ## mount
										{"index":16, "x":61, "y": 163, "width":32, "height":32}, ## sash
										{"index":17, "x":12, "y": 163, "width":32, "height":32}, ## mount
										{"index":18, "x":12, "y":200, "width":32, "height":32},
										{"index":19, "x":61, "y":200, "width":32, "height":32},	
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
