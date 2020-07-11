import uiScriptLocale

window = {
	"name" : "favlist",

	"x" : SCREEN_WIDTH - 170,
	"y" : SCREEN_HEIGHT - 400 - 50,

	"style" : ("movable", "float",),

	"width" : 170+100,
	"height" : 300,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",

			"x" : 0,
			"y" : 0,

			"width" : 170+100,
			"height" : 300,

			"title" : "Favori Videolarým",
		},
		{
			"name" : "yuzdelik",
			"type" : "text",

			"x" : 110,
			"y" : 130,

			"text" : "%",
		},
		{
			"name" : "ScrollBar",
			"type" : "scrollbar",

			"x" : 27,
			"y" : 40,

			"size" : 220,

			"horizontal_align" : "right",
		},
		{
			"name" : "sil",
			"type" : "button",

			"x" : 80,
			"y" : 265,

			"width" : 61,
			"height" : 21,

			"text" : "Sil",

			"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
			"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
			"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
		},
		{
			"name" : "izle",
			"type" : "button",

			"x" : 145,
			"y" : 265,

			"width" : 61,
			"height" : 21,

			"text" : "Izle",

			"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
			"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
			"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
		},
		{
			"name" : "cancel",
			"type" : "button",

			"x" : 115+100,
			"y" : 265,

			"width" : 41,
			"height" : 21,

			"text" : "Kapat",

			"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
			"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
			"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
		},
		{
			"name" : "refresh",
			"type" : "button",

			"x" : 15,
			"y" : 265,

			"width" : 41,
			"height" : 21,

			"text" : uiScriptLocale.MARKLIST_REFRESH,

			"default_image" : "d:/ymir work/ui/public/small_button_01.sub",
			"over_image" : "d:/ymir work/ui/public/small_button_02.sub",
			"down_image" : "d:/ymir work/ui/public/small_button_03.sub",
		},
	)
}
