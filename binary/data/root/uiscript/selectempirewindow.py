import uiScriptLocale
import localeInfo
import app

ROOT_PATH = "d:/ymir work/ui/public/"
LOCALE_PATH = uiScriptLocale.EMPIRE_PATH

FLAG_SCALE_X = 0.5
FLAG_SCALE_Y = 0.5
ATALS_X = SCREEN_WIDTH/2 - 340
ATALS_Y = SCREEN_HEIGHT * (180) / 600
window = {
	"name" : "SelectCharacterWindow",

	"x" : 0,
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	(
		## Board
		{
			"name" : "BackGround", "type" : "expanded_image", 
			"x" : 0, "y" : 0, "x_scale" : float(SCREEN_WIDTH) / 1024.0, "y_scale" : float(SCREEN_HEIGHT) / 768.0,

			"image" : "d:/ymir work/ui/intro/empire/background/empire_select.jpg",
		},

		## Title
		{
			"name" : "Title",
			"type" : "expanded_image",

			"x" : ATALS_X + 70,
			"y" : 40,
			"x_scale" : 1.0 + ((SCREEN_WIDTH/800.0) - 1.0)*0.5,
			"y_scale" : 1.0 + ((SCREEN_HEIGHT/600.0) - 1.0)*0.5,

			"image" : "d:/ymir work/ui/empire/title.sub"
		},
		{
			"name" : "desc_phase", "type" : "image", "x" : 0, 
			"y" : SCREEN_HEIGHT * (156) / 600 - 129, "image" : ROOT_PATH + "public_intro_btn/descPhase_btn.sub",
			"children" :
			(
				{
					"name" : "desc_phase_text", "type" : "text",
					"x" : 18, "y" : 7, "r" : 0.7843, "g" : 0.7843, "b" : 0.7843,
					"text" : localeInfo.EMPIRE_SELECT_MODE,
					"fontsize" : "LARGE",
				},
			),
		},
		## Atlas
		{
			"name" : "Atlas",
			"type" : "image",

			"x" : ATALS_X,
			"y" : ATALS_Y,

			"image" : "d:/ymir work/ui/intro/empire/atlas.sub",

			"children" :
			(
				## Empire Image
				{
					"name" : "EmpireArea_A",
					"type" : "expanded_image",

					"x" : 43,
					"y" : 201,

					"image" : "d:/ymir work/ui/intro/empire/empirearea_a.sub"
				},
				{
					"name" : "EmpireArea_B",
					"type" : "expanded_image",

					"x" : 17,
					"y" : 16,

					"image" : "d:/ymir work/ui/intro/empire/empirearea_b.sub"
				},
				{
					"name" : "EmpireArea_C",
					"type" : "expanded_image",

					"x" : 314,
					"y" : 33,

					"image" : "d:/ymir work/ui/intro/empire/empirearea_c.sub"
				},

				## Empire Flag
				{
					"name" : "EmpireAreaFlag_A",
					"type" : "expanded_image",

					"x" : 167,
					"y" : 235,

					"image" : "d:/ymir work/ui/intro/empire/empireareaflag_a.sub"
				},
				{
					"name" : "EmpireAreaFlag_B",
					"type" : "expanded_image",

					"x" : 70,
					"y" : 42,

					"image" : "d:/ymir work/ui/intro/empire/empireareaflag_b.sub"
				},
				{
					"name" : "EmpireAreaFlag_C",
					"type" : "expanded_image",

					"x" : 357,
					"y" : 78,

					"image" : "d:/ymir work/ui/intro/empire/empireareaflag_c.sub"
				},
			),
		},

		## Buttons
		{
			"name" : "left_button", "type" : "button",
			"x" : ATALS_X + 120, "y" : ATALS_Y + 340,
			
			"default_image" : "d:/ymir work/ui/intro/select/left_button_01.sub",
			"over_image" : "d:/ymir work/ui/intro/select/left_button_02.sub",
			"down_image" : "d:/ymir work/ui/intro/select/left_button_03.sub",
		},
		{
			"name" : "right_button", "type" : "button",
			"x" : ATALS_X + 120 + 130, "y" : ATALS_Y + 340,
			
			"default_image" : "d:/ymir work/ui/intro/select/right_button_01.sub",
			"over_image" : "d:/ymir work/ui/intro/select/right_button_02.sub",
			"down_image" : "d:/ymir work/ui/intro/select/right_button_03.sub",
		},

		#Information
		{
			"name" : "empire_board",
			"type" : "thinboard_gold",

			"x" : SCREEN_WIDTH - 201 - SCREEN_WIDTH * (25) / 800,
			"y" : SCREEN_HEIGHT * (156) / 600,

			"width" : 205,
			"height" : 339,

			"children" :
			(
				## Empire Flag & Name##
				{	
					"name" : "EmpireNameSlot", "type" : "thinboard_circle",
					"x" : 13, "y" : 14, "width" : 180, "height" : 40,

					"children" :
					(
						{
							"name" : "EmpireFlag_A", "type" : "expanded_image",
							"x" : 0, "y" : 0, "x_scale" : FLAG_SCALE_X, "y_scale" : FLAG_SCALE_Y,
							"image" : "d:/ymir work/ui/intro/empire/empireflag_a.sub"
						},
						{
							"name" : "EmpireFlag_B", "type" : "expanded_image",
							"x" : 0, "y" : 0, "x_scale" : FLAG_SCALE_X, "y_scale" : FLAG_SCALE_Y,
							"image" : "d:/ymir work/ui/intro/empire/empireflag_b.sub"
						},
						{
							"name" : "EmpireFlag_C", "type" : "expanded_image",
							"x" : 0, "y" : 0, "x_scale" : FLAG_SCALE_X, "y_scale" : FLAG_SCALE_Y,
							"image" : "d:/ymir work/ui/intro/empire/empireflag_c.sub"
						},
						{
							"name" : "EmpireName", "type" : "text",
							"x" : 30, "y" : 0,
							"text" : "None",
							"fontsize" : "LARGE",
							"all_align" : "center",
						},
					),
				},
				{
					"name" : "text_board",
					"type" : "thinboard_circle",

					"x" : 11,
					"y" : 57,

					"width" : 180,
					"height" : 270,

					"children" :
					(
						{
							"name" : "prev_text_button", "type" : "button",
							"x" : 122, "y" : 250,

							"default_image" : ROOT_PATH + "public_intro_btn/prev_btn_01.sub",
							"over_image" 	: ROOT_PATH + "public_intro_btn/prev_btn_02.sub",
							"down_image" 	: ROOT_PATH + "public_intro_btn/prev_btn_01.sub",
						},
						{
							"name" : "next_text_button", "type" : "button",
							"x" : 122 + 20 + 10, "y" : 250,

							"default_image" : ROOT_PATH + "public_intro_btn/next_btn_01.sub",
							"over_image" 	: ROOT_PATH + "public_intro_btn/next_btn_02.sub",
							"down_image" 	: ROOT_PATH + "public_intro_btn/next_btn_01.sub",
						},
					),
				},
			),
		},
		
		## Buttons
		{
			"name" : "select_button", "type" : "button",
			"x" : SCREEN_WIDTH - 201 - SCREEN_WIDTH * (25) / 800, "y" : SCREEN_HEIGHT * (156) / 600 + 341,

			"default_image" : ROOT_PATH + "public_intro_btn/select_btn_01.sub",
			"over_image" 	: ROOT_PATH + "public_intro_btn/select_btn_02.sub",
			"down_image" 	: ROOT_PATH + "public_intro_btn/select_btn_03.sub",
		},
		{
			"name" : "exit_button", "type" : "button",
			"x" : SCREEN_WIDTH - 201 - SCREEN_WIDTH * (25) / 800 + 130, "y" : SCREEN_HEIGHT * (156) / 600 + 341,

			"default_image" : ROOT_PATH + "public_intro_btn/cancel_btn_01.sub",
			"over_image" 	: ROOT_PATH + "public_intro_btn/cancel_btn_02.sub",
			"down_image" 	: ROOT_PATH + "public_intro_btn/cancel_btn_03.sub",
		},
	),
}
