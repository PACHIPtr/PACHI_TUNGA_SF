import uiScriptLocale
import app
ROOT = "d:/ymir work/ui/game/"
BOARD_ADD_X = 0
BOARD_ADD_X += 110+69
BOARD_X = SCREEN_WIDTH - (140 + BOARD_ADD_X)
BOARD_WIDTH = (140 + BOARD_ADD_X)
BOARD_HEIGHT = 40
window = {
	"name" : "ExpandedMoneyTaskbar",
	"x" : BOARD_X,
	"y" : SCREEN_HEIGHT - 65,
	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,
	"style" : ("float",),
	"children" :
	[
		{
			"name" : "ExpanedMoneyTaskBar_Board",
			"type" : "board",
			"x" : 0,
			"y" : 0,
			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,
			"children" :
			[
				## Print
				{
					"name":"Money_Icon",
					"type":"image",
					
					"x":20 + BOARD_ADD_X,
					"y":10,

					"image":"d:/ymir work/ui/game/windows/money_icon.sub",
				},
				{
					"name":"Money_Slot",
					"type":"button",

					"x":39 + BOARD_ADD_X,
					"y":10,

					#"horizontal_align":"center",

					"default_image" : "d:/ymir work/ui/public/gold_slot.sub",
					"over_image" : "d:/ymir work/ui/public/gold_slot.sub",
					"down_image" : "d:/ymir work/ui/public/gold_slot.sub",

					"children" :
					(
						{
							"name" : "Money",
							"type" : "text",

							"x" : 3,
							"y" : 3,

							"horizontal_align" : "right",
							"text_horizontal_align" : "right",

							"text" : "9,999,999,999",
						},
					),
				},
			],
		},		
	],
}

window["children"][0]["children"] = window["children"][0]["children"] + [
					{
						"name":"Cheque_Icon",
						"type":"image",
						
						"x": BOARD_ADD_X - 27,
						"y": 10,

						"image":"d:/ymir work/ui/game/windows/cheque_icon.sub",
					},					
					{
						"name":"Cheque_Slot",
						"type":"button",

						"x": BOARD_ADD_X - 8,
						"y":10,

						"default_image" : "d:/ymir work/ui/public/cheque_slot.sub",
						"over_image" : "d:/ymir work/ui/public/cheque_slot.sub",
						"down_image" : "d:/ymir work/ui/public/cheque_slot.sub",

						"children" :
						(
							{
								"name" : "Cheque",
								"type" : "text",

								"x" : 3,
								"y" : 3,

								"horizontal_align" : "right",
								"text_horizontal_align" : "right",

								"text" : "99",
							},
						),
					},		
]
window["children"][0]["children"] = window["children"][0]["children"] + [
				{
					"name":"Gem_Icon",
					"type":"image",
					
					"x":BOARD_ADD_X - 98,
					"y":13,

					"image":"d:/ymir work/ui/gemshop/gemshop_gemicon.sub",
				},					
				{
					"name":"Gem_Slot",
					"type":"image",

					"x": BOARD_ADD_X - 82,
					"y":10,
					
					"image" : "d:/ymir work/ui/public/Parameter_Slot_01.sub",

					"children" :
					(
						{
							"name" : "Gem",
							"type" : "text",

							"x" : 3,
							"y" : 3,

							"horizontal_align" : "right",
							"text_horizontal_align" : "right",

							"text" : "999,999",
						},
					),
				},		
				]
				
window["children"][0]["children"] = window["children"][0]["children"] + [
				{
					"name":"RedGem_Icon",
					"type":"image",
					
					"x":BOARD_ADD_X - 98 - 86 + 13,
					"y":13,

					"image":"d:/ymir work/ui/metin-gaya.dds",
				},					
				{
					"name":"RedGem_Slot",
					"type":"image",

					"x": BOARD_ADD_X - 82 - 91 + 5  + 13,
					"y":10,
					
					"image" : "d:/ymir work/ui/public/Parameter_Slot_01.sub",

					"children" :
					(
						{
							"name" : "RedGem",
							"type" : "text",

							"x" : 3,
							"y" : 3,

							"horizontal_align" : "right",
							"text_horizontal_align" : "right",

							"text" : "999,999",
						},
					),
				},		
				]