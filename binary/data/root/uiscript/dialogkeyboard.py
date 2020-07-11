import uiScriptLocale

ROOT = "d:/ymir work/ui/game/"

window = {
	"name" : "KeyBoard",

	"x" : SCREEN_WIDTH,
	"y" : 0,

	"width" : 80,
	"height" : 275,

	"children" :
	(
		{
			"name" : "LeftMouseButton",
			"type" : "button",

			"x" : 1,
			"y" : 2,

			"default_image" : ROOT + "TaskBar/Mouse_Button_Move_01.sub",
			"over_image" : ROOT + "TaskBar/Mouse_Button_Move_02.sub",
			"down_image" : ROOT + "TaskBar/Mouse_Button_Move_03.sub",
		},
		{
			"name" : "RightMouseButton",
			"type" : "button",

			"x" : 2,
			"y" : 1,

			"default_image" : ROOT + "TaskBar/Mouse_Button_Move_01.sub",
			"over_image" : ROOT + "TaskBar/Mouse_Button_Move_02.sub",
			"down_image" : ROOT + "TaskBar/Mouse_Button_Move_03.sub",
		},

		## QuickBar
		{
			"name" : "quickslot_board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 0,
			"height" : 275,

			"children" :
			(
				{
					"name" : "quick_slot_1",
					"type" : "grid_table",

					"start_index" : 0,

					"x" : 23,
					"y" : 9,

					"x_count" : 1,
					"y_count" : 4,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/Public/Slot_Base.sub",
					"image_r" : 1.0,
					"image_g" : 1.0,
					"image_b" : 1.0,
					"image_a" : 1.0,

					"children" :
					(
						{ "name" : "slot_1", "type" : "image", "x" : 3, "y" : 3, "image" : "d:/ymir work/ui/game/taskbar/5.sub", },
						{ "name" : "slot_2", "type" : "image", "x" : 3, "y" : 35, "image" : "d:/ymir work/ui/game/taskbar/6.sub", },
						{ "name" : "slot_3", "type" : "image", "x" : 3, "y" : 67, "image" : "d:/ymir work/ui/game/taskbar/7.sub", },
						{ "name" : "slot_4", "type" : "image", "x" : 3, "y" : 99, "image" : "d:/ymir work/ui/game/taskbar/8.sub", },
					),
				},
				{
					"name" : "quick_slot_2",
					"type" : "grid_table",

					"start_index" : 4,

					"x" : 23,
					"y" : 128 + 9,

					"x_count" : 1,
					"y_count" : 4,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/Public/Slot_Base.sub",
					"image_r" : 1.0,
					"image_g" : 1.0,
					"image_b" : 1.0,
					"image_a" : 1.0,

					"children" :
					(
						{ "name" : "slot_5", "type" : "image", "x" : 3, "y" : 3, "image" : "d:/ymir work/ui/game/keyboard/f5.tga", },
						{ "name" : "slot_6", "type" : "image", "x" : 3, "y" : 35, "image" : "d:/ymir work/ui/game/keyboard/f6.tga", },
						{ "name" : "slot_7", "type" : "image", "x" : 3, "y" : 67, "image" : "d:/ymir work/ui/game/keyboard/f7.tga", },
						{ "name" : "slot_8", "type" : "image", "x" : 3, "y" : 99, "image" : "d:/ymir work/ui/game/keyboard/f8.tga", },
					),
				},
			),
		},
	),
}
