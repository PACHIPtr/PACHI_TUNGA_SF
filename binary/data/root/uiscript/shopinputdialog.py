import uiScriptLocale

window = {
	"name" : "InputDialog",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : 205,
	"height" : 200,

	"children" :
	(
		{
			"name" : "Board",
			"type" : "board_with_titlebar",

			"x" : 0,
			"y" : 0,

			"width" : 205,
			"height" : 200,

			"title" : "",

			"children" :
			(

				## Input Slot
				{
					"name" : "InputSlot",
					"type" : "slotbar",

					"x" : 0,
					"y" : 40,
					"width" : 90,
					"height" : 18,
					"horizontal_align" : "center",

					#"type" : "image",
					#"image" : "d:/ymir work/ui/public/Parameter_Slot_03.sub",

					"children" :
					(
						{
							"name" : "InputValue",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 90,
							"height" : 18,

							"input_limit" : 12,
						},
					),
				},

				{
					"name" : "TypeBG", "type" : "image",
					"x" : 5, "y" : 75,
					"image" : "d:/ymir work/yellow_bar.tga"
				},				
				{
					"name" : "SizeBG", "type" : "image",
					"x" : 5, "y" : 102,
					"image" : "d:/ymir work/yellow_bar.tga"
				},
				{
					"name" : "ColorBG", "type" : "image",
					"x" : 5, "y" : 129,
					"image" : "d:/ymir work/yellow_bar.tga"
				},
	

				# Type
				{
					"name" : "TypeDesc", "type" : "text",
					"x" : -122, "y" : 78, "text_horizontal_align" : "center", "horizontal_align" : "center",
					"text" : uiScriptLocale.OFFLINE_SHOP_TYPE,
				},

				## TypeTab
				{
					"name" : "TypeTab1", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_NORMAL_SHOP,
					"x" : 60, "y" : 75, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "TypeTab2", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_6HOURS_SHOP,
					"x" : 100, "y" : 75, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "TypeTab3", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_12HOURS_SHOP,
					"x" : 140, "y" : 75, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "TypeTab4", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_18HOURS_SHOP,
					"x" : 180, "y" : 75, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "TypeTab5", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_24HOURS_SHOP,
					"x" : 220, "y" : 75, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "TypeTab6", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_48HOURS_SHOP,
					"x" : 260, "y" : 75, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				

					# Size
				{
					"name" : "SizeDesc", "type" : "text",
					"x" : -122, "y" : 105, "text_horizontal_align" : "center", "horizontal_align" : "center",
					"text" : uiScriptLocale.OFFLINE_SHOP_SIZE,
				},

				## SizeTab
				{
					"name" : "SizeTab1", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_40SLOT,
					"x" : 60, "y" : 102, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "SizeTab2", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_50SLOT,
					"x" : 100, "y" : 102, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "SizeTab3", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_60SLOT,
					"x" : 140, "y" : 102, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "SizeTab4", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_70SLOT,
					"x" : 180, "y" : 102, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "SizeTab5", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_80SLOT,
					"x" : 220, "y" : 102, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "SizeTab6", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_90SLOT,
					"x" : 260, "y" : 102, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},


				# Color
				{
					"name" : "ColorDesc", "type" : "text",
					"x" : -122, "y" : 132, "text_horizontal_align" : "center", "horizontal_align" : "center",
					"text" : uiScriptLocale.OFFLINE_SHOP_COLOR,
				},

				## ColorTab
				{
					"name" : "ColorTab1", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_BLUE_NPC,
					"x" : 60, "y" : 129, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "ColorTab2", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_GREEN_NPC,
					"x" : 100, "y" : 129, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "ColorTab3", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_YELLOW_NPC,
					"x" : 140, "y" : 129, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "ColorTab4", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_PINK_NPC,
					"x" : 180, "y" : 129, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "ColorTab5", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_CLARET_RED,
					"x" : 220, "y" : 129, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},
				{
					"name" : "ColorTab6", "type" : "radio_button", "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_LIGHT_YELLOW,
					"x" : 260, "y" : 129, "width" : 61, "height" : 21,
					"default_image" : "d:/ymir work/normal.tga", "over_image" : "d:/ymir work/hover.tga", "down_image" : "d:/ymir work/active.tga",
				},

				
				## Button
				{
					"name" : "AcceptButton",
					"type" : "button",

					"x" : - 61 - 5 + 30,
					"y" : 165,
					"horizontal_align" : "center",

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "CancelButton",
					"type" : "button",

					"x" : 5 + 30,
					"y" : 165,
					"horizontal_align" : "center",

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
			),
		},
	),
}