--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest new_easter_gift_box_gf begin
	state start begin
	
		function give_item_with_log(vnum, amount,item_id)
			if vnum == 1 then 
				pc.changegold(amount)
			else
				pc.give_item2(vnum,amount)
			end
			char_log(tonumber(item_id), "EASTER_BOX_GF","reward vnum: "..vnum.." reward amount: "..amount);
		end
		

		
		function gift(item_id)
			local n = number (1,100)
			if n <= 2 then
				new_easter_gift_box_gf.give_item_with_log(71032,1,item_id)
			elseif n <= 9 then
				new_easter_gift_box_gf.give_item_with_log(50513,5,item_id)
			elseif n <= 16 then
				new_easter_gift_box_gf.give_item_with_log(72309,1,item_id)
			elseif n <= 18 then
				new_easter_gift_box_gf.give_item_with_log(51501,1,item_id)
			elseif n <= 25 then
				new_easter_gift_box_gf.give_item_with_log(50216,5,item_id)
			elseif n <= 36 then
				new_easter_gift_box_gf.give_item_with_log(41584,1,item_id)
			elseif n <= 42 then
				new_easter_gift_box_gf.give_item_with_log(45274,1,item_id)
			elseif n <= 48 then
				new_easter_gift_box_gf.give_item_with_log(71108,5,item_id)
			elseif n <= 54 then
				new_easter_gift_box_gf.give_item_with_log(71026,1,item_id)
			elseif n <= 60 then
				new_easter_gift_box_gf.give_item_with_log(71135,1,item_id)
			elseif n <= 66 then
				new_easter_gift_box_gf.give_item_with_log(50300,3,item_id)
			elseif n <= 77 then
				new_easter_gift_box_gf.give_item_with_log(70056,5,item_id)
			elseif n <= 86 then
				new_easter_gift_box_gf.give_item_with_log(27993,1,item_id)
			elseif n <= 94 then
				new_easter_gift_box_gf.give_item_with_log(27993,1,item_id)
			else
				new_easter_gift_box_gf.give_item_with_log(7994,1,item_id)
			end

		end

		-- ¸¶¹ýÀÇ ´Þ°¿: 71150
		when 71150.use begin
			local t = get_global_time() - item.get_socket(1)
			local n = item.get_socket (0)
			if t >= 1800 then
			--if t >= 60 then
				new_easter_gift_box_gf.gift(item.get_id())
				item.set_socket(1, get_global_time())
				if n == 2 then
					item.remove()
				else
					item.set_socket(0, n + 1)
				end
				syschat (string.format("% D dan, hala% d açabilirsiniz. ", 3, 2 - n))
			else
				syschat (string.format ("Sonraki hediye için % d dakika bekleyin. ", 31 - t / 60))
			end
		end

	end
end

