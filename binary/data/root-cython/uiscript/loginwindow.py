import uiScriptLocale
import grp

ACCOUNT_MANAGER_START_X = 30
ACCOUNT_MANAGER_X = 30
ACCOUNT_MANAGER_Y = 12

DIFF = 100


window = {
	"name" : "LoginWindow",
	"style" : ("movable",),
	
	"x" : 0,
	"y" : 0,
	
	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,
	
	"children" :
	(
		# Main Background
		{
			"name" : "BackGround",
			"type" : "expanded_image", "x" : 0, "y" : 0,
			"x_scale" : float(SCREEN_WIDTH) / 1920.0,
			"y_scale" : float(SCREEN_HEIGHT) / 1080.0,
			#"horizontal_align" : "center",
			#"vertical_align" : "center", 
			"image" : "d:/ymir work/ui/login_new/arven2/login.jpg",
		},
		
		{
			"name" : "ConnectBoard",
			"type" : "window",
			
			"x" : float(SCREEN_WIDTH) / 1920.0 + 150,
			"y" : 0,
			
			"horizontal_align" : "center",
			"vertical_align" : "center",
			
			"width" : 800,
			"height" : 550,
			
			"children" : 
			(
				{
					"name" : "WebLink",
					"type" : "button",
					
					"x" : 16-45+50-7,
					"y" : 65-25+245,
					
					"horizontal_align" : "center",
					"vertical_align" : "bottom",
					
					"default_image" : "d:/ymir work/ui/login_new/web_btn_default.tga",
					"over_image" : "d:/ymir work/ui/login_new/web_btn_over.tga",
					"down_image" : "d:/ymir work/ui/login_new/web_btn_down.tga",
				},
				{
					"name" : "LoginWrapper",
					"type" : "image",
					
					"x" : 0-50-10,
					"y" : 87-25+100+50+25+3+40,
					
					"image" : "d:/ymir work/ui/login_new/dialogbg.tga",
					
					"children" : 
					(

									
						{
							"name" : "ID_EditLine2",
							"type" : "image",
							
							"x" : 16+31+25,
							"y" : 65-25,
							
							"image" : "d:/ymir work/ui/login_new/user_input.tga",
							
							"children" : 
							(
								{
									"name" : "ID_EditLine",
									"type" : "editline",

									"x" : 50,
									"y" : 8,

									"width" : 194,
									"height" : 22,

									"input_limit" : 16,

									"fontsize" : "LARGE",
									
									"color" : grp.GenerateColor(1.0, 1.0, 0.0, 1.0),
								},
							),
						},
						
						{
							"name" : "Password_EditLine2",
							"type" : "image",
							
							"x" : 16+31+25,
							"y" : 105-25,
							
							"image" : "d:/ymir work/ui/login_new/pw_input.tga",
							
							"children" : 
							(
								{
									"name" : "Password_EditLine",
									"type" : "editline",

									"x" : 50,
									"y" : 8,

									"width" : 194,
									"height" : 22,

									"input_limit" : 16,
									"secret_flag" : 1,

									"fontsize" : "LARGE",
									
									"color" : grp.GenerateColor(1.0, 1.0, 0.0, 1.0),
								},
							),
						},
						
						{
							"name" : "Pin_EditLine2",
							"type" : "image",
							
							"x" : 16+31+25,
							"y" : 145-25,
							
							"image" : "d:/ymir work/ui/login_new/pin_input.tga",
							
							"children" : 
							(
								{
									"name" : "Pin_EditLine",
									"type" : "editline",

									"x" : 50,
									"y" : 8,

									"width" : 194,
									"height" : 22,

									"input_limit" : 4,
									"secret_flag" : 1,

									"fontsize" : "LARGE",
									
									"color" : grp.GenerateColor(1.0, 1.0, 0.0, 1.0),
								},
							),
						},
						
						{
							"name" : "LoginButton",
							"type" : "button",
							
							"x" : 17+30+33+17,
							"y" : 147+40-25+12-15,
							
							"default_image" : "d:/ymir work/ui/login_new/login_btn_down.tga",
							"over_image" : "d:/ymir work/ui/login_new/login_btn_over.tga",
							"down_image" : "d:/ymir work/ui/login_new/login_btn_default.tga",
						},
						
						{
							"name" : "LoginExitButton",
							"type" : "button",
							
							# "x" : 17+140+2-8-5-4,
							# "y" : 147+40-25,
							"x" : 17+140+2-8-5-4+40-9+10+17,
							"y" : 147+40-25+36+20,
							
							"default_image" : "d:/ymir work/ui/login_new/logout_3.tga",
							"over_image" : "d:/ymir work/ui/login_new/logout_1.tga",
							"down_image" : "d:/ymir work/ui/login_new/logout_2.tga",
						},
						
						{
							"name" : "KaydetConnectButton",
							"type" : "button",
							
							"x" : 16+40-9+10+17,
							"y" : 147+40-25+36+20,
							
							"default_image" : "d:/ymir work/ui/login_new/save_btn_3.tga",
							"over_image" : "d:/ymir work/ui/login_new/save_btn_1.tga",
							"down_image" : "d:/ymir work/ui/login_new/save_btn_2.tga",
						},
						
						# {
							# "name" : "AccountDeleteButton",
							# "type" : "button",
							
							# "x" : 17+140+2-8-5-4,
							# "y" : 147+40-25+36,
							
							# "default_image" : "d:/ymir work/ui/login_new/clear_btn_1.tga",
							# "over_image" : "d:/ymir work/ui/login_new/clear_btn_2.tga",
							# "down_image" : "d:/ymir work/ui/login_new/clear_btn_3.tga",
						# },
						
						# {
							# "name" : "AccountDeleteAbort",
							# "type" : "button",
							
							# "x" : 17+140+2-8-5-4,
							# "y" : 147+40-25+36,
							
							# "default_image" : "d:/ymir work/ui/login_new/clear_btn_4.tga",
							# "over_image" : "d:/ymir work/ui/login_new/clear_btn_5.tga",
							# "down_image" : "d:/ymir work/ui/login_new/clear_btn_6.tga",

						# },
						{
							"name" : "Channel1Button",
							"type" : "radio_button",
							
							"x" : 271+83,
							"y" : 65-25-18,
							
							"default_image" : "d:/ymir work/ui/login_new/ch_btn_default.tga",
							"over_image" : "d:/ymir work/ui/login_new/ch_btn_over.tga",
							"down_image" : "d:/ymir work/ui/login_new/ch_btn_down.tga",
							
							"children" : 
							(
								{
									"name" : "ChannelText1",
									"type" : "text",
									
									"x" : 0,
									"y" : 4+5+3,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|cFFffff96|hCH1",
									
									"fontsize" : "LARGE",
								},
								
								{
									"name" : "Channel1Status",
									"type" : "text",
									
									"x" : 0,
									"y" : 23+5,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|Eemoji/offx|e",
									"color" : 0xffb77643,
								},
							),
						},
						
						{
							"name" : "Channel2Button",
							"type" : "radio_button",
							
							"x" : 386+83+10,
							"y" : 65-25-18,
							
							"default_image" : "d:/ymir work/ui/login_new/ch_btn_default.tga",
							"over_image" : "d:/ymir work/ui/login_new/ch_btn_over.tga",
							"down_image" : "d:/ymir work/ui/login_new/ch_btn_down.tga",
							
							"children" : 
							(
								{
									"name" : "ChannelText2",
									"type" : "text",
									
									"x" : 0,
									"y" : 4+5+3,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|cFFffff96|hCH2",
									
									"fontsize" : "LARGE",
									"r":0.0,
									"g":255.0,
									"b":0.0,
								},
								
								{
									"name" : "Channel2Status",
									"type" : "text",
									
									"x" : 0,
									"y" : 23+5,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|Eemoji/offx|e",
									"color" : 0xffb77643,
								},
							),
						},
						
						{
							"name" : "Channel3Button",
							"type" : "radio_button",
							
							"x" : 271+83,
							"y" : 116-25-18,
							
							"default_image" : "d:/ymir work/ui/login_new/ch_btn_default.tga",
							"over_image" : "d:/ymir work/ui/login_new/ch_btn_over.tga",
							"down_image" : "d:/ymir work/ui/login_new/ch_btn_down.tga",
							
							"children" : 
							(
								{
									"name" : "ChannelText3",
									"type" : "text",
									
									"x" : 0,
									"y" : 4+5+3,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|cFFffff96|hCH3",
									
									"fontsize" : "LARGE",
									"r":0.0,
									"g":255.0,
									"b":0.0,
								},
								
								{
									"name" : "Channel3Status",
									"type" : "text",
									
									"x" : 0,
									"y" : 23+5,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|Eemoji/offx|e",
									"color" : 0xffb77643,
								},
							),
						},
						
						{
							"name" : "Channel4Button",
							"type" : "radio_button",
							
							"x" : 386+83+10,
							"y" : 116-25-18,
							
							"default_image" : "d:/ymir work/ui/login_new/ch_btn_default.tga",
							"over_image" : "d:/ymir work/ui/login_new/ch_btn_over.tga",
							"down_image" : "d:/ymir work/ui/login_new/ch_btn_down.tga",
							
							"children" : 
							(
								{
									"name" : "ChannelText4",
									"type" : "text",
									
									"x" : 0,
									"y" : 4+5+3,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|cFFffff96|hCH4",
									
									"fontsize" : "LARGE",
									"r":0.0,
									"g":255.0,
									"b":0.0,
								},
								
								{
									"name" : "Channel4Status",
									"type" : "text",
									
									"x" : 0,
									"y" : 23+5,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|Eemoji/offx|e",
									"color" : 0xffb77643,
								},
							),
						},
						
						{
							"name" : "Channel5Button",
							"type" : "radio_button",
							
							"x" : 271+83,
							"y" : 168-25-18,
							
							"default_image" : "d:/ymir work/ui/login_new/ch_btn_default.tga",
							"over_image" : "d:/ymir work/ui/login_new/ch_btn_over.tga",
							"down_image" : "d:/ymir work/ui/login_new/ch_btn_down.tga",
							
							"children" : 
							(
								{
									"name" : "ChannelText5",
									"type" : "text",
									
									"x" : 0,
									"y" : 4+5+3,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|cFFffff96|hCH5",
									
									"fontsize" : "LARGE",
									"r":0.0,
									"g":255.0,
									"b":0.0,
								},
								
								{
									"name" : "Channel5Status",
									"type" : "text",
									
									"x" : 0,
									"y" : 23+5,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|Eemoji/offx|e",
									"color" : 0xffb77643,
								},
							),
						},
						
						{
							"name" : "Channel6Button",
							"type" : "radio_button",
							
							"x" : 386+83+10,
							"y" : 168-25-18,
							
							"default_image" : "d:/ymir work/ui/login_new/ch_btn_default.tga",
							"over_image" : "d:/ymir work/ui/login_new/ch_btn_over.tga",
							"down_image" : "d:/ymir work/ui/login_new/ch_btn_down.tga",
							
							"children" : 
							(
								{
									"name" : "ChannelText6",
									"type" : "text",
									
									"x" : 0,
									"y" : 4+5+3,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|cFFffff96|hCH6",
									
									"fontsize" : "LARGE",
									"r":170.0,
									"g":255.0,
									"b":86.0,
								},
								
								{
									"name" : "Channel6Status",
									"type" : "text",
									
									"x" : 0,
									"y" : 23+5,
									
									"text_horizontal_align" : "center",
									"horizontal_align" : "center",
									
									"text" : "|Eemoji/offx|e",
									"color" : 0xffb77643,
								},
							),
						},
						{
							"name": "K4Button",
							"type": "button",


							"x" : 271+2+6+85+130-25+17,
							"y" : 168-25+47+24-5+10 + 15,

							"default_image" : "d:/ymir work/ui/login_new/acc1.tga",
							"over_image" : "d:/ymir work/ui/login_new/acc2.tga",
							"down_image" : "d:/ymir work/ui/login_new/acc3.tga",

							"text": "",
						},
						{	
							"name": "K3Button",
							"type": "button",

							"x" : 271+2+6+85-25+17,
							"y" : 168-25+47+24-5+10  + 15,

							"default_image" : "d:/ymir work/ui/login_new/acc1.tga",
							"over_image" : "d:/ymir work/ui/login_new/acc2.tga",
							"down_image" : "d:/ymir work/ui/login_new/acc3.tga",

							"text": "",
						},
						{	
							"name": "K2Button",
							"type": "button",


							"x" : 271+2+6+85+130-25+17,
							"y" : 168-25+47-10-5+10,
							"default_image" : "d:/ymir work/ui/login_new/acc1.tga",
							"over_image" : "d:/ymir work/ui/login_new/acc2.tga",
							"down_image" : "d:/ymir work/ui/login_new/acc3.tga",

							"text": "",
						},
						{
							"name": "K1Button",
							"type": "button",

							"x" : 271+2+6+85-25+17,
							"y" : 168-25+47-10-5+10,
							"default_image" : "d:/ymir work/ui/login_new/acc1.tga",
							"over_image" : "d:/ymir work/ui/login_new/acc2.tga",
							"down_image" : "d:/ymir work/ui/login_new/acc3.tga",

							"text": "",
						},
						{
							"name": "S1Button",
							"type": "button",

							"x" : 271+2+6+85-25+97+17,
							"y" : 168-25+47-10-5+4+10,

							"default_image" : "d:/ymir work/ui/login_new/delete_acc_1.tga",
							"over_image" : "d:/ymir work/ui/login_new/delete_acc_2.tga",
							"down_image" : "d:/ymir work/ui/login_new/delete_acc_3.tga",

							"text": "",
						},
						
						{
							"name": "S2Button",
							"type": "button",


							"x" : 271+2+6+85+130-25+97+17,
							"y" : 168-25+47-10-5+4+10,

							"default_image" : "d:/ymir work/ui/login_new/delete_acc_1.tga",
							"over_image" : "d:/ymir work/ui/login_new/delete_acc_2.tga",
							"down_image" : "d:/ymir work/ui/login_new/delete_acc_3.tga",

							"text": "",
						},
						{
							"name": "S3Button",
							"type": "button",


							"x" : 271+2+6+85-25+97+17,
							"y" : 168-25+47+24-5+5+10  + 15,

							"default_image" : "d:/ymir work/ui/login_new/delete_acc_1.tga",
							"over_image" : "d:/ymir work/ui/login_new/delete_acc_2.tga",
							"down_image" : "d:/ymir work/ui/login_new/delete_acc_3.tga",

							"text": "",
						},
						
						{
							"name": "S4Button",
							"type": "button",


							"x" : 271+2+6+85+130-25+97+17,
							"y" : 168-25+47+24-5+5+10  + 15,
							
							"default_image" : "d:/ymir work/ui/login_new/delete_acc_1.tga",
							"over_image" : "d:/ymir work/ui/login_new/delete_acc_2.tga",
							"down_image" : "d:/ymir work/ui/login_new/delete_acc_3.tga",

							"text": "",
						},

					),
					
				},
				

				

				# {
					# "name" : "TwitchBtn",
					# "type" : "button",
					
					# "x" : 0,
					# "y" : 54,
					
					# "horizontal_align" : "center",
					# "vertical_align" : "bottom",
					
					# "default_image" : "d:/ymir work/ui/login_new/twitch_btn_default.tga",
					# "over_image" : "d:/ymir work/ui/login_new/twitch_btn_over.tga",
					# "down_image" : "d:/ymir work/ui/login_new/twitch_btn_down.tga",
				# },
				
				# {
					# "name" : "DiscordBtn",
					# "type" : "button",
					
					# "x" : 45,
					# "y" : 54,
					
					# "horizontal_align" : "center",
					# "vertical_align" : "bottom",
					
					# "default_image" : "d:/ymir work/ui/login_new/discord_btn_default.tga",
					# "over_image" : "d:/ymir work/ui/login_new/discord_btn_over.tga",
					# "down_image" : "d:/ymir work/ui/login_new/discord_btn_down.tga",
				# },
			),
		},
	),
}