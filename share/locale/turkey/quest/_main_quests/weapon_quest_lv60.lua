--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest weapon_quest_lv60 begin
	state start begin
	end
	state run begin
		when login or levelup or enter with pc.level >= 60 begin
			set_state(information)
		end
	end
	state information begin
		when letter begin
			send_letter("~Genç Kahramanlar Ýçin Silahlar")
		end
		
		when button or info begin
			say_title("Genç kahramanlar için silahlar")
			say("")
			say("Genç kahraman en cesur savaþçý olarak nam saldýn")
			say("ama hala tecrübe ve güce ihtiyacýn var. Ejderha")
			say("Tanrýsý bu krallýðýn umudu olarak seni koruyup")
			say("kollayacak ve sana maceralarýnda belli bir süre")
			say("kullanabileceðin bir silah sunacak.")
			wait()
			say_title("Silahýný Seç: ")
			if pc.job == 0 then
				say_item_vnum_ex(get_changing_weapon(0, 13), 0, 2)
				say_item_vnum_ex(get_changing_weapon(0, 14), 1, 2)
				local s = select(item_name(get_changing_weapon(0, 13)), item_name(get_changing_weapon(0, 14)), "Henüz Deðil.")
				if s == 1 then
					pc.give_item2_select(get_changing_weapon(0, 13), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(0, 13))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)
				elseif s == 2 then
					pc.give_item2_select(get_changing_weapon(0, 14), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(0, 14))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)			
				else
					send_letter("~Genç Kahramanlar Ýçin Silahlar")
				end
			elseif pc.job == 1 then
				say_size(300, 450)
				say_item_vnum_ex(get_changing_weapon(1, 19), 0, 3)
				say_item_vnum_ex(get_changing_weapon(1, 20), 1, 3)
				say_item_vnum_ex(get_changing_weapon(1, 21), 22, 30)
				local s = select(item_name(get_changing_weapon(1, 19)), item_name(get_changing_weapon(1, 20)), item_name(get_changing_weapon(1, 21)), "Henüz Deðil.")		
				if s == 1 then
					pc.give_item2_select(get_changing_weapon(1, 19), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(1, 19))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)
				elseif s == 2 then
					pc.give_item2_select(get_changing_weapon(1, 20), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(1, 20))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)
				elseif s == 3 then
					pc.give_item2_select(get_changing_weapon(1, 21), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(1, 21))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)					
				else
					send_letter("~Genç Kahramanlar Ýçin Silahlar")
				end
			elseif pc.job == 2 then
				say_item_vnum(get_changing_weapon(2, 7))
				say("")
				say("")
				say("")
				local s = select(item_name(get_changing_weapon(2, 7)), "Henüz Deðil.")						
				if s == 1 then
					pc.give_item2_select(get_changing_weapon(2, 7), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(2, 7))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)
				else
					send_letter("~Genç Kahramanlar Ýçin Silahlar")
				end
			elseif pc.job == 3 then
				say_item_vnum_ex(get_changing_weapon(3, 13), 0, 2)
				say_item_vnum_ex(get_changing_weapon(3, 14), 1, 2)
				local s = select(item_name(get_changing_weapon(3, 13)), item_name(get_changing_weapon(3, 14)), "Henüz Deðil.")
				if s == 1 then
					pc.give_item2_select(get_changing_weapon(3, 13), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(3, 13))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)
				elseif s == 2 then
					pc.give_item2_select(get_changing_weapon(3, 14), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(3, 14))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)			
				else
					send_letter("~Genç Kahramanlar Ýçin Silahlar")
				end
			elseif pc.job == 4 then
				say_item_vnum(get_changing_weapon(4, 7))
				say("")
				say("")
				say("")
				local s = select(item_name(get_changing_weapon(4, 7)), "Henüz Deðil.")						
				if s == 1 then
					pc.give_item2_select(get_changing_weapon(4, 7), 1)
					item.set_basic(true)
					say_title("Genç Kahramanlar Ýçin Silahlar")
					say("")
					say("Silahýn mühürlenerek karakterine eklendi.")
					say_item_vnum(get_changing_weapon(4, 7))
					clear_letter()
					set_quest_state("weapon_quest_lv70","run")
					set_state(__COMPLETE__)
				else
					send_letter("~Genç Kahramanlar Ýçin Silahlar")
				end
			end
		end
	end
	state __COMPLETE__ begin
	end
end
			