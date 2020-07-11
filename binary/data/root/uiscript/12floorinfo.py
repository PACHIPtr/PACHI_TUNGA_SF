import localeInfo

ROOT_PATH = "d:/ymir work/ui/game/12zi/timer/"

window = {
	"name" : "FloorInfo",

	"x" : SCREEN_WIDTH - 136,
	"y" : 0,

	"width" : 136,
	"height" : 161,

	"children" :
	(
		{
			"name" : "FloorInfoBG",
			"type" : "image",
			"x" : 0,
			"y" : 0,
			"image" : ROOT_PATH + "bg_12zi_timer.sub",
		},
		{
			"name" : "CoolTime",
			"type" : "expanded_image",
			"x" : -1,
			"y" : -1,
			"image" : ROOT_PATH + "gauge_timer.sub",
		},
		{
			"name" : "JumpButton",
			"type" : "button",

			"x" : 39,
			"y" : 115,

			"default_image" : ROOT_PATH + "btn_jump_default.sub",
			"over_image" : ROOT_PATH + "btn_jump_over.sub",
			"down_image" : ROOT_PATH + "btn_jump_down.sub",
		},
		{ "name":"JumpStep", "type":"text", "x":68, "y":40, "text" : "5", "text_horizontal_align":"center", "r" : 1.0, "g" : 0.0, "b" : 0.0, "a" : 1.0, "fontsize":"LARGE"},
		{ "name":"LeftTime", "type":"text", "x":68, "y":62, "text" : "11:11", "text_horizontal_align":"center" },
		{ "name":"CurrentFloor", "type":"text", "x":68, "y":78, "text" : "5", "text_horizontal_align":"center", "r" : 1.0, "g" : 0.831, "b" : 0.043, "a" : 1.0, "fontsize":"LARGE"},
	),
}

