
WEB_WIDTH = 640
WEB_HEIGHT = 360+25

window = {
	"name" : "TrailerWindow",
	"style" : ("movable", "float",),

	"x" : 0,
	"y" : 0,

	"width"  : WEB_WIDTH  + 20,
	"height" : WEB_HEIGHT + 40,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",

			"style" : ("attach", ),

			"x" : 0,
			"y" : 0,

			"width"	 : WEB_WIDTH  + 20,
			"height" : WEB_HEIGHT + 40,

			"title" : "Youtube",
		},
	),
}
