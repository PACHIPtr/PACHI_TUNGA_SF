import uiScriptLocale
import localeInfo

BUTTON_ROOT = "d:/ymir work/ui/public/"

window = {
	"name" : "CrystalDungeonCubeWindow",
	
	"style" : ("movable", "float",),
	
	"x"		: 0,
	"y"		: 0,
	
	"width" : 352,
	"height" : 416,
	
	"children" : 
	(
		{
			"name" 		: "main_board",
			"type" 		: "expanded_image",
			
			"style" 	: ("attach",),
			
			"x"			: 0,
			"y"			: 0,
			
			"width"		: 352,
			"height" 	: 416,
			
			"image"		: "d:/ymir work/ui/crystal_new_cube.tga",
			
			"children" : 
			(
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),
					
					"x" : 5,
					"y" : 5,
					
					"width" : 340,
					"color" : "yellow",
					
					"children" : 
					(
						{ "name" : "TitleName", "type" : "text", "x" : 169, "y" : 3, "text" : uiScriptLocale.CUBE_EX_WINDOW_TITLE, "text_horizontal_align" : "center" },
					),
				},
				
				{
					"name" : "LeftSlot",
					"type" : "slot",
					
					"x" : 33,
					"y" : 100,
					
					"width" : 190,
					"height" : 40,
					"slot" : (
						{ "index" : 0, "x" : 20, "y" : 12, "width" : 32, "height" : 32 },
					),
				},
				
				{
					"name" : "RightSlot",
					"type" : "slot",
					
					"x" : 250,
					"y" : 100,
					
					"width" : 190,
					"height" : 40,
					"slot" : (
						{ "index" : 6, "x" : 20, "y" : 12, "width" : 32, "height" : 32 },
					),
				},
				
				{
					"name" : "MaterialSlot1",
					"type" : "slot",
					
					"x" : 160,
					"y" : 85,
					
					"width" : 32,
					"height" : 32,
					
					"slot" : (
						{ "index" : 1, "x" : 0, "y" : 0, "width" : 32, "height" : 32 },
					),
				},
				
				{
					"name" : "MaterialSlot2",
					"type" : "slot",
					
					"x" : 115,
					"y" : 125,
					
					"width" : 32,
					"height" : 32,
					
					"slot" : (
						{ "index" : 2, "x" : 0, "y" : 0, "width" : 32, "height" : 32 },
					),
				},					
				
				{
					"name" : "MaterialSlot3",
					"type" : "slot",
					
					"x" : 160,
					"y" : 125,
					
					"width" : 32,
					"height" : 32,
					
					"slot" : (
						{ "index" : 3, "x" : 0, "y" : 0, "width" : 32, "height" : 32 },
					),
				},	
				
				{
					"name" : "MaterialSlot4",
					"type" : "slot",
					
					"x" : 205,
					"y" : 125,
					
					"width" : 32,
					"height" : 32,
					
					"slot" : (
						{ "index" : 4, "x" : 0, "y" : 0, "width" : 32, "height" : 32 },
					),
				},					

				{
					"name" : "MaterialSlot5",
					"type" : "slot",
					
					"x" : 160,
					"y" : 170,
					
					"width" : 32,
					"height" : 32,
					
					"slot" : (
						{ "index" : 5, "x" : 0, "y" : 0, "width" : 32, "height" : 32 },
					),
				},					
				
				{
					"name" : "MoneyText",
					"type" : "text",
					
					"x" : 120,
					"y" : 250,
					
					"width" : 200,
					
					"horizontal_align" : "right",
					"text_horizontal_align" : "right",

					"text" : localeInfo.NumberToMoneyString(0),
				},
				
				##LeftButton
				{
					"name" : "LButton",
					"type" : "button",

					"x" : 55,
					"y" : 380,
					
					"default_image" : BUTTON_ROOT+"AcceptButton00.sub",
					"over_image" : BUTTON_ROOT+"AcceptButton01.sub",
					"down_image" : BUTTON_ROOT+"AcceptButton02.sub",
				},
				
				##RightButton
				{
				    "name" : "RButton",
				    "type" : "button",
				    
				    "x" : 235,
				    "y" : 380,
					
					"default_image" : BUTTON_ROOT+"CancleButton00.sub",
					"over_image" : BUTTON_ROOT+"CancleButton01.sub",
					"down_image" : BUTTON_ROOT+"CancleButton02.sub",				    
				},
				
				{
					"name" : "OpenListButton",
					"type" : "button",
					
					"x" : 135,
					"y" : 380,
					
					"text" : uiScriptLocale.CUBE_EX_OPEN_LIST_BUTTON,
					
					"default_image" : "d:/ymir work/ui/boss_follow/boss_floow_clear_button_002.tga",
					"over_image" : "d:/ymir work/ui/boss_follow/boss_floow_clear_button_001.tga",
					"down_image" : "d:/ymir work/ui/boss_follow/boss_floow_clear_button_002.tga",
				},				
				
				{
					"name" : "MainBoardText",
					"type" : "text",
					
					"x" : 155,
					"y" : 40,
					
					"text" : uiScriptLocale.CUBE_EX_MAIN_BOARD_WINDOW_TITLE,
				},
				
				{
					"name" : "SubBoardText",
					"type" : "text",
					
					"x" : 115,
					"y" : 290,
					
					"text" : uiScriptLocale.CUBE_EX_SUBBOARD_WINDOW_TITLE,
				},
				
				{
					"name" : "InsideBoardText1",
					"type" : "text",
					
					"x" : 20,
					"y" : 315,
					
					"text" : uiScriptLocale.CUBE_EX_SUBBOARD_INSIDE_TEXT1,
				},
				
				{
					"name" : "InsideBoardText2",
					"type" : "text",
					
					"x" : 20,
					"y" : 330,
					
					"text" : uiScriptLocale.CUBE_EX_SUBBOARD_INSIDE_TEXT2,
				},	

				{
					"name" : "InsideBoardText2",
					"type" : "text",
					
					"x" : 20,
					"y" : 345,
					
					"text" : uiScriptLocale.CUBE_EX_SUBBOARD_INSIDE_TEXT3,
				},

				{
					"name" : "ToolTipButton",
					"type" : "button",

					"x" : 306,
					"y" : 7,

					"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
					"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
					"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
				},
				
				{
					"name" : "InformButton",
					"type" : "button",

					"x" : 290,
					"y" : 7,

					"default_image" : "d:/ymir work/ui/button_new/information_01.tga",
					"over_image" : "d:/ymir work/ui/button_new/information_02.tga",
					"down_image" : "d:/ymir work/ui/button_new/information_01.tga",
				},				
			),
		},
	),
}