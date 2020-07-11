--[[
 DungeonLib, Dungeon Information System Library, Version 1.8
 Copyright 2019 Owsap Productions
]]

dungeonLib = {}
dungeonInfo = {}
dungeonInfo.table = {
	-- game.get_event_flags
	--[[
	{
		["type"] = 2, -- Dungeon type [ 0 (Unkown), 1 (Private), 2 (Global) ]
		["organization"] = 0, -- Dungeon organization [ 0 (None), 1 (Party), 2 (Guild) ]
		["level_limit"] = 75, -- Dungeon level limit [ max_level ]
		["party_members"] = 8, -- Dungeon party members [ max_members (0) Unlimited ]
		["map"] = 66, -- Dungeon map index
		["cooldown"] = 0, -- [ 0 (None) ] | Ex: 60 * 60 * 3 = 3 hours
		["duration"] = 0, -- [ 0 (None) ] | Ex: 60 * 60 * 3 = 3 hours
		["entrance_map"] = 65, -- Entrance map index
		["strength_bonus"] = 63, -- Strength bonus id against dungeon monsters
		["resistance_bonus"] = 35, -- Resistance bonus id against dungeon monsters
		["item_vnum"] = 0, -- Required dungeon item
	},
	]]
	{ -- devil_tower
		["type"] = 2,
		["organization"] = 0,
		["level_limit"] = 50,
		["party_members"] = 8,
		["map"] = 66,
		["map_coords"] = {5905, 1108},
		["cooldown"] = 60,
		["duration"] = 0,
		["entrance_map"] = 65,
		["strength_bonus"] = 35,
		["resistance_bonus"] = 44,
		["item_vnum"] = 0,
	},
	{ -- Crystal
		["type"] = 1,
		["organization"] = 1,
		["level_limit"] = 85,
		["party_members"] = 8,
		["map"] = 9,
		["map_coords"] = {7339, 6231},
		["cooldown"] = 60,
		["duration"] = 60*60*3,
		["entrance_map"] = 64,
		["strength_bonus"] = 35,
		["resistance_bonus"] = 41,
		["item_vnum"] = 31335,
	},
	{ -- blue_tower
		["type"] = 1,
		["organization"] = 1,
		["level_limit"] = 40,
		["party_members"] = 8,
		["map"] = 18,
		["map_coords"] = {5905, 1108},
		["cooldown"] = 60,
		["duration"] = 60*60*2,
		["entrance_map"] = 65,
		["strength_bonus"] = 35,
		["resistance_bonus"] = 44,
		["item_vnum"] = 31336,
	},
	{ -- ork_maze
		["type"] = 1,
		["organization"] = 0,
		["level_limit"] = 40,
		["party_members"] = 0,
		["map"] = 245,
		["map_coords"] = {3306, 7553},
		["cooldown"] = 60*60*1,
		["duration"] = 60*60*1,
		["entrance_map"] = 64,
		["strength_bonus"] = 35,
		["resistance_bonus"] = 44,
		["item_vnum"] = 90005,
	},
	{ -- midgard_dungeon
		["type"] = 0,
		["organization"] = 0,
		["level_limit"] = 110,
		["party_members"] = 3,
		["map"] = 507,
		["map_coords"] = {10608, 7620},
		["cooldown"] = 60*60*2,
		["duration"] = 60*45,
		["entrance_map"] = 69,
		["strength_bonus"] = 35,
		["resistance_bonus"] = 43,
		["item_vnum"] = 31324,
	},
	{ -- Zodyak
		["type"] = 2,
		["organization"] = 0,
		["level_limit"] = 90,
		["party_members"] = 0,
		["map"] = 358,
		["map_coords"] = {3334, 14313},
		["cooldown"] = 0,
		["duration"] = 0,
		["entrance_map"] = 358,
		["strength_bonus"] = 63,
		["resistance_bonus"] = 0,
		["item_vnum"] = 0,
	},
	{ -- hydra
		["type"] = 1,
		["organization"] = 1,
		["level_limit"] = 100,
		["party_members"] = 5,
		["map"] = 502,
		["map_coords"] = {1686, 6117},
		["cooldown"] = 60*60*6,
		["duration"] = 60*45,
		["entrance_map"] = 502,
		["strength_bonus"] = 63,
		["resistance_bonus"] = 40,
		["item_vnum"] = 31323,
	},
	{ -- Mantar
		["type"] = 2,
		["organization"] = 0,
		["level_limit"] = 100,
		["party_members"] = 0,
		["map"] = 244,
		["map_coords"] = {11144, 534},
		["cooldown"] = 60*60*2,
		["duration"] = 60*45,
		["entrance_map"] = 68,
		["strength_bonus"] = 1,
		["resistance_bonus"] = 42,
		["item_vnum"] = 31322,
	},
	{ -- catacomb
		["type"] = 1,
		["organization"] = 1,
		["level_limit"] = 60,
		["party_members"] = 5,
		["map"] = 216,
		["map_coords"] = {5917, 996},
		["cooldown"] = 120,
		["duration"] = 0,
		["entrance_map"] = 65,
		["strength_bonus"] = 35,
		["resistance_bonus"] = 44,
		["item_vnum"] = 30319,
	},
	{ -- Beran-Setaou
		["type"] = 1,
		["organization"] = 0,
		["level_limit"] = 80,
		["party_members"] = 0,
		["map"] = 207,
		["map_coords"] = {1815, 12206},
		["cooldown"] = 60*10,
		["duration"] = 60*30,
		["entrance_map"] = 73,
		["strength_bonus"] = 39,
		["resistance_bonus"] = 40,
		["item_vnum"] = 30179,
	},
	{ -- Barones
		["type"] = 1,
		["organization"] = 0,
		["level_limit"] = 55,
		["party_members"] = 0,
		["map"] = 217,
		["map_coords"] = {689, 6107},
		["cooldown"] = 60*20,
		["duration"] = 60*45,
		["entrance_map"] = 71,
		["strength_bonus"] = 1,
		["resistance_bonus"] = 43,
		["item_vnum"] = 30324,
	},

	{ -- Nemere
		["type"] = 1,
		["organization"] = 01,
		["level_limit"] = 90,
		["party_members"] = 0,
		["map"] = 352,
		["map_coords"] = {4320, 1650},
		["cooldown"] = 60*60*2,
		["duration"] = 60*45,
		["entrance_map"] = 61,
		["strength_bonus"] = 22,
		["resistance_bonus"] = 40,
		["item_vnum"] = 71174,
	},
	
	{ -- Meley
		["type"] = 1,
		["organization"] = 2,
		["level_limit"] = 90,
		["party_members"] = 0,
		["map"] = 212,
		["map_coords"] = {5959, 6992},
		["cooldown"] = 60*60*3,
		["duration"] = 60*45,
		["entrance_map"] = 62,
		["strength_bonus"] = 22,
		["resistance_bonus"] = 41,
		["item_vnum"] = 0,
	},
	
	{ -- Razadör
		["type"] = 1,
		["organization"] = 0,
		["level_limit"] = 90,
		["party_members"] = 0,
		["map"] = 351,
		["map_coords"] = {5959, 6992},
		["cooldown"] = 60*60*1,
		["duration"] = 60*60,
		["entrance_map"] = 62,
		["strength_bonus"] = 22,
		["resistance_bonus"] = 41,
		["item_vnum"] = 71175,
	},
	{ -- Jotun
		["type"] = 1,
		["organization"] = 1,
		["level_limit"] = 90,
		["party_members"] = 0,
		["map"] = 353,
		["map_coords"] = {12778, 17343},
		["cooldown"] = 60*60*1,
		["duration"] = 0,
		["entrance_map"] = 302,
		["strength_bonus"] = 35,
		["resistance_bonus"] = 43,
		["item_vnum"] = 30613,
	},
}
dungeonInfo.map_name = {
	[0] = "Bilinmiyor",
	[9] = "Kristal Zindan",
	[64] = "Seungryong Vadisi",
	[65] = "Hwang Tapýnaðý ",
	[66] = "Þeytan Kulesi",
	[216] = "Þeytan Katakombu",
	[321] = "Büyülü Orman",
	[322] = "Erebus Zindaný ",
	[502] = "Gemi Savunmasý Giriþi ",
	[207] = "Beran-Setaou ",
	[73] = "Sürgün Maðarasý Kat 2 ",
	[71] = "Örümcek Zindaný Kat 2 ",
	[217] = "Baronesin Ýni ",
	[61] = "Sohan Daðý ",
	[352] = "Nemerenin Gözetleme Kulesi",
	[62] = "Doyum Paper ",
	[212] = "Meleyin Ýni ",
	[351] = "Kýrmýzý Ejderha Kalesi ",
	[302] = "Guatama Uçurumu ",
	[353] = "Jotun Thrym ",
	[358] = "Zodyak Tapýnaðý ",
	[244] = "Lanetli Orman ",
	[68] = "Kýzýl Orman ",
	[507] = "Ruhlar Maðarasý ",
	[69] = "Yýlan Vadisi ",
	[245] = "Ork Labirenti",
	[18] = "Mavi Ölümün Kulesi",
}
dungeonInfo.bonus_name = {
	[0] = "Bilinmiyor ",
	[1] = "Hayvanlara Karþý Güç ",
	[19] = "Orklara Karþý Güç ",
	[22] = "Þeytanlara Karþý Güç ",
	[63] = "Canavarlara Karþý Güç ",
	[35] = "Ölümsüzlere Karþý Güç",
	[38] = "Orklara Karþý Güç ",
	[39] = "Mistiklere Karþý Güç ",
	[40] = "Buz Týlsýmý ",
	[41] = "Ateþ Týlsýmý ",
	[42] = "Toprak Týlsýmý ",
	[43] = "Rüzgar Týlsýmý ",
	[44] = "Karanlýk týlsýmý ",
	[45] = "Þimþek Týlsýmý ",
}

function dungeonLib.GetRepTime(dungeon_index)
	if dungeon_index == 66 then  -- devil_tower
		return 83832 -- Sürekli girilebilir
	elseif dungeon_index == 507 then -- midgard_dungeon
		return pc.getf("midgard_dungeon", "md_player_time_110")
	elseif dungeon_index == 358 then -- Zodyak
		return 83833  -- Animasfer ile girilebilir
	elseif dungeon_index == 502 then -- hydra
		return pc.getf("defensewave", "dungeon_last_login_time")
	elseif dungeon_index == 244 then -- Mantar
		return pc.getf("mushroom_dungeon", "player_dung_65")
	elseif dungeon_index == 245 then -- ork_zindani
		return pc.getf("ork_maze", "ork_player_time_110")
	elseif dungeon_index == 216 then -- catacomb
		return 83832  -- Sürekli girilebilir
	elseif dungeon_index == 207 then -- Beran-Setaou
		return pc.getf("dragonlair", "dragonlair_soguma_suresi")
	elseif dungeon_index == 217 then -- Barones
		return pc.getf("spider_dungeon_3floor_boss", "retry_limit_time")
	elseif dungeon_index == 352 then -- Nemere
		return pc.getf("snow_dungeon","duration")
	elseif dungeon_index == 212 then -- Meley
		return 83834  -- Bilinmiyor
	elseif dungeon_index == 351 then -- Razadör
		local t = get_global_time() - pc.getf("flame_dungeon","exit_time")
		return get_global_time()+t
	elseif dungeon_index == 353 then -- Jotun
		return 83834  -- Bilinmiyor
	elseif dungeon_index == 18 then -- BlueTower
		return pc.getf("devil_tower2_plechito", "bt_player_time_duration")
	elseif dungeon_index == 9 then -- Crystal
		return pc.getf("crystal_dungeon1", "cy_player_time_duration")
	end
end

function dungeonLib.update()
	local dungeonTable = dungeonInfo.table

	if table.getn(dungeonTable) == 0 then return end

	cmdchat(string.format("DungeonInfo %d", q.getcurrentquestindex()))
	cmdchat(string.format("CleanDungeonInfo"))

	for index in ipairs(dungeonTable) do
		-- Get indexed data from dungeon table
		dungeonType = dungeonTable[index]["type"]
		dungeonOrganization = dungeonTable[index]["organization"]
		dungeonLevelLimit = dungeonTable[index]["level_limit"]
		dungeonPartyMembers = dungeonTable[index]["party_members"]
		dungeonMap = dungeonTable[index]["map"]
		dungeonMapIndex = dungeonMap
		dungeonMapCoordX = dungeonTable[index]["map_coords"][1]
		dungeonMapCoordY = dungeonTable[index]["map_coords"][2]
		dungeonCooldown = dungeonTable[index]["cooldown"]
		dungeonDuration = dungeonTable[index]["duration"]
		dungeonEntranceMap = dungeonTable[index]["entrance_map"]
		dungeonStrengthBonus = dungeonTable[index]["strength_bonus"]
		dungeonResistanceBonus = dungeonTable[index]["resistance_bonus"]
		dungeonItemVnum = dungeonTable[index]["item_vnum"]
		dungeonRepTime = dungeonLib.GetRepTime(dungeonMap)

		-- Converts data to string
		dungeonMapName = dungeonInfo.map_name[dungeonMap]
		dungeonEntranceMapName = dungeonInfo.map_name[dungeonEntranceMap]
		dungeonStrengthBonusName = dungeonInfo.bonus_name[dungeonStrengthBonus]
		dungeonResistanceBonusName = dungeonInfo.bonus_name[dungeonResistanceBonus]

		-- Replace converted strings for client communication
		dungeonMapName = string.gsub(dungeonMapName, " ", "_")
		dungeonEntranceMapName = string.gsub(dungeonEntranceMapName, " ", "_")
		dungeonStrengthBonusName = string.gsub(dungeonStrengthBonusName, " ", "_")
		dungeonResistanceBonusName = string.gsub(dungeonResistanceBonusName, " ", "_")

		cmdchat(string.format("UpdateDungeonInfo %d %d %d %d %s %d %d %d %d %d %s %s %s %d %d",
			dungeonType,
			dungeonOrganization,
			dungeonLevelLimit,
			dungeonPartyMembers,
			dungeonMapName,
			dungeonMapIndex,
			dungeonMapCoordX,
			dungeonMapCoordY,
			dungeonCooldown,
			dungeonDuration,
			dungeonEntranceMapName,
			dungeonStrengthBonusName,
			dungeonResistanceBonusName,
			dungeonItemVnum,
			dungeonRepTime
		))
	end
end