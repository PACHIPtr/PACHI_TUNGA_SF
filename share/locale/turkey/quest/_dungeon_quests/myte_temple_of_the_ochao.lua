--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest temple_of_the_ochao begin
    state start begin
		when login with pc.get_map_index() == 354 begin
			temple_ochao.initialize()
		end
		
		when 6311.kill with pc.get_level() >= 95 begin
			local drop = {
							{["vnum"] = 2145, ["count"] = 1, ["chance"] = 60,},
							{["vnum"] = 11295, ["count"] = 1, ["chance"] = 55,},
							{["vnum"] = 11495, ["count"] = 1, ["chance"] = 55,},
							{["vnum"] = 11695, ["count"] = 1, ["chance"] = 55,},
							{["vnum"] = 11895, ["count"] = 1, ["chance"] = 55,},
							{["vnum"] = 21075, ["count"] = 1, ["chance"] = 55,},
							{["vnum"] = 70043, ["count"] = 1, ["chance"] = 45,},
							{["vnum"] = 30609, ["count"] = 1, ["chance"] = 15,},
							{["vnum"] = 30612, ["count"] = 1, ["chance"] = 15,},
							{["vnum"] = 30523, ["count"] = 1, ["chance"] = 40,},
			}
			
			for i, info in ipairs(drop) do
				local generate = number(1, 100)
				if generate <= info.chance then
					game.drop_item_with_ownership(info.vnum, info.count, 300)
				end
			end
		end
		
		when 20408.chat."Ochao Tapýnaðý'na giriþ yap " with pc.get_level() >= 95 begin
			say_title(mob_name(20408))
			say("")
			say("Doðrudan Ochao Tapýnaðý'na gitmek ister misin? ")
			local answer = select("Evet. ", "Hayýr ")
			if answer == 1 then
				pc.warp(853700, 1416400)
			end
		end
		
		when 20415.chat."Efsunlu Orman'a giriþ yap " with pc.get_level() >= 95 and pc.get_map_index() == 354 begin
			say_title(mob_name(20415))
			say("")
			say("Efsunlu Orman'a gitmek ister misin? ")
			local answer = select("Evet. ", "Hayýr ")
			if answer == 1 then
				pc.setf("enchanted_forest", "limit_activity", 0)
				pc.warp(816700, 1505000)
			end
		end
	end
end