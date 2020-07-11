import uiScriptLocale

ROOT = "d:/ymir work/ui/game/"

Y_ADD_POSITION = 0
window = {
	"name" : "ExpandTaskBar",

	"x" : SCREEN_WIDTH/2 - 5,
	"y" : SCREEN_HEIGHT - 74,

	"width" : 147,
	"height" : 37,

	"children" :
	(
		{
			"name" : "ExpanedTaskBar_Board",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 147,
			"height" : 37,

			"children" :
			(
				{
					"name" : "DragonSoulButton",
					"type" : "button",

					"x" : 0,
					"y" : 0,

					"width" : 37,
					"height" : 37,

					"tooltip_text" : uiScriptLocale.TASKBAR_DISABLE,
											
					"default_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_01.tga",
					"over_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_02.tga",
					"down_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_03.tga",
				},
								
				{
					"name" : "PetGuiButton",
					"type" : "button",

					"x" : 74,
					"y" : 0,

					"width" : 37,
					"height" : 37,

					"tooltip_text" : "Pet",

					"default_image" : "d:/ymir work/ui/pet/taskbar_pet_button_01.tga",
					"over_image" : "d:/ymir work/ui/pet/taskbar_pet_button_02.tga",
					"down_image" : "d:/ymir work/ui/pet/taskbar_pet_button_03.tga",
				},
				{
					"name" : "AutoButton",
					"type" : "button",
					"style" : ("ltr", ),

					"x" : 110,
					"y" : 0,

					"width" : 37,
					"height" : 37,

					"tooltip_text" : "Otomatik Av",
							
					"default_image" : "icon/item/TaskBar_Auto_Button_01.tga",
					"over_image" : "icon/item/TaskBar_Auto_Button_02.tga",
					"down_image" : "icon/item/TaskBar_Auto_Button_03.tga",
				},
			),
		},
	),
		
}
