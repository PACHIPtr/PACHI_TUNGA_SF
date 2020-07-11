import uiScriptLocale
import localeInfo
ROOT_PATH = "d:/ymir work/ui/public/"
LOCALE_PATH = "d:/ymir work/ui/intro/select/"

BOARD_X = SCREEN_WIDTH * (25) / 800
BOARD_Y = SCREEN_HEIGHT * (156) / 600

## Default Value ##
X_GAP = 11
Y_GAP = 12

SELECT_BTN_GAP = 44

NAME_X = 18
NAME_Y = 84
NAME_SCALE_X = 0.65
NAME_SCALE_Y = 0.65

FACE_X = 4
FACE_Y = -23

SMALL_NAME_X = 150 - 65
SMALL_NAME_Y = 6
SMALL_NAME_SCALE_X = 1.0
SMALL_NAME_SCALE_Y = 1.0

FLAG_SCALE_X = 0.45
FLAG_SCALE_Y = 0.45

SHADOW_SCALE_X = 3.0 * SCREEN_WIDTH  / 800.0
SHADOW_SCALE_Y = 2.0 * SCREEN_HEIGHT / 600.0

STAT_GAUGE_X = X_GAP + 3
STAT_GAUGE_Y = 286
STAT_GAUGE_BAR_X = 40
STAT_GAUGE_BAR_WIDTH = 105
STAT_GAUGE_GAP = 18
STAT_GAUGE_TEXT_WIDTH = 21
STAT_GAUGE_TEXT_HEIGHT = 13

#THINBOARD_GOLD_WIDTH = 198#194
THINBOARD_GOLD_HEIGHT = 364#359

THINBOARD_CIRCLE_LEFT_WIDTH = 175

#SELECT_Y = BOARD_Y + 312 + 7

THINBOARD_CIRCLE_RIGHT_WIDTH = 180
THINBOARD_CIRCLE_RIGHT_HEIGHT = 270
###################

##대괄호 양식도 완전히 똑같아야됨!##
window = {
	"name" : "New_CreateCharacterWindow",
	"x" : 0, "y" : 0, "width" : SCREEN_WIDTH, "height" : SCREEN_HEIGHT,
	
	"children"	:
	(
		## BackGround##
		{
			"name" : "BackGround", "type" : "expanded_image", "x" : 0, "y" : 0, "x_scale" : float(SCREEN_WIDTH) / 1024.0, "y_scale" : float(SCREEN_HEIGHT) / 768.0,
			"image" : LOCALE_PATH + "empire/background/empire_sinsu.sub",
		},
		{
			"name" : "desc_phase", "type" : "image", "x" : 0, "y" : BOARD_Y - NAME_Y - 45, "image" : ROOT_PATH + "public_intro_btn/descPhase_btn.sub",
			"children" :
			(
				{
					"name" : "desc_phase_text", "type" : "text",
					"x" : 18, "y" : 7, "r" : 0.7843, "g" : 0.7843, "b" : 0.7843,
					"text" : localeInfo.CHARACTER_CREATE_MODE,
					"fontsize" : "LARGE",
				},
			),
		},
		## Shadow ##
		{
			"name" : "Shadow", "type" : "expanded_image", "x" : SCREEN_WIDTH/2 - (64)*SHADOW_SCALE_X, "y" : SCREEN_HEIGHT - SCREEN_HEIGHT*(70)/600.0 - (88)*SHADOW_SCALE_Y/2.0, 
			"x_scale" : SHADOW_SCALE_X, "y_scale" : SHADOW_SCALE_Y,
			"image" : LOCALE_PATH + "shadow/shadow.tga",#test.bmp",
		},
		
		## Big Name##
		{
			"name" : "name_warrior", "type" : "expanded_image", 
			"x" : BOARD_X + NAME_X, "y" : BOARD_Y - NAME_Y, "x_scale" : NAME_SCALE_X, "y_scale" : NAME_SCALE_Y,
			"image" : LOCALE_PATH + "name_warrior.sub",
		},
		{
			"name" : "name_assassin", "type" : "expanded_image",
			"x" : BOARD_X + NAME_X, "y" : BOARD_Y - NAME_Y, "x_scale" : NAME_SCALE_X, "y_scale" : NAME_SCALE_Y,
			"image" : LOCALE_PATH + "name_assassin.sub",
		},
		{
			"name" : "name_sura", "type" : "expanded_image",
			"x" : BOARD_X + NAME_X, "y" : BOARD_Y - NAME_Y, "x_scale" : NAME_SCALE_X, "y_scale" : NAME_SCALE_Y,
			"image" : LOCALE_PATH + "name_sura.sub",
		},
		{
			"name" : "name_shaman", "type" : "expanded_image",
			"x" : BOARD_X + NAME_X, "y" : BOARD_Y - NAME_Y, "x_scale" : NAME_SCALE_X, "y_scale" : NAME_SCALE_Y,
			"image" : LOCALE_PATH + "name_shaman.sub",
		},
		{
			"name" : "name_wolfman", "type" : "expanded_image",
			"x" : BOARD_X + NAME_X, "y" : BOARD_Y - NAME_Y, "x_scale" : NAME_SCALE_X, "y_scale" : NAME_SCALE_Y,
			"image" : LOCALE_PATH + "name_wolfman.sub",
		},
				
		##Character Board(Left)##
		{
			"name" : "character_board", "type" : "thinboard_gold",
			"x" : BOARD_X, "y" : BOARD_Y, "width" : THINBOARD_CIRCLE_LEFT_WIDTH + (X_GAP * 2) + 1, "height" : 305,
			
			"children" :
			(
				## Empire Flag & Name##
				{	
					"name" : "EmpireNameSlot", "type" : "thinboard_circle",
					"x" : X_GAP, "y" : Y_GAP, "width" : THINBOARD_CIRCLE_LEFT_WIDTH, "height" : 42,

					"children" :
					(
						{
							"name" : "EmpireFlag", "type" : "expanded_image",
							"x" : 4, "y" : 3, "x_scale" : FLAG_SCALE_X, "y_scale" : FLAG_SCALE_Y,
							"image" : LOCALE_PATH + "empire/empireflag_a.sub"
						},
						{
							"name" : "EmpireName", "type" : "text",
							"x" : 29, "y" : 0,
							"text" : uiScriptLocale.SELECT_EMPIRE_NAME,
							"fontsize" : "LARGE",
							"all_align" : "center",
						},
					),
				},
				
				{
					"name" : "SelectJobSlot", "type" : "thinboard_circle",
					"x" : X_GAP, "y" : 61, "width" : THINBOARD_CIRCLE_LEFT_WIDTH, "height" : 231,
					"children" :
					(
						##Job Select##
						{
							"name" : "WARRIOR",	"type" : "radio_button", 
							"x" : X_GAP + 4, "y" : 10,
			
							"default_image" : ROOT_PATH + "public_intro_btn/150x35_Createbtn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_03.sub",
							"children" :
							(
								# {
									# "name" : "character_gender", "type" : "text",
									# "x" : 145, "y" : 19,
									# "text" : localeInfo.JOB_WARRIOR,
									# "text_horizontal_align" : "right",
								# },
								{ 
									"name" : "CharacterFace_0", "type" : "image", 
									"x" : -6, "y" : -4, 
									"image" : "D:/ymir work/ui/intro/public_intro/face/face_warrior_m_01.sub" # size : 43 x 43
								},
								{								
									"name" : "name_warrior_small", "type" : "expanded_image", 
									"x" : SMALL_NAME_X, "y" : SMALL_NAME_Y, "x_scale" : SMALL_NAME_SCALE_X, "y_scale" : SMALL_NAME_SCALE_Y,
									"image" : LOCALE_PATH + "public_intro/name/name_warrior_small.sub",
								},	
							),
						},
						{
							"name" : "ASSASSIN", "type" : "radio_button",
							"x" : X_GAP + 4, "y" : 10 + SELECT_BTN_GAP,

							"default_image" : ROOT_PATH + "public_intro_btn/150x35_Createbtn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_03.sub",
							"children" :
							(
								{ 
									"name" : "CharacterFace_1", "type" : "image", 
									"x" : -6, "y" : -4, 
									"image" : "D:/ymir work/ui/intro/public_intro/face/face_assassin_m_01.sub" # size : 43 x 43
								},
								{								
									"name" : "name_assassin_small", "type" : "expanded_image",
									"x" : SMALL_NAME_X, "y" : SMALL_NAME_Y, "x_scale" : SMALL_NAME_SCALE_X, "y_scale" : SMALL_NAME_SCALE_Y,
									"image" : LOCALE_PATH + "public_intro/name/name_assassin_small.sub",
								},	
							),
						},
						{
							"name" : "SURA", "type" : "radio_button",
							"x" : X_GAP + 4, "y" : 10 + SELECT_BTN_GAP*2,

							"default_image" : ROOT_PATH + "public_intro_btn/150x35_Createbtn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_03.sub",
							"children" :
							(
								{ 
									"name" : "CharacterFace_2", "type" : "image", 
									"x" : -6, "y" : -4, 
									"image" : "D:/ymir work/ui/intro/public_intro/face/face_sura_m_01.sub" # size : 43 x 43
								},
								{								
									"name" : "name_sura_small", "type" : "expanded_image",
									"x" : SMALL_NAME_X, "y" : SMALL_NAME_Y, "x_scale" : SMALL_NAME_SCALE_X, "y_scale" : SMALL_NAME_SCALE_Y,
									"image" : LOCALE_PATH + "public_intro/name/name_sura_small.sub",
								},	
							),
						},
						{
							"name" : "SHAMAN", "type" : "radio_button",
							"x" : X_GAP + 4, "y" : 10 + SELECT_BTN_GAP*3,

							"default_image" : ROOT_PATH + "public_intro_btn/150x35_Createbtn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_03.sub",
							"children" :
							(
								{ 
									"name" : "CharacterFace_3", "type" : "image", 
									"x" : -6, "y" : -4, 
									"image" : "D:/ymir work/ui/intro/public_intro/face/face_shaman_m_01.sub" # size : 43 x 43
								},
								{								
									"name" : "name_shaman_small", "type" : "expanded_image",
									"x" : SMALL_NAME_X, "y" : SMALL_NAME_Y, "x_scale" : SMALL_NAME_SCALE_X, "y_scale" : SMALL_NAME_SCALE_Y,
									"image" : LOCALE_PATH + "public_intro/name/name_shaman_small.sub",
								},	
							),
						},
						{
							"name" : "WOLFMAN", "type" : "radio_button",
							"x" : X_GAP + 4, "y" : 10 + SELECT_BTN_GAP*4,

							"default_image" : ROOT_PATH + "public_intro_btn/150x35_Createbtn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/150x35_Createbtn_03.sub",
							"children" :
							(
								{ 
									"name" : "CharacterFace_4", "type" : "image", 
									"x" : -6, "y" : -4, 
									"image" : "D:/ymir work/ui/intro/public_intro/face/face_wolfman_m_01.sub" # size : 43 x 43
								},
								{								
									"name" : "name_wolfman_small", "type" : "expanded_image",
									"x" : SMALL_NAME_X, "y" : SMALL_NAME_Y, "x_scale" : SMALL_NAME_SCALE_X, "y_scale" : SMALL_NAME_SCALE_Y,
									"image" : LOCALE_PATH + "public_intro/name/name_wolfman_small.sub",
								},	
							),
						},
					),
				}, 
			), #Character children
		},##Character Board(Left)##
		## Select Sex & Shape ##
		{
			"name" : "select_thinboard2", "type" : "thinboard_gold", 
			"x" : BOARD_X, "y" : BOARD_Y + 305 + 3, "width" : THINBOARD_CIRCLE_LEFT_WIDTH + (X_GAP * 2) + 1, "height" : (Y_GAP * 2) + 48,
			"children" : 
			(
				{
					"name" : "SelectSex", "type" : "image", 
					"x" : X_GAP - 1, "y" : Y_GAP, "image" : ROOT_PATH + "public_intro_btn/88x47_btn.sub",
					"children" :
					(
						{
							"name" : "gender_button_01", "type" : "radio_button",
							"x" : 5, "y" : 5, 

							"default_image" : ROOT_PATH + "public_intro_btn/sex_M_btn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/sex_M_btn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/sex_M_btn_03.sub",
						},
						{
							"name" : "gender_button_02", "type" : "radio_button",
							"x" : 88 - 5 - 37, "y" : 5, 
											
							"default_image" : ROOT_PATH + "public_intro_btn/sex_W_btn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/sex_W_btn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/sex_W_btn_03.sub",
						},
					),
				},
				{
					"name" : "SelectShpae", "type" : "image", 
					"x" : THINBOARD_CIRCLE_LEFT_WIDTH + X_GAP - 87, "y" : Y_GAP, "image" : ROOT_PATH + "public_intro_btn/88x47_btn.sub",
					"children" : 
					(
						{
							"name" : "shape_button_01", "type" : "radio_button",
							"x" : 5, "y" : 5,

							"default_image" : ROOT_PATH + "public_intro_btn/shape1_btn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/shape1_btn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/shape1_btn_03.sub",
						},
						{
							"name" : "shape_button_02", "type" : "radio_button",
							"x" : 88 - 5 - 37, "y" : 5,

							"default_image" : ROOT_PATH + "public_intro_btn/shape2_btn_01.sub",
							"over_image"	: ROOT_PATH + "public_intro_btn/shape2_btn_02.sub",
							"down_image"	: ROOT_PATH + "public_intro_btn/shape2_btn_03.sub",
						},
					),
				},
			),
		},

		##Description Board(Right)##
		{
			"name" : "character_discriptionboard", "type" : "thinboard_gold",
			"x" : SCREEN_WIDTH - BOARD_X - (THINBOARD_CIRCLE_RIGHT_WIDTH + (X_GAP * 2)), "y" : BOARD_Y, "width" : THINBOARD_CIRCLE_RIGHT_WIDTH + (X_GAP * 2) + 1, "height" : THINBOARD_GOLD_HEIGHT,
			
			"children"	:
			(
				{ 
					"name" : "raceName_Btn", "type" : "image", "x" : 10, "y" : -17, "image" : ROOT_PATH + "public_intro_btn/raceName_btn.sub",
					"children" :
					(
						{ "name" : "DiscFace", "type" : "image", "x" : FACE_X, "y" : FACE_Y, "image" : "icon/face/warrior_m.tga"},
						{ "name" : "raceName_Text",	"type" : "text", "x" : 100, "y" : 5, "r" : 0.7843, "g" : 0.7843, "b" : 0.7843, "text" : localeInfo.JOB_WARRIOR, "text_horizontal_align" : "center"}
						
					),
				},
				{
					"name" : "text_board", "type" : "thinboard_circle",
					"x" : X_GAP, "y" : Y_GAP, "width" : THINBOARD_CIRCLE_RIGHT_WIDTH, "height" : THINBOARD_CIRCLE_RIGHT_HEIGHT,
					
					"children" :
					(
						{
							"name" : "prev_button", "type" : "button",
							"x" : 122, "y" : 250,

							"default_image" : ROOT_PATH + "public_intro_btn/prev_btn_01.sub",
							"over_image" 	: ROOT_PATH + "public_intro_btn/prev_btn_02.sub",
							"down_image" 	: ROOT_PATH + "public_intro_btn/prev_btn_01.sub",
						},
						{
							"name" : "next_button", "type" : "button",
							"x" : 122 + 20 + 10, "y" : 250,

							"default_image" : ROOT_PATH + "public_intro_btn/next_btn_01.sub",
							"over_image" 	: ROOT_PATH + "public_intro_btn/next_btn_02.sub",
							"down_image" 	: ROOT_PATH + "public_intro_btn/next_btn_01.sub",
						},
					),
				},
				
				##HP##
				{
					"name" : "hth",	"type" : "text",
					"x" : STAT_GAUGE_X, "y" : STAT_GAUGE_Y,

					"text" : uiScriptLocale.CREATE_HP,

					"children" :
					(
						{
							"name" : "hth_gauge", "type" : "gauge",
							"x" : STAT_GAUGE_BAR_X, "y" : 4, "width" : STAT_GAUGE_BAR_WIDTH, "color" : "red",
						},
						{
							"name" : "hth_slot", "type" : "slotbar",
							"x" : STAT_GAUGE_BAR_WIDTH + STAT_GAUGE_BAR_X + 7, "y" : -1, "width" : STAT_GAUGE_TEXT_WIDTH, "height" : STAT_GAUGE_TEXT_HEIGHT,

							"children" :
							(
								{
									"name" : "hth_value", "type" : "text",
									"x" : 0, "y" : 1,
									"all_align" : "center",
									"text" : "0",
								},
							),
						},
					),
				},##HP END##
				##INT##
				{
					"name" : "int",	"type" : "text",
					"x" : STAT_GAUGE_X, "y" : STAT_GAUGE_Y + STAT_GAUGE_GAP,

					"text" : uiScriptLocale.CREATE_SP,

					"children" :
					(
						{
							"name" : "int_gauge", "type" : "gauge",
							"x" : STAT_GAUGE_BAR_X, "y" : 4, "width" : STAT_GAUGE_BAR_WIDTH, "color" : "pink",
						},
						{
							"name" : "int_slot", "type" : "slotbar",
							"x" : STAT_GAUGE_BAR_WIDTH + STAT_GAUGE_BAR_X + 7, "y" : -1, "width" : STAT_GAUGE_TEXT_WIDTH, "height" : STAT_GAUGE_TEXT_HEIGHT,

							"children" :
							(
								{
									"name" : "int_value", "type" : "text",
									"x" : 0, "y" : 1,
									"all_align" : "center",
									"text" : "0",
								},
							),
						},
					),
				},##INT END##
				##STR##
				{
					"name" : "str",	"type" : "text",
					"x" : STAT_GAUGE_X, "y" : STAT_GAUGE_Y + STAT_GAUGE_GAP*2,

					"text" : uiScriptLocale.CREATE_ATT_GRADE,

					"children" :
					(
						{
							"name" : "str_gauge", "type" : "gauge",
							"x" : STAT_GAUGE_BAR_X, "y" : 4, "width" : STAT_GAUGE_BAR_WIDTH, "color" : "purple",
						},
						{
							"name" : "str_slot", "type" : "slotbar",
							"x" : STAT_GAUGE_BAR_WIDTH + STAT_GAUGE_BAR_X + 7, "y" : -1, "width" : STAT_GAUGE_TEXT_WIDTH, "height" : STAT_GAUGE_TEXT_HEIGHT,

							"children" :
							(
								{
									"name" : "str_value", "type" : "text",
									"x" : 0, "y" : 1,
									"all_align" : "center",
									"text" : "0",
								},
							),
						},
					),
				},##STR END##
				##DEX##
				{
					"name" : "dex",	"type" : "text",
					"x" : STAT_GAUGE_X, "y" : STAT_GAUGE_Y + STAT_GAUGE_GAP*3,

					"text" : uiScriptLocale.CREATE_DEX_GRADE,

					"children" :
					(
						{
							"name" : "dex_gauge", "type" : "gauge",
							"x" : STAT_GAUGE_BAR_X, "y" : 4, "width" : STAT_GAUGE_BAR_WIDTH, "color" : "blue",
						},
						{
							"name" : "dex_slot", "type" : "slotbar",
							"x" : STAT_GAUGE_BAR_WIDTH + STAT_GAUGE_BAR_X + 7, "y" : -1, "width" : STAT_GAUGE_TEXT_WIDTH, "height" : STAT_GAUGE_TEXT_HEIGHT,

							"children" :
							(
								{
									"name" : "dex_value", "type" : "text",
									"x" : 0, "y" : 1,
									"all_align" : "center",
									"text" : "0",
								},
							),
						},
					),
				},##DEX END##	
			), 
		},##Description Board(Right)##
		
		##Character ID, MAKE OR CANCEL ##
		{
			"name" : "textAlpha", "type" : "image",
			"x" : SCREEN_WIDTH/2 - 153, "y" : SCREEN_HEIGHT - SCREEN_HEIGHT*(70)/600.0 - 31,

			"image" : ROOT_PATH + "public_intro_btn/textAlpha.sub",
					
			"children" :
			(
				{
					"name" : "character_name", "type" : "text",
					"x" : 58, "y" : 6, "r" : 0.7843, "g" : 0.7843, "b" : 0.7843,

					"text" : uiScriptLocale.CREATE_NAME, "text_horizontal_align" : "center",
				},
				{
					"name" : "character_name_slot", "type" : "image",
					"x" : 97, "y" : 4,

					"image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
				},
				{
					"name" : "character_name_value", "type" : "editline",
					"x" : 100, "y" : 6,
					"input_limit" : 12,
					"width" : 90, "height" : 20,
				},
			),
		},
		{
			"name" : "create_button", "type" : "button",
			"x" : SCREEN_WIDTH/2 - 107, "y" : SCREEN_HEIGHT - SCREEN_HEIGHT*(70)/600.0,

			"default_image" : ROOT_PATH + "public_intro_btn/addcharacter_btn_01.sub",
			"over_image"	: ROOT_PATH + "public_intro_btn/addcharacter_btn_02.sub",
			"down_image"	: ROOT_PATH + "public_intro_btn/addcharacter_btn_03.sub",
		},
		{
			"name" : "cancel_button", "type" : "button",
			"x" : SCREEN_WIDTH/2 + 32, "y" : SCREEN_HEIGHT - SCREEN_HEIGHT*(70)/600.0,

			"default_image" : ROOT_PATH + "public_intro_btn/cancel_btn_01.sub",
			"over_image"	: ROOT_PATH + "public_intro_btn/cancel_btn_02.sub",
			"down_image"	: ROOT_PATH + "public_intro_btn/cancel_btn_03.sub",
		},##Character ID, MAKE OR CANCEL ##
	), #New_CreateCharacterWindow children
}
