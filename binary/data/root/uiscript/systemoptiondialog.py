import uiScriptLocale
import app

ROOT_PATH = "d:/ymir work/ui/public/"

TEMPORARY_X = +13
TEXT_TEMPORARY_X = -10
BUTTON_TEMPORARY_X = 5
PVP_X = -10

window = {
	"name" : "SystemOptionDialog",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width" : 308,
	"height" : 328,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 308,
			"height" : 328,

			"children" :
			(
				## Title
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,

					"width" : 292,
					"color" : "gray",

					"children" :
					(
						{ 
						"name":"titlename", "type":"text", "x":0, "y":3, 
						"horizontal_align":"center", "text_horizontal_align":"center",
						"text": uiScriptLocale.SYSTEMOPTION_TITLE, 
						 },
					),
				},

				{
					"name" : "Board_Name2",
					"type" : "new_board",

					"x" : 7,
					"y" : 30,
	
					"width" : 290,
					"height" : 258,

				},
				
				{
					"name" : "NextPage",
					"type" : "button",
					"x" : 185,
					"y" : 295,
					"default_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_01.sub",
					"over_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_01.sub",
					"down_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_02.sub",
				},
				{
					"name" : "PrevPage",
					"type" : "button",
					"x" : 105,
					"y" : 295,
					"default_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_01.sub",
					"over_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_01.sub",
					"down_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_02.sub",
				},
				
				## Music
				{
					"name" : "music_name",
					"type" : "text",

					"x" : 30,
					"y" : 75,

					"text" : uiScriptLocale.OPTION_MUSIC,
				},
				
				{
					"name" : "music_volume_controller",
					"type" : "sliderbar",

					"x" : 110,
					"y" : 75,
				},
				
				{
					"name" : "bgm_button",
					"type" : "button",

					"x" : 20,
					"y" : 100+20+5+25,

					"text" : uiScriptLocale.OPTION_MUSIC_CHANGE,

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				
				{
					"name" : "bgm_file",
					"type" : "text",

					"x" : 100,
					"y" : 102+20+5+25,

					"text" : uiScriptLocale.OPTION_MUSIC_DEFAULT_THEMA,
				},
			
				## Sound
				{
					"name" : "sound_name",
					"type" : "text",

					"x" : 30,
					"y" : 50,

					"text" : uiScriptLocale.OPTION_SOUND,
				},
				
				{
					"name" : "sound_volume_controller",
					"type" : "sliderbar",

					"x" : 110,
					"y" : 50,
				},	
				
				## 그림자
				{
					"name" : "shadow_mode",
					"type" : "text",

					"x" : 30,
					"y" : 100,

					"text" : uiScriptLocale.OPTION_SHADOW,
				},
				
				{
					"name" : "shadow_bar",
					"type" : "sliderbar",

					"x" : 110,
					"y" : 100,
				},
				{
					"name" : "shop_name_range",
					"type" : "text",

					"x" : 30,
					"y" : 125,

					"text" : uiScriptLocale.OPTION_SHOPNAME_RANGE,
				},

				{
					"name" : "salestext_range_controller",
					"type" : "sliderbar",

					"x" : 110,
					"y" : 125,
				},

				## 카메라
				{
					"name" : "camera_mode",
					"type" : "text",

					"x" : 40 + TEXT_TEMPORARY_X,
					"y" : 135+2+20+25,

					"text" : uiScriptLocale.OPTION_CAMERA_DISTANCE,
				},
				
				{
					"name" : "camera_short",
					"type" : "radio_button",

					"x" : 110,
					"y" : 135+20+25,

					"text" : uiScriptLocale.OPTION_CAMERA_DISTANCE_SHORT,

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				
				{
					"name" : "camera_long",
					"type" : "radio_button",

					"x" : 110+70,
					"y" : 135+20+25,

					"text" : uiScriptLocale.OPTION_CAMERA_DISTANCE_LONG,

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},

				## 안개
				{
					"name" : "fog_mode",
					"type" : "text",

					"x" : 30,
					"y" : 160+20+2+25,

					"text" : uiScriptLocale.OPTION_FOG,
				},
				
				{
					"name" : "fog_on",
					"type" : "radio_button",

					"x" : 110,
					"y" : 160+20+25,

					"text" : uiScriptLocale.OPTION_FOG_ON,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "fog_off",
					"type" : "radio_button",

					"x" : 110+50,
					"y" : 160+20+25,

					"text" : uiScriptLocale.OPTION_FOG_OFF,
					
					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},

				## 타일 가속
				{
					"name" : "tiling_mode",
					"type" : "text",

					"x" : 40 + TEXT_TEMPORARY_X,
					"y" : 185+2+20+25,

					"text" : uiScriptLocale.OPTION_TILING,
				},
				
				{
					"name" : "tiling_cpu",
					"type" : "radio_button",

					"x" : 110,
					"y" : 185+20+25,

					"text" : uiScriptLocale.OPTION_TILING_CPU,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "tiling_gpu",
					"type" : "radio_button",

					"x" : 110+50,
					"y" : 185+20+25,

					"text" : uiScriptLocale.OPTION_TILING_GPU,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "tiling_apply",
					"type" : "button",

					"x" : 110+100,
					"y" : 185+20+25,

					"text" : uiScriptLocale.OPTION_TILING_APPLY,

					"default_image" : ROOT_PATH + "middle_Button_01.sub",
					"over_image" : ROOT_PATH + "middle_Button_02.sub",
					"down_image" : ROOT_PATH + "middle_Button_03.sub",
				},

				{
					"name" : "night_mode",
					"type" : "text",

					"x" : 30,
					"y" : 210+20+2+25,

					"text" : uiScriptLocale.OPTION_NIGHT,
				},
				
				{
					"name" : "night_on",
					"type" : "radio_button",

					"x" : 110,
					"y" : 210+20+25,

					"text" : uiScriptLocale.OPTION_NIGHT_ON,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "night_off",
					"type" : "radio_button",

					"x" : 110+50,
					"y" : 210+20+25,

					"text" : uiScriptLocale.OPTION_NIGHT_OFF,
					
					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				## 타일 가속
				{
					"name" : "textures_mode",
					"type" : "text",

					"x" : 30,
					"y" : 40+2,

					"text" : uiScriptLocale.OPTION_TEXTURES,
				},
				
				{
					"name" : "texture_classic",
					"type" : "radio_button",

					"x" : 110,
					"y" : 40,

					"text" : uiScriptLocale.OPTION_TEXTURES_CLASSIC,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "texture_snow",
					"type" : "radio_button",

					"x" : 110+50,
					"y" : 40,

					"text" : uiScriptLocale.OPTION_TEXTURES_SNOW,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "texture_desert",
					"type" : "radio_button",

					"x" : 110+100,
					"y" : 40,

					"text" : uiScriptLocale.OPTION_TEXTURES_DESERT,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "snow_mode",
					"type" : "text",

					"x" : 30,
					"y" : 65+2,

					"text" : uiScriptLocale.OPTION_SNOW_MODE,
				},
				
				{
					"name" : "snow_mode_on",
					"type" : "radio_button",

					"x" : 110,
					"y" : 65,

					"text" : uiScriptLocale.OPTION_SNOW_MODE_ON,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "snow_mode_off",
					"type" : "radio_button",

					"x" : 110+50,
					"y" : 65,

					"text" : uiScriptLocale.OPTION_SNOW_MODE_OFF,

					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				
				{
					"name" : "font_type_name",
					"type" : "text",

					"x" : 30,
					"y" : 90+2,

					"text" : uiScriptLocale.OPTION_FONT,
				},
				
				{
					"name" : "font_type_arial",
					"type" : "radio_button",

					"x" : 110,
					"y" : 90,

					"text" : "Arial",

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				
				{
					"name" : "font_type_tahoma",
					"type" : "radio_button",

					"x" : 110+70,
					"y" : 90,

					"text" : "Tahoma",

					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
				{
					"name" : "background_text",
					"type" : "text",
					"x" : 30,
					"y" : 115+2,
					"text" : uiScriptLocale.OPTION_BACKGROUND_MASK,
				},
				{
					"name" : "background_object",
					"type" : "toggle_button",
					"x" : 110,
					"y" : 115,
					"text" : uiScriptLocale.OPTION_BACKGROUND_MASK_OBJECT,
					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				{
					"name" : "background_cloud",
					"type" : "toggle_button",
					"x" : 110 + 48,
					"y" : 115,
					"text" : uiScriptLocale.OPTION_BACKGROUND_MASK_CLOUD,
					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				{
					"name" : "background_tree",
					"type" : "toggle_button",
					"x" : 110 + 50 + 46,
					"y" : 115,
					"text" : uiScriptLocale.OPTION_BACKGROUND_MASK_TREE,
					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				{
					"name" : "background_water",
					"type" : "toggle_button",
					"x" : 110 + 50 + 50 + 44,
					"y" : 115,
					"text" : uiScriptLocale.OPTION_BACKGROUND_MASK_WATER,
					"default_image" : ROOT_PATH + "small_Button_01.sub",
					"over_image" : ROOT_PATH + "small_Button_02.sub",
					"down_image" : ROOT_PATH + "small_Button_03.sub",
				},
				{
					"name" : "viewcostume_on_off",
					"type" : "text",

					"x" : 40 + TEXT_TEMPORARY_X,
					"y" : 140+2,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME,
				},
				{
					"name" : "viewcostume_on_button",
					"type" : "radio_button",

					"x" : 110,
					"y" : 140,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "viewcostume_off_button",
					"type" : "radio_button",

					"x" : 110 + 50 + 25,
					"y" : 140,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "viewcostumew_on_off",
					"type" : "text",

					"x" : 40 + TEXT_TEMPORARY_X,
					"y" : 165+2,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME_W,
				},
				{
					"name" : "viewcostumew_on_button",
					"type" : "radio_button",

					"x" : 110,
					"y" : 165,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME_W_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "viewcostumew_off_button",
					"type" : "radio_button",

					"x" : 110 + 50 + 25,
					"y" : 165,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME_W_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "viewcostumeh_on_off",
					"type" : "text",

					"x" : 40 + TEXT_TEMPORARY_X,
					"y" : 190+2,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME_H,
				},
				{
					"name" : "viewcostumeh_on_button",
					"type" : "radio_button",

					"x" : 110,
					"y" : 190,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME_H_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "viewcostumeh_off_button",
					"type" : "radio_button",

					"x" : 110 + 50 + 25,
					"y" : 190,

					"text" : uiScriptLocale.OPTION_VIEW_COSTUME_H_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "sash_on_off",
					"type" : "text",

					"x" : 40 + TEXT_TEMPORARY_X,
					"y" : 215+2,

					"text" : uiScriptLocale.OPTION_SASH,
				},
				{
					"name" : "sash_on_button",
					"type" : "radio_button",

					"x" : 110,
					"y" : 215,

					"text" : uiScriptLocale.OPTION_SASH_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "sash_off_button",
					"type" : "radio_button",

					"x" : 110 + 50 + 25,
					"y" : 215,

					"text" : uiScriptLocale.OPTION_SASH_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "level_on_off",
					"type" : "text",

					"x" : 40 + TEXT_TEMPORARY_X,
					"y" : 240+2,

					"text" : uiScriptLocale.OPTION_LEVEL,
				},
				{
					"name" : "viewlevel_on_button",
					"type" : "radio_button",

					"x" : 110,
					"y" : 240,

					"text" : uiScriptLocale.OPTION_LEVEL_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "viewlevel_off_button",
					"type" : "radio_button",

					"x" : 110 + 50 + 25,
					"y" : 240,

					"text" : uiScriptLocale.OPTION_LEVEL_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "highlight_on_off",
					"type" : "text",

					"x" : 40 + TEXT_TEMPORARY_X,
					"y" : 265+2,

					"text" : uiScriptLocale.OPTION_ITEM_HIGHLIGHT,
				},
				{
					"name" : "highlight_on_button",
					"type" : "radio_button",

					"x" : 110,
					"y" : 265,

					"text" : uiScriptLocale.OPTION_ITEM_HIGHLIGHT_ON,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
				{
					"name" : "highlight_off_button",
					"type" : "radio_button",

					"x" : 110 + 50 + 25,
					"y" : 265,

					"text" : uiScriptLocale.OPTION_ITEM_HIGHLIGHT_OFF,

					"default_image" : ROOT_PATH + "middle_button_01.sub",
					"over_image" : ROOT_PATH + "middle_button_02.sub",
					"down_image" : ROOT_PATH + "middle_button_03.sub",
				},
			),
		},
	),
}