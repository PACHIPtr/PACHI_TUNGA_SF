import uiScriptLocale

WEB_WIDTH = 640
WEB_HEIGHT = 360+25

window = {
	"name" : "YoutubeWebWindow",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width"  : WEB_WIDTH  + 20,
	"height" : WEB_HEIGHT + 40,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width"	 : WEB_WIDTH  + 20,
			"height" : WEB_HEIGHT + 40,

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 7,

					"width" : WEB_WIDTH + 10,
					"color" : "yellow",

					"children" :
					(
						{
							"name":"TitleName",
							"type":"text",

							"x":50,
							"y":3,

							"text": "Youtube",
							"text_horizontal_align":"center"
						},
					),
				},
			),
		},
	),
}
