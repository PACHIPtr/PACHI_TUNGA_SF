import uiScriptLocale

window = {
	"name" : "GuildWindow_BoardPage",
	"x" : 8,
	"y" : 30,
	"width" : 360,
	"height" : 298,
	"children" :
	(
		{
			"name" : "GradeNumber", "type" : "text", "x" : 21, "y" : 5, "text" : uiScriptLocale.GUILD_GRADE_NUM,
		},
		{
			"name" : "GradeName", "type" : "text", "x" : 76, "y" : 5, "text" : uiScriptLocale.GUILD_GRADE_RANK,
		},
		{
			"name" : "InviteAuthority", "type" : "text", "x" : 126, "y" : 5, "text" : uiScriptLocale.GUILD_GRADE_PERMISSION_JOIN,
		},
		{
			"name" : "DriveOutAuthority", "type" : "text", "x" : 183, "y" : 5, "text" : uiScriptLocale.GUILD_GRADE_PERMISSION_DELETE,
		},
		{
			"name" : "NoticeAuthority", "type" : "text", "x" : 240, "y" : 5, "text" : uiScriptLocale.GUILD_GRADE_PERMISSION_NOTICE,
		},
		{
			"name" : "SkillAuthority", "type" : "text", "x" : 297, "y" : 5, "text" : uiScriptLocale.GUILD_GRADE_PERMISSION_SKILL,
		},
		{
			"name" : "NextGrade",
			"type" : "button",
			"x" : 346,
			"y" : 6,
			"default_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_01.sub",
			"over_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_01.sub",
			"down_image" : "d:/ymir work/ui/public/public_page_button/page_next_btn_02.sub",
		},
		{
			"name" : "PrevGrade",
			"type" : "button",
			"x" : 4,
			"y" : 6,
			"default_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_01.sub",
			"over_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_01.sub",
			"down_image" : "d:/ymir work/ui/public/public_page_button/page_prev_btn_02.sub",
		},
		{
			"name" : "WarAuthority",
			"type" : "text",
			"x" : 126,
			"y" : 5,
			"text" : uiScriptLocale.GUILD_GRADE_PERMISSION_WAR,
		},

	),
}
