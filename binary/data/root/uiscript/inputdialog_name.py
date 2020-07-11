import uiScriptLocale
window = {
	"name" : "InputDialog",
	"x" : 0,
	"y" : 0,
	"style" : ("movable", "float",),
	"width" : 170,
	"height" : 146,
	"children" :
	(
		{
			"name" : "Board",
			"type" : "board_with_titlebar",
			"x" : 0,
			"y" : 0,
			"width" : 170,
			"height" : 146,
			"title" : "",
			"children" :
			(
				{
					"name" : "image1",
					"type" : "image",
					"x" : 6,
					"y" : 34,
					"image" : "icon/item/55401.tga",
				},
				{
					"name" : "image1",
					"type" : "image",
					"x" : 31,
					"y" : 34,
					"image" : "icon/item/55402.tga",
				},
				{
					"name" : "image2",
					"type" : "image",
					"x" : 56,
					"y" : 34,
					"image" : "icon/item/55403.tga",
				},
				{
					"name" : "image3",
					"type" : "image",
					"x" : 81,
					"y" : 34,
					"image" : "icon/item/55404.tga",
				},
				{
					"name" : "image4",
					"type" : "image",
					"x" : 106,
					"y" : 34,
					"image" : "icon/item/55401.tga",
				},
				{
					"name" : "image5",
					"type" : "image",
					"x" : 131,
					"y" : 34,
					"image" : "icon/item/55402.tga",
				},
				{
					"name" : "info_text",
					"type" : "text",
					"outline" : 1,
					"x" : 15,
					"y" : 70,
					"text" : "Yeni evcil haycan ismini giriniz: ",
				},
				## Input Slot
				{
					"name" : "InputSlot",
					"type" : "slotbar",
					"x" : -1,
					"y" : 86,
					"width" : 125,
					"height" : 18,
					"horizontal_align" : "center",
					"children" :
					(
						{
							"name" : "InputValue",
							"type" : "editline",
							"x" : 3,
							"y" : 3,
							"width" : 125,
							"height" : 18,
							"input_limit" : 12,
						},
					),
				},
				## Button
				{
					"name" : "AcceptButton",
					"type" : "button",
					"x" : - 61 - 5 + 30,
					"y" : 112,
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
					"y" : 112,
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
