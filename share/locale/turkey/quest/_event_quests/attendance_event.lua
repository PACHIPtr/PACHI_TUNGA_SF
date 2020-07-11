--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest attendance_event begin
	state start begin	
        function getInfoByMob(mobVnum)
            info_map = {
                [6415]     = { "beran_hit_points", 2000, 1000, 500 },
                [6416]     = { "nemere_hit_points", 2000, 1000, 500 },
                [6417]     = { "razador_hit_points", 2000, 1000, 500 },
				[6418]     = { "jotun_thrym_hit_points", 2000, 1000, 500 },
				[6419]     = { "red_dragon_hit_points", 2000, 1000, 500 },
            }

            mobVnum = tonumber(mobVnum)

            return info_map[mobVnum]
        end
	
		when 30131.chat."Patron Avcýlarý Etkinliði" with game.get_event_flag("enable_attendance_event") >= 1 begin
			say("Patron avý etkinliði seviye 30 üzeri oyuncular")
			say("için.")
			say("Baþlangýçta patron canavarlar tarafsýz bölgelerde")
			say("belirir.")
			say("Her gün bir ödül alabilirsin.")
			say("En az bir isabetle günlük ödülünü elde edersin.")
			say("Ayrýca bir etkinlik ödülü kazanma þansýn olur -")
			say("patron canavar baþý isabetlerin geçerli sayýlýr.")
			say("")
			say("Oradan hayat puanlarýnýn güncel durumunu da")
			say("öðrenebilir ve ödüllerini alabilirsin. Hayat")
			say("puanlarýn ne kadar yüksekse ödülün o kadar")
			say("yükselir.")
		end
		
		when 30131.chat."Ödülünü teslim al" with game.get_event_flag("enable_attendance_event") >= 1 begin
			local selectEvent = select(mob_name(6415), mob_name(6416), mob_name(6417), mob_name(6419), "Vazgeç ")
			if selectEvent == 1 or selectEvent == 2 or selectEvent == 3 or selectEvent == 4 then
				local mob_vnum = 0
				
				if selectEvent == 1 then
					mob_vnum = 6415
				elseif selectEvent == 2 then
					mob_vnum = 6416
				elseif selectEvent == 3 then
					mob_vnum = 6417
				elseif selectEvent == 4 then
					mob_vnum = 6419
				end
				
				local map_info = attendance_event.getInfoByMob(mob_vnum)
				
				if mob_vnum != 0 and map_info != null then
					local selectReward = select(item_name(50272), item_name(50273), item_name(50274), "Vazgeç ")
					if selectReward == 1 or selectReward == 2 or selectReward == 3 then
						local item_vnum = 0
						local rewardPoints = 0
						
						if selectReward == 1 then
							item_vnum = 50272
							rewardPoints = map_info[2]
						elseif selectReward == 2 then
							item_vnum = 50273
							rewardPoints = map_info[3]
						elseif selectReward == 3 then
							item_vnum = 50274
							rewardPoints = map_info[4]
						end

						if item_vnum != 0 and rewardPoints != 0 then
							if pc.getqf(map_info[1]) >= rewardPoints then
								say_title("Patron Avcýlarý Etkinliði")
								say("Bu eþyayý seçmek istediðine emin misin ? "..item_name(item_vnum))
								say("("..rewardPoints.." Hayat puaný otomatik düþecek)")
								say("")
								say(mob_name(mob_vnum).." Sahip Olunan Hayat Puaný: "..pc.getqf(map_info[1]))
								say("")
								
								local selectReturn = select("Evet", "Hayýr")
								if selectReturn == 1 then
									pc.setqf(map_info[1], pc.getqf(map_info[1]) - rewardPoints)
									pc.give_item2(item_vnum, 1)
								elseif selectReturn == 2 then
									return
								end
							else
								say_title("Patron Avcýlarý Etkinliði")
								say("Karakterinden "..rewardPoints.." Hayat Puaný düþüldü ")
								say("Eþya envanterine eklendi.")
								say("Yeni hayat puaný: "..pc.getqf(map_info[1]))
							end
						end
					elseif selectReward == 4 then
						return
					end
				end
			elseif selectEvent == 5 then
				return
			end
		end

		when 30131.chat."Toplam Puan" with game.get_event_flag("enable_attendance_event") >= 1 begin
			say_title("Patron Avcýlarý Etkinliði")
			say(mob_name(6415).." Hayat Puaný: "..pc.getqf("beran_hit_points"))
			say("")
			say(mob_name(6416).." Hayat Puaný: "..pc.getqf("nemere_hit_points"))
			say("")
			say(mob_name(6417).." Hayat Puaný: "..pc.getqf("razador_hit_points"))
			say("")
			say(mob_name(6192).." Hayat Puaný: "..pc.getqf("jotun_thrym_hit_points"))
			say("")
			say(mob_name(6419).." Hayat Puaný: "..pc.getqf("red_dragon_hit_points"))
			say("")
		end
	end
end
