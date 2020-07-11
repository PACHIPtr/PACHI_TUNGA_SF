import uiScriptLocale
ROOT_PATH = "d:/ymir work/ui/public/"
window = {
	"name" : "ExtendedInventoryWindow",
	"x" : SCREEN_WIDTH - 176 - 287 - 10,
	"y" : SCREEN_HEIGHT - 37 - 525,
	"style" : ("movable", "float",),
	"width" : 176,
	"height" : 395+10+30+25,
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"x" : 0,
			"y" : 0,
			"width" : 176,
			"height" : 395+10+30+25,
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),
					"x" : 8,
					"y" : 7,
					"width" : 161,
					"color" : "yellow",
					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":77, "y":3, "text":uiScriptLocale.EXTEND_INVENTORY_TITLE, "text_horizontal_align":"center" },
					),
				},
				
				{
					"name" : "BlackBoard",
					"type" : "thinboard_circle",
					"x" : 7,
					"y" : 47+33+150+100+25-5,
					"width" : 162,
					"height" : 42+60,
				},
				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",
					"x" : 8,
					"y" : 35,
					"start_index" : 0,
					"x_count" : 5,
					"y_count" : 9,
					"x_step" : 32,
					"y_step" : 32,
					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},
				
				{
					"name" : "Inventory_Tab_01",
					"type" : "radio_button",

					"x" : 10 +2,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_1,

					"children" :
					(
						{
							"name" : "Inventory_Tab_01_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "I",
						},
					),
				},
				{
					"name" : "Inventory_Tab_02",
					"type" : "radio_button",

					"x" : 10 + 30 +2,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2,

					"children" :
					(
						{
							"name" : "Inventory_Tab_02_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "II",
						},
					),
				},
				{
					"name" : "Inventory_Tab_03",
					"type" : "radio_button",

					"x" : 10 + 60 + 2,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_3,

					"children" :
					(
						{
							"name" : "Inventory_Tab_03_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "III",
						},
					),
				},
				{
					"name" : "Inventory_Tab_04",
					"type" : "radio_button",

					"x" : 10 + 90 + 2,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_4,

					"children" :
					(
						{
							"name" : "Inventory_Tab_04_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "IV",
						},
					),
				},
				{
					"name" : "Inventory_Tab_05",
					"type" : "radio_button",

					"x" : 10 + 120 + 2,
					"y" : 328,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_5,

					"children" :
					(
						{
							"name" : "Inventory_Tab_05_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "V",
						},
					),
				},

				## Button
				{
					"name" : "SkillBookButton",
					"type" : "radio_button",
					"x" : 15,
					"y" : 47+23+35,
					"tooltip_text" : uiScriptLocale.EXTEND_INVENTORY_SKILL_BOOK,
					"vertical_align" : "bottom",
					"default_image" : "d:/ymir work/ui/game/special_inventory/bk_env_1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/bk_env_2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/bk_env_3.tga",
				},
				{
					"name" : "UpgradeItemsButton",
					"type" : "radio_button",
					"x" : 15 + 52,
					"y" : 47+23+35,
					"tooltip_text" : uiScriptLocale.EXTEND_INVENTORY_UPGRADE_ITEMS,
					"vertical_align" : "bottom",
					"default_image" : "d:/ymir work/ui/game/special_inventory/yuk_env_1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/yuk_env_2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/yuk_env_3.tga",
				},
				{
					"name" : "StoneButton",
					"type" : "radio_button",
					"x" : 15 + 104,
					"y" : 47+23+35,
					"tooltip_text" : uiScriptLocale.EXTEND_INVENTORY_STONE_TOOLTIP,
					"vertical_align" : "bottom",
					"default_image" : "d:/ymir work/ui/game/special_inventory/tas_env_1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/tas_env_2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/tas_env_3.tga",
				},
				{
					"name" : "ChestButton",
					"type" : "radio_button",
					"x" : 15,
					"y" : 47+23,
					"tooltip_text" : uiScriptLocale.EXTEND_INVENTORY_CHEST_TOOLTIP,
					"vertical_align" : "bottom",
					"default_image" : "d:/ymir work/ui/game/special_inventory/sandik1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/sandik2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/sandik3.tga",
				},
				{
					"name" : "AttrButton",
					"type" : "radio_button",
					"x" : 15 + 52,
					"y" : 47+23,
					"tooltip_text" : uiScriptLocale.EXTEND_INVENTORY_ATTR_TOOLTIP,
					"vertical_align" : "bottom",
					"default_image" : "d:/ymir work/ui/game/special_inventory/efsun_env1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/efsun_env2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/efsun_env3.tga",
				},
				{
					"name" : "FlowerButton",
					"type" : "radio_button",
					"x" : 15 + 104,
					"y" : 47+23,
					"tooltip_text" : uiScriptLocale.EXTEND_INVENTORY_FLOWER_TOOLTIP,
					"vertical_align" : "bottom",
					"default_image" : "d:/ymir work/ui/game/special_inventory/cicek_env1.tga",
					"over_image" : "d:/ymir work/ui/game/special_inventory/cicek_env2.tga",
					"down_image" : "d:/ymir work/ui/game/special_inventory/cicek_env3.tga",
				},
				{
					"name" : "ExtendInventoryToolTIpButton",
					"type" : "button",

					"x" : 130,
					"y" : 8,

					"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
					"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
					"down_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
				},	
				{
					"name" : "inventory_open_on",
					"type" : "radio_button",
					"x" : 20+5,
					"y" : 47+33+150+100+25-5+30+12+8+30,
					"tooltip_text" : uiScriptLocale.EXTENDED_INVEN_CHECK_ON,
					"default_image" : ROOT_PATH + "acceptbutton00.sub",
					"over_image" : ROOT_PATH + "acceptbutton01.sub",
					"down_image" : ROOT_PATH + "acceptbutton02.sub",
				},
				{
					"name" : "inventory_open_of",
					"type" : "radio_button",
					"x" : 90+5,
					"y" : 47+33+150+100+25-5+30+12+8+30,
					"tooltip_text" : uiScriptLocale.EXTENDED_INVEN_CHECK_OFF,
					"default_image" : ROOT_PATH + "canclebutton00.sub",
					"over_image" : ROOT_PATH + "canclebutton01.sub",
					"down_image" : ROOT_PATH + "canclebutton02.sub",
				},
			),
		},
	),
}
