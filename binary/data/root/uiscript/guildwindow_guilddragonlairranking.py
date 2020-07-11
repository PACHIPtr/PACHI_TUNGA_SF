import uiScriptLocale

BOARD_WIDTH = 368
ROOT_PATH = "d:/ymir work/ui/game/guild/dragonlairranking/"
BACK_IMG_PATH = "d:/ymir work/ui/public/public_board_back/"

window = {
	"name" : "GuildDragonLairWindow",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : BOARD_WIDTH,
	"height" : 238,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : BOARD_WIDTH,
			"height" : 238,
		
			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 6,

					"width" : 355,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":BOARD_WIDTH/2, "y":3, "text":uiScriptLocale.GUILD_DRAGONLAIR_RANKING, "text_horizontal_align":"center" },
					),
				},
				## 센터 백그라운드 시작
				## LeftTop
				{
					"name" : "LeftTop",
					"type" : "image",
					"x" : 17,
					"y" : 38,
					"image" : BACK_IMG_PATH+"boardback_mainboxlefttop.sub",
				},
				## RightTop
				{
					"name" : "RightTop",
					"type" : "image",
					"x" : 318,
					"y" : 38,
					"image" : BACK_IMG_PATH+"boardback_mainboxrighttop.sub",
				},
				## LeftBottom
				{
					"name" : "LeftBottom",
					"type" : "image",
					"x" : 17,
					"y" : 173,
					"image" : BACK_IMG_PATH+"boardback_mainboxleftbottom.sub",
				},
				## RightBottom
				{
					"name" : "RightBottom",
					"type" : "image",
					"x" : 318,
					"y" : 173,
					"image" : BACK_IMG_PATH+"boardback_mainboxrightbottom.sub",
				},
				## leftcenterImg
				{
					"name" : "leftcenterImg",
					"type" : "expanded_image",
					"x" : 17,
					"y" : 38+16,
					"image" : BACK_IMG_PATH+"boardback_leftcenterImg.tga",
					"rect" : (0.0, 0.0, 0, 6),
				},
				## rightcenterImg
				{
					"name" : "rightcenterImg",
					"type" : "expanded_image",
					"x" : 317,
					"y" : 38+16,
					"image" : BACK_IMG_PATH+"boardback_rightcenterImg.tga",
					"rect" : (0.0, 0.0, 0, 6),
				},
				## topcenterImg
				{
					"name" : "topcenterImg",
					"type" : "expanded_image",
					"x" : 17+15,
					"y" :  38,
					"image" : BACK_IMG_PATH+"boardback_topcenterImg.tga",
					"rect" : (0.0, 0.0, 16, 0),
				},
				## bottomcenterImg
				{
					"name" : "bottomcenterImg",
					"type" : "expanded_image",
					"x" : 17+15,
					"y" : 173,
					"image" : BACK_IMG_PATH+"boardback_bottomcenterImg.tga",
					"rect" : (0.0, 0.0, 16, 0),
				},
				## centerImg
				{
					"name" : "centerImg",
					"type" : "expanded_image",
					"x" : 17+15,
					"y" : 38+15,
					"image" : BACK_IMG_PATH+"boardback_centerImg.tga",
					"rect" : (0.0, 0.0, 16, 6),
				},
				## 센터 백그라운드 끝
				## GuildTiTleImg
				{
					"name" : "GuildTiTleImg",
					"type" : "image",
					"x" : 20,
					"y" : 41,
					"image" : ROOT_PATH+"ranking_list_menu.sub",
					"children" :
					(
						## Text
						{ "name" : "ResultNameRanking", "type" : "text", "x" : 10, "y" : 4,  "text" : uiScriptLocale.GUILD_DRAGONLAIR_RANKING_COUNT, },
						{ "name" : "ResultNameGuild", "type" : "text", "x" : 95, "y" : 4, "text" : uiScriptLocale.GUILD_DRAGONLAIR_RANKING_NAME, },
						{ "name" : "ResultMemberCount", "type" : "text", "x" : 180, "y" : 4, "text" : uiScriptLocale.GUILD_DRAGONLAIR_RANKING_MEMBER, },
						{ "name" : "ResultClearTime", "type" : "text", "x" : 240, "y" : 4, "text" : uiScriptLocale.GUILD_DRAGONLAIR_RANKING_TIME, },
					),
				},
				## 스크롤 바.
				{
					"name" : "GuildDragonLairScrollBar",
					"type" : "scrollbar",
					"x" : 340,
					"y" : 38,
					"size" : 180,
				},
				## 센터 백그라운드 시작 (자신길드)
				## LeftTop
				{
					"name" : "LeftTopSelf",
					"type" : "image",
					"x" : 17,
					"y" : 190,
					"image" : BACK_IMG_PATH+"boardback_mainboxlefttop.sub",
				},
				## RightTop
				{
					"name" : "RightTopSelf",
					"type" : "image",
					"x" : 318,
					"y" : 190,
					"image" : BACK_IMG_PATH+"boardback_mainboxrighttop.sub",
				},
				## LeftBottom
				{
					"name" : "LeftBottomSelf",
					"type" : "image",
					"x" : 17,
					"y" : 190+15,
					"image" : BACK_IMG_PATH+"boardback_mainboxleftbottom.sub",
				},
				## RightBottom
				{
					"name" : "RightBottomSelf",
					"type" : "image",
					"x" : 318,
					"y" : 190+15,
					"image" : BACK_IMG_PATH+"boardback_mainboxrightbottom.sub",
				},
				## topcenterImg
				{
					"name" : "topcenterImgSelf",
					"type" : "expanded_image",
					"x" : 17+15,
					"y" :  190,
					"image" : BACK_IMG_PATH+"boardback_topcenterImg.tga",
					"rect" : (0.0, 0.0, 16, 0),
				},
				## bottomcenterImg
				{
					"name" : "bottomcenterImgSelf",
					"type" : "expanded_image",
					"x" : 17+15,
					"y" : 190+15,
					"image" : BACK_IMG_PATH+"boardback_bottomcenterImg.tga",
					"rect" : (0.0, 0.0, 16, 0),
				},			
			),
		},
	),
}

