--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest charge_cash_by_voucher begin
	state start begin
		function charge(amount, charge_type)
			if charge_type == nil then 
				charge_type = "cash"
			end

			if 0 < amount then
				-- syntax : pc.charge_cash(amount, charge_type)
				-- warning: 	1. 'charge_type' : "cash"(default) | "mileage"
				-- 	   	2. 'amount' must be positive number.
				local result = pc.charge_cash(amount, charge_type)

				if true == result then
					local item_id = item.get_id()
					char_log(item_id, "CHARGE_CASH_BY_VOUCHER", amount)
					syschat(string.format("%s Ejderha Parasý Kazandýn.", amount))

					local flag_name = "total_" .. charge_type
					pc.setqf(flag_name, pc.getqf(flag_name) + amount)

					if item.get_count() > 1 then
						item.set_count(item.get_count()-1)
					else
						item.remove()
					end
				end
			end

			return false
		end
	
		when 80014.use or 80015.use or 80016.use or 80017.use with utils.is_feature_dc_coupon_enabled() == true begin
			local amount = item.get_value(0)
			
			if pc.is_busy() == true then
				say_title("EP Kuponu: ")
				say("")
				say_reward("Meþgul durumda olduðunuz için EP Kuponunu kullanamazsýnýz.")
			elseif pc.is_dead() == true then
				say_title("EP Kuponu: ")
				say("")
				say_reward("Ölü durumda olduðunuz için EP Kuponunu kullanamazsýnýz.")
			else
				charge_cash_by_voucher.charge(amount, "cash")
			end
		end
	end
end
