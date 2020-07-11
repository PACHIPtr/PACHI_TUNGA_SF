import uiScriptLocale, localeInfo

SIZE_X = 192
SIZE_Y = 154
NEED_MONEY = 1000000

window = {
	"name" : "SkillBookCombinationWindow",
	"style" : ("movable", "float",),
	"x" : SCREEN_WIDTH/2,
	"y" : SCREEN_HEIGHT/2,	
	"width" : SIZE_X,
	"height" : SIZE_Y,
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),
			"x" : 0,
			"y" : 0,
			"width" : SIZE_X,
			"height" : SIZE_Y,
			"children" :
			(
				## SkillBook Combination Dialog Title
				{
					"name" : "SkillBook_TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),
					"x" : 6,
					"y" : 7,
					"width" : SIZE_X - 13,
					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":0, "text":uiScriptLocale.COMB_SKILLBOOK_TILE, "all_align":"center" },
					),
				},
				## Money
				{
					"name" : "NeedMoney",
					"type" : "text",
					"x" : 0, "y" : SIZE_Y - 48,
					"horizontal_align" : "center",
					"text_horizontal_align" : "center",
					"text" : localeInfo.COMB_COST % (localeInfo.NumberToMoneyString(NEED_MONEY)),
				},
				## Button
				{
					"name" : "accept_button",
					"type" : "button",
					"x" : SIZE_X/2 - 65,
					"y" : SIZE_Y - 30,
					"text" : uiScriptLocale.OK,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "cancel_button",
					"type" : "button",
					"x" : SIZE_X/2 + 5,
					"y" : SIZE_Y - 30,
					"text" : uiScriptLocale.CANCEL,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				## SkillBook Slot Area
				{
					"name" : "SkillBook_Slot_Area",
					"type" : "window",
					"style" : ("attach",),
					"x" : 16,
					"y" : 34,
					"width" : 160, # 32 * 5
					"height" : 64, # 32 * 2
					"children" :
					(
						# SkillBook Slot
						{
							"name" : "SkillBookSlot",
							"type" : "grid_table",
							"x" : 0,
							"y" : 0,
							"start_index" : 0,
							"x_count" : 5,
							"y_count" : 2,
							"x_step" : 32,
							"y_step" : 32,
							"image" : "d:/ymir work/ui/public/Slot_Base.sub"
						},
					),
				}, ## End of SkillBook Slot
			),
		},
	),
}
