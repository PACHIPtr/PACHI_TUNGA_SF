import uiScriptLocale

WINDOW_WIDTH	= 288
WINDOW_HEIGHT	= 208

NORMAL_SLOT_FILE  = "d:/ymir work/ui/minigame/fish_event/fish_normal_slot.sub"
SPECIAL_SLOT_FILE = "d:/ymir work/ui/minigame/fish_event/fish_special_slot.sub"

WHITE_COLOR = 0xFFFFFFFF

window = {
	"name" : "MiniGameFishEventGamePage",
	"style" : ("movable", "float",),
	
	"x" : SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2,
	"y" : SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2,
	
	"width" : WINDOW_WIDTH,
	"height" : WINDOW_HEIGHT,
	
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),
			
			"x" : 0,
			"y" : 0,
			
			"width" : WINDOW_WIDTH,
			"height" : WINDOW_HEIGHT,
			
			"children" :
			(				
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 0,
					"y" : 0,

					"width" : WINDOW_WIDTH,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":0, "text": uiScriptLocale.MINI_GAME_FISH_TITLE, "all_align":"center" },
					),
				},
				
				## Game Slot Img
				{
					"name" : "GameSlotImg",
					"type" : "expanded_image",
					"style" : ("attach",),
					
					"x" : 14,
					"y" : 30,

					"image" : "d:/ymir work/ui/minigame/fish_event/fish_grid_slot.sub",
				},
				## Game Slot
				{
					"name" : "GameSlot",
					"type" : "grid_table",

					"x" : 18,
					"y" : 34,

					"start_index" : 0,
					"x_count" : 6,
					"y_count" : 4,
					"x_step" : 32,
					"y_step" : 32,

					#"image" : "d:/ymir work/ui/public/slot_base.sub",
				},
				## Special Item Slot BG
				{ 
					"name" : "SpecialItemSlotBG", 
					"type" : "expanded_image", 
					"style" : ("attach",), 
					"x" : 225, 
					"y" : 54, 
					"image" : SPECIAL_SLOT_FILE,
				},
				## Special Item Slot
				{
					"name" : "SpecialItemSlot",
					"type" : "slot",

					"x" : 225+7,
					"y" : 54+7,
					"width" : 32,
					"height" : 32,
					
					"slot" : ({"index":0, "x":0, "y":0, "width":32, "height":32,},),
				},
				## Normal Item Slot BG
				{ 
					"name" : "NormalItemSlotBG", 
					"type" : "expanded_image", 
					"style" : ("attach",), 
					"x" : 225, 
					"y" : 105, 
					"image" : NORMAL_SLOT_FILE,
				},
				## Normal Item Slot
				{
					"name" : "NormalItemSlot",
					"type" : "slot",

					"x" : 225+7,
					"y" : 105+7,
					"width" : 32,
					"height" : 32,
					
					"slot" : ({"index":0, "x":0, "y":0, "width":32, "height":32,},),
				},
				{
					"name" : "help_button",
					"type" : "button",
					
					"x" : 15,
					"y" : 177,
					
					"text" : uiScriptLocale.MINI_GAME_FISH_HELP_TEXT,					
					
					"default_image" : "d:/ymir work/ui/public/large_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/large_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/large_button_03.sub",
				},
				
				{
					"name" : "count_text",
					"type" : "text",

					"x" : 166,
					"y" : 182,

					"text" : uiScriptLocale.MINIGAME_FISH_EVENT_USE_COUNT,
				},
				{
					"name" : "use_count_area",
					"type" : "expanded_image",
					"style" : ("attach",),
					
					"x" : 219,
					"y" : 178,

					"image" : "d:/ymir work/ui/minigame/fish_event/fish_use_count_area.sub",
				},
				{ 
					"name" : "use_count_window", "type" : "window", "x" : 219, "y" : 178, "width" : 51, "height" : 18, "style" : ("attach",),
					"children" :
					(
						{"name":"use_count_text", "type":"text", "x":0, "y":0, "text": "", "color":WHITE_COLOR, "all_align" : "center"},
					),	
				},
				
				{
					"name" : "score_completion_effect1",
					"type" : "ani_image",
					
					"x" : -5,
					"y" : 23,
					
					"delay" : 6,

					"images" :
					(
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff1.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff2.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff3.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff4.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff7.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff8.sub",
					),
				},
				
				{
					"name" : "score_completion_effect2",
					"type" : "ani_image",
					
					"x" : 38,
					"y" : 23,
					
					"delay" : 6,

					"images" :
					(
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff1.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff2.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff3.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff4.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff7.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff8.sub",
					),
				},
				
				{
					"name" : "score_completion_effect3",
					"type" : "ani_image",
					
					"x" : 81,
					"y" : 23,
					
					"delay" : 6,

					"images" :
					(
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff1.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff2.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff3.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff4.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff7.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_eff8.sub",
					),
				},
				{
					"name" : "score_completion_text_effect",
					"type" : "ani_image",
					
					"x" : 29,
					"y" : 56,
					
					"delay" : 0,

					"images" :
					(
						
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect1.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect5.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect6.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect7.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect8.sub",
						"D:/Ymir Work/UI/minigame/rumi/card_completion_effect/card_completion_text_effect9.sub",
					),
				},
			),
		},
	),	
}
