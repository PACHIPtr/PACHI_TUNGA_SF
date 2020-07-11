import uiScriptLocale
import item
import uiScriptLocale, constInfo

WIDTH = 160+14
HEIGHT = 290+14

X=9+7-5-4+6
Y=7+3-3
EK_X=0
EK_Y=27

Button5_IMG_1="icon/menu/ticcami.png"
Button5_IMG_2="icon/menu/ticcami2.png"
Button5_IMG_3="icon/menu/ticcami3.png"

Button2_IMG_1="icon/menu/dungeoninfo.png"
Button2_IMG_2="icon/menu/dungeoninfo2.png"
Button2_IMG_3="icon/menu/dungeoninfo3.png"

Button3_IMG_1="icon/menu/bosstakip.png"
Button3_IMG_2="icon/menu/bosstakip2.png"
Button3_IMG_3="icon/menu/bosstakip3.png"

Button6_IMG_1="icon/menu/lonca.png"
Button6_IMG_2="icon/menu/lonca2.png"
Button6_IMG_3="icon/menu/lonca3.png"

Button4_IMG_1="icon/menu/efsunbotu.png"
Button4_IMG_2="icon/menu/efsunbotu2.png"
Button4_IMG_3="icon/menu/efsunbotu3.png"

Button1_IMG_1="icon/menu/uzakmarket.png"
Button1_IMG_2="icon/menu/uzakmarket2.png"
Button1_IMG_3="icon/menu/uzakmarket3.png"

window = {
	"name" : "SystemsWindow",

	"x" : SCREEN_WIDTH - 190 - 148, #176
	"y" : SCREEN_HEIGHT - 55 - 565 + 209 + 32 - 37, #37
	
	"width" : 148 -91, #148
	"height" : 155+35+30, #139

	"type" : "window",

	"children" :
	(
		
		# {
			# "name" : "ExpandBtn",
			# "type" : "button",

			# "x" : 2,
			# "y" : 15,

			# "default_image" : str(constInfo.CONFIG_YOL)+"btn_expand_normal.tga",
			# "over_image" : str(constInfo.CONFIG_YOL)+"btn_expand_over.tga",
			# "down_image" : str(constInfo.CONFIG_YOL)+"btn_expand_down.tga",
			# "disable_image" : str(constInfo.CONFIG_YOL)+"btn_expand_disabled.tga",
		# },
		
		{
			"name" : "SystemsWindowLayer",

			"x" : 5,
			"y" : 0,

			"width" : 148 - 91,
			"height" : 180+50+30, #139

			"children" :
			(
				
				# {
					# "name" : "MinimizeBtn",
					# "type" : "button",

					# "x" : 0,
					# "y" : 15+12,

					# "width" : 10,

					# "default_image" : str(constInfo.CONFIG_YOL)+"btn_minimize_normal.tga",
					# "over_image" : str(constInfo.CONFIG_YOL)+"btn_minimize_over.tga",
					# "down_image" : str(constInfo.CONFIG_YOL)+"btn_minimize_down.tga",
					# "disable_image" : str(constInfo.CONFIG_YOL)+"btn_minimize_disabled.tga",
				# },

				{
					"name" : "SystemsWindowBoard",
					#"type" : "board",
					#"style" : ("attach", "float"),

					"x" : 10,
					"y" : 0,

					"width" : 148 - 9991-800,
					"height" : 172+27+30-6, #172

					"children" :
					(
						{
							"name" : "shop_button",
							"type" : "button",

							"x" : X,
							"y" : 8,
							

							"default_image" : Button1_IMG_1,
							"over_image" : Button1_IMG_2,
							"down_image" : Button1_IMG_3,
						},
						{
							"name" : "ek_envanter",
							"type" : "button",

							"x" : X,
							"y" : 8+34,
							

							"default_image" : Button2_IMG_1,
							"over_image" : Button2_IMG_2,
							"down_image" : Button2_IMG_3,
						},
						{
							"name" : "uzak_market",
							"type" : "button",

							"x" : X,
							"y" : 8+34*2,
							

							"default_image" : Button3_IMG_1,
							"over_image" : Button3_IMG_2,
							"down_image" : Button3_IMG_3,
						},
						{
							"name" : "bgm_button",
							"type" : "button",

							"x" : X,
							"y" : 8+34*3,
							
							"default_image" : Button4_IMG_1,
							"over_image" : Button4_IMG_2,
							"down_image" : Button4_IMG_3,
						},
						{
							"name" : "bgm_button2",
							"type" : "button",

							"x" : X,
							"y" : 8+34*4,
							
							"default_image" : Button5_IMG_1,
							"over_image" : Button5_IMG_2,
							"down_image" : Button5_IMG_3,
						},
						{
							"name" : "lonca_siralama",
							"type" : "button",

							"x" : X,
							"y" : 8+34*5,
							
							"default_image" : Button6_IMG_1,
							"over_image" : Button6_IMG_2,
							"down_image" : Button6_IMG_3,
						},
					),
				},
			)
		},

	),
}
