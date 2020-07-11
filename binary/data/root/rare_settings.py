import localeInfo

rarityLevelNames = {
	0: localeInfo.ITEM_RARE_LEVEL_0,
	1: localeInfo.ITEM_RARE_LEVEL_1,
	2: localeInfo.ITEM_RARE_LEVEL_2,
	3: localeInfo.ITEM_RARE_LEVEL_3,
	4: localeInfo.ITEM_RARE_LEVEL_4,
}
rarityLevelColors = {
	0: 4294043328L,
	1: 4289593135L,
	2: 4294967040L,
	3: 4294956800L,
	4: 4294944000L,
	5: 4294937600L,
	6: 4294901760L,
	7: 4278812142L,
}

def DetermineRareLevel(rarePoints):
	if rarePoints >= 10 and rarePoints < 25:
		return 1
	elif rarePoints >= 25 and rarePoints < 50:
		return 2
	elif rarePoints >= 50 and rarePoints < 75:
		return 3
	elif rarePoints >= 75 and rarePoints < 200:
		return 4

	return 0