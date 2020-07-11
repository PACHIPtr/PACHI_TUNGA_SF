--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest basic_weapon_item begin
	state start begin
		when 50187.use begin
			-----------------------------------
			if pc.getqf("basic_weapon_item") != 0 then
				item.remove()
				syschat("Baþlangýç Sandýðý zaten kullanýlmýþ.")
				return
			end

			item.remove()
			-----------------------------------
			pc.setqf("basic_weapon_item",1)
			say_title("Baþlangýç Sandýðý:")
			say("")
			say(string.format("Hoþgeldin [%s].", pc.get_name()))
			say("Ejderha tanrýlarý karakterine eklendi.")
			say("Aklýna takýlan sorularý destek sistemimiz [ENTER]aracýlýðýyla bize iletebilirsin.")
			say("Myte2 ekibi olarak iyi oyunlar dileriz.")
			say("")
			-----------------------------------
			pc.give_item2(27105,50)
			pc.give_item2(27102,50)
			pc.give_item2(71027,1)
			pc.give_item2(71028,1)
			pc.give_item2(71029,1)
			pc.give_item2(71030,1)
			pc.give_item2(71044,1)
			pc.give_item2(71045,1)
			pc.give_item2(39039,1)
			pc.give_item2(39042,1)
			pc.give_item2(40003,1)
			pc.give_item2(72701,1)
			pc.give_item2(27989,1)
			pc.give_item2(70038,1)
			pc.give_item2(81700,3)
			pc.give_item2(50053,1)
			pc.give_item2(13131,1)
			
			-- buffi
			pc.give_item2_select(81001, 1)
			item.set_basic(true)
			
			-- mount
			pc.give_item2_select(40496, 1)
			item.set_basic(true)
			
			-- Pet
			pc.give_item2_select(40497, 1)
			item.set_basic(true)

			game.set_safebox_level(3)
			ds.give_qualification()
			-----------------------------------
		end
	end
end
