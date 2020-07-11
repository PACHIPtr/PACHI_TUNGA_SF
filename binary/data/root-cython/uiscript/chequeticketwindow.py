import uiScriptLocale

BUTTON_ROOT = "d:/ymir work/ui/public/"

window = {
	"name" : "ChequeTicketWindow",
	
	"style" : ("movable", "float", ),
	
	"x" : 0,
	"y" : 0,
	
	"width" 	: 200,
	"height"	: 110,
	
	"children" : 
	(
		{
			"name" 	: "main_board",
			"type"	: "board_with_titlebar",
			
			"style" : ("attach",),
			
			"x"		: 0,
			"y"		: 0,
			
			"width"	: 200,
			"height" : 110,
			
			"title" : uiScriptLocale.CHEQUE_TICKET_TITLE,
			
			"children" : 
			(
				{
					"name" : "InputSlot",
					"type" : "slotbar",
					
					"x" : 10,
					"y" : 40,
					
					"width" : 180,
					"height" : 20,
					
					"children" : 
					(
						{
							"name" : "InputValue",
							"type" : "editline",
							
							"x" : 25,
							"y" : 4,
							
							"width" : 180,
							"height" : 20,
							
							"text" : "0",
							"input_limit" : 4,
							"only_number" : 1,
						},
						{
							"name" : "ChequeIcon",
							"type" : "image",
							
							"x" : 2,
							"y" : 3,
							
							"image" : "d:/ymir work/ui/game/windows/cheque_icon.sub",
						},								
					),
				},
				
				##LeftButton
				{
					"name" : "Lbutton",
					"type" : "button",

					"x" : 30,
					"y" : 80,
					
					"default_image" : BUTTON_ROOT+"AcceptButton00.sub",
					"over_image" : BUTTON_ROOT+"AcceptButton01.sub",
					"down_image" : BUTTON_ROOT+"AcceptButton02.sub",
				},
				
				##RightButton
				{
				    "name" : "Rbutton",
				    "type" : "button",
				    
				    "x" : 110,
				    "y" : 80,
					
					"default_image" : BUTTON_ROOT+"CancleButton00.sub",
					"over_image" : BUTTON_ROOT+"CancleButton01.sub",
					"down_image" : BUTTON_ROOT+"CancleButton02.sub",				    
				},				
			),
		},
	),
}