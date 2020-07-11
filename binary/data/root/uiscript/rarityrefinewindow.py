import uiScriptLocale
PET_UI_ROOT = "d:/ymir work/ui/pet/"
PET_UI_BG_WIDTH	= 352
PET_UI_BG_HEIGHT = 450
BOARD_WIDTH = 352
BOARD_HEIGH = 450
GOLD_COLOR	= 0xFFFEE3AE
WHITE_COLOR = 0xFFFFFFFF
window = {
	"name" : "RarityRefineWindow",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGH,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGH,
			
			"title" : "Güncelleme Notlarý",
		
			"children" :
			(
				{
					"name" : "PetAttrChangeUIBG",
					"type" : "expanded_image",
					"style" : ("attach",),
					"x" : 0, "y" : 0,
					"image" : "d:/ymir work/ui/pet/res/rarity_refine.png",
				},
				
				## Pet Information Title
				{ 
					"name" : "TitleAttrChangeWindow", "type" : "window", "x" : 10, "y" : 10, "width" : PET_UI_BG_WIDTH-10-15, "height" : 15, "style" : ("attach",),
					"children" :
					(
						{"name":"TitleName", "type":"text", "x":0, "y":0, "text":"Eritme Penceresi", "all_align" : "center"},
					),	
				},
				
				## Close Button
				{ 
					"name" : "CloseButton", 
					"type" : "button", 
					"x" : PET_UI_BG_WIDTH -10-15, 
					"y" : 10, 
					"tooltip_text" : uiScriptLocale.CLOSE, 
					"default_image" : "d:/ymir work/ui/public/close_button_01.sub",	
					"over_image" : "d:/ymir work/ui/public/close_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/close_button_03.sub",
				},
				
				## UpBringing Pet Item Slot
				{
					"name" : "RefineR_Slot",
					"type" : "slot",
					"x" : 78,
					"y" : 65,
					"width" : 197+50,
					"height" : 302,
					
					"slot" : (
						{"index":0, "x":82, "y":2, "width":32, "height":96},
						{"index":1, "x":53, "y":133, "width":32, "height":32},
						{"index":2, "x":106, "y":133, "width":32, "height":32},
						{"index":3, "x":82, "y":206, "width":32, "height":96},
					),
				},
				## Pet Change Button
				{
					"name" : "RefineR_Change_Button",
					"type" : "button",

					"x" : 120,
					"y" : 412,

					"default_image" : PET_UI_ROOT + "feed_button/feed_button_default.sub",
					"over_image" : PET_UI_ROOT + "feed_button/feed_button_over.sub",
					"down_image" : PET_UI_ROOT + "feed_button/feed_button_down.sub",
			
					"text" : "Evrim Ýþle",
					"text_color" : GOLD_COLOR,
				},
				
				## Pet OK Button
				{
					"name" : "RefineR_OK_Button",
					"type" : "button",

					"x" : 120,
					"y" : 412,

					"default_image" : PET_UI_ROOT + "feed_button/feed_button_default.sub",
					"over_image" : PET_UI_ROOT + "feed_button/feed_button_over.sub",
					"down_image" : PET_UI_ROOT + "feed_button/feed_button_down.sub",
			
					"text" : "Tamam",
					"text_color" : GOLD_COLOR,
				},
				{
					"name" : "bilgi",
					"type" : "button",

					"x" : PET_UI_BG_WIDTH -10-15-20,
					"y" : 8,

					"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
					"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
					"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
				},
								
				## Ot bilgi
				{ 
					"name" : "Pet_Determine_Info_Title", "type" : "window", "x" : 142, "y" : 246, "width" : 70, "height" : 11, "style" : ("attach",),
					"children" :
					(
						{"name":"otBilgiText", "type":"text", "x":0, "y":0, "text": "otbilgi", "color":GOLD_COLOR, "all_align" : "center"},
					),	
				},
				
				## Sonuç
				{ 
					"name" : "Attr_Change_Result_Text", "type" : "window", "x" : 166, "y" : 383, "width" : 23, "height" : 11, "style" : ("attach",),
					"children" :
					(
						{"name":"AttrChangeResultText", "type":"text", "x":0, "y":0, "text": uiScriptLocale.SASH_RESULT, "color":GOLD_COLOR, "all_align" : "center"},
					),	
				},
			),
		},
	),
}
