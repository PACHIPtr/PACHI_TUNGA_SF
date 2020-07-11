--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest skill_book_chests begin
	state start begin
		function give_books(x)
		local a = {}
			if x == 71203 or x == 71204 then
				a = special.active_skill_list[1][1]
			elseif x == 71205 or x == 71206 then
				a = special.active_skill_list[1][2]
			elseif x == 71207 or x == 71208 then
				a = special.active_skill_list[2][1]
			elseif x == 71209 or x == 71210 then
				a = special.active_skill_list[2][2]
			elseif x == 71211 or x == 71212 then
				a = special.active_skill_list[3][1]
			elseif x == 71213 or x == 71214 then
				a = special.active_skill_list[3][2]
			elseif x == 71215 or x == 71216 then
				a = special.active_skill_list[4][1]
			elseif x == 71217 or x == 71218 then
				a = special.active_skill_list[4][2]
			end
			local n = number(1,table.getn(a))	
			pc.give_item2_select("50300",1)
			item.set_socket(0,a[n])
		end
		
		function is_big_chest()
			local x = {71204, 71206, 71208, 71210, 71212, 71214, 71216, 71218}
			for i = 1, table.getn(x) do
				if item.get_vnum() == x[i] then
				return true
				end
			end
			return false
		end
		
		when 71203.use or 71204.use or 71205.use or 71206.use or 71207.use or 71208.use or 71209.use or 71210.use or 71211.use or 71212.use or 71213.use or 71214.use or 71215.use or 71216.use or 71217.use or 71218.use begin
			local a = pc.count_item("50300")
			local b = item.get_vnum()
			if skill_book_chests.is_big_chest() then
				if pc.get_empty_inventory_count() < 25 then
					syschat("Envanterde yeterli boþluk yok.")
					return
				end
			repeat
			skill_book_chests.give_books(b)	
			until pc.count_item("50300") - a == 25
			else
				if pc.get_empty_inventory_count() < 10 then
				syschat("Envanterde yeterli boþluk yok.")
				return
				end
			repeat
			skill_book_chests.give_books(b)
			until pc.count_item("50300") - a == 10
			end
		pc.remove_item(b,1)
		end
	end
end
				