--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest new_valentine_day_gift_box begin
	state start begin

		function give_item_with_log(vnum, amount, item_id)
			if vnum == 1 then 
				pc.changegold(amount)
			else
				pc.give_item2(vnum,amount)
			end
			char_log(tonumber(item_id), "VALENTINE_BOX","reward vnum: "..vnum.." reward amount: "..amount);
		end

		function gift(item_id)
			local n = number (1,100)
			if n <= 4 then
				new_valentine_day_gift_box.give_item_with_log(71025,1,item_id)
			elseif n <= 8 then
				new_valentine_day_gift_box.give_item_with_log(71153,1,item_id)
			elseif n <= 12 then
				new_valentine_day_gift_box.give_item_with_log(100100,1,item_id)
			elseif n <= 16 then
				new_valentine_day_gift_box.give_item_with_log(70005,1,item_id)
			elseif n <= 21 then
				new_valentine_day_gift_box.give_item_with_log(70043,1,item_id)
			elseif n <= 26 then
				new_valentine_day_gift_box.give_item_with_log(71018,1,item_id)
			elseif n <= 31 then
				new_valentine_day_gift_box.give_item_with_log(71019,1,item_id)
			elseif n <= 40 then
				new_valentine_day_gift_box.give_item_with_log(100200,1,item_id)
			elseif n <= 50 then
				new_valentine_day_gift_box.give_item_with_log(1,2000000,item_id)
			elseif n <= 61 then
				new_valentine_day_gift_box.give_item_with_log(1,1000000,item_id)
			elseif n <= 73 then
				new_valentine_day_gift_box.give_item_with_log(1,750000,item_id)
			elseif n <= 86 then
				new_valentine_day_gift_box.give_item_with_log(1,500000,item_id)
			else
				new_valentine_day_gift_box.give_item_with_log(1,250000,item_id)
			end

		end

		-- ¹ß·»Å¸ÀÎ¼±¹°»óÀÚ : 71146-71147
		when 71146.use begin
			new_valentine_day_gift_box.gift(item.get_id())
			local n = item.get_socket(0)
			if n == 9 then
				item.remove()
			else
				item.set_socket(0, n + 1)
			end
			syschat (string.format("%s adet kutudan %s adet açabilirsin.", 10, 9 - n))
		end
		when 71147.use begin
			new_valentine_day_gift_box.gift(item.get_id())
			local n = item.get_socket(0)
			if n == 9 then
				item.remove()
			else
				item.set_socket(0, n + 1)
			end
			syschat (string.format("%s adet kutudan %s adet açabilirsin.", 10, 9 - n))
		end
	end
end

