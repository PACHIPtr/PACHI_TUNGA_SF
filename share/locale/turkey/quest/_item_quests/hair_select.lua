--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest hair_select begin
	state start begin
		when 74001.use or 74002.use or 74003.use or 74004.use or 74005.use or 74006.use or 74007.use or 74008.use or 74009.use or 74010.use or 74011.use or 74012.use or 74251.use or 74252.use or 74253.use or 74254.use or 74255.use or 74256.use or 74257.use or 74258.use or 74259.use or 74260.use or 74261.use or 74262.use or
				74501.use or 74502.use or 74503.use or 74504.use or 74505.use or 74506.use or 74507.use or 74508.use or 74509.use or 74510.use or 74511.use or 74512.use or 74751.use or 74752.use or 74753.use or 74754.use or 74755.use or 74756.use or 74757.use or 74758.use or 74759.use or 74760.use or 74761.use or 74762.use or
				75001.use or 75002.use or 75003.use or 75004.use or 75005.use or 75006.use or 75007.use or 75008.use or 75009.use or 75010.use or 75011.use or 75012.use or 75201.use or 75202.use or 75203.use or 75204.use or 75205.use or 75206.use or 75207.use or 75208.use or 75209.use or 75210.use or 75211.use or 75212.use or 
				75401.use or 75402.use or 75403.use or 75404.use or 75405.use or 75406.use or 75407.use or 75408.use or 75409.use or 75410.use or 75411.use or 75412.use or 75601.use or 75602.use or 75603.use or 75604.use or 75605.use or 75606.use or 75607.use or 75608.use or 75609.use or 75610.use or 75611.use or 75612.use or 74521.use or
				74522.use or 74523.use or 74524.use or 74525.use or 74526.use or 74527.use or 74528.use or 74529.use or 74530.use or 74531.use or 74532.use or 74533.use or 74534.use or 74535.use or 74536.use or 74537.use or 74538.use or 74539.use or 74540.use or 74541.use or 74542.use or 74543.use or 74544.use or 74545.use or 74546.use or
				74547.use or 74548.use or 74549.use or 74550.use or 74551.use or 74552.use or 74553.use or 74554.use or 74555.use or 74556.use or 74557.use or 74558.use or 74559.use begin
			say_size(350,375)
			say_title("Saç Stili: ")
			say("")
			say_reward("Saç stiline sadece bir adet bonus ekleyebilirsin.")
			say_reward("Hangi bonusu eklemek istiyorsun ?")
			local s = select("%10 Savaþçýlara Karþý Güçlü ","%10 Ninjalara Karþý Güçlü ","%10 Suralara Karþý Güçlü ","%10 Þamanlara Karþý Güçlü ","%10 Lycanlara Karþý Güçlü ","%2 HP Artýþý ","%2 SP Artýþý ","Daha Sonra Seç ")
			local bonus = "Savaþçýlara Karþý Güçlü "
			if s == 1 then
				item.set_attribute(0, apply.ATTBONUS_WARRIOR, 10)
				bonus = "%10 Savaþçýlara Karþý Güçlü "
			elseif s == 2 then
				item.set_attribute(0, apply.ATTBONUS_ASSASSIN, 10)
				bonus = "%10 Ninjalara Karþý Güçlü "
			elseif s == 3 then
				item.set_attribute(0, apply.ATTBONUS_SURA, 10)
				bonus = "%10 Suralara Karþý Güçlü "
			elseif s == 4 then
				item.set_attribute(0, apply.ATTBONUS_SHAMAN, 10)
				bonus = "%10 Þamanlara Karþý Güçlü "
			elseif s == 5 then
				item.set_attribute(0, 95, 10)
				bonus = "%10 Lycanlara Karþý Güçlü "
			elseif s == 6 then
				item.set_attribute(0, apply.MAX_HP_PCT, 2)
				bonus = "%2 HP Artýþý "
			elseif s == 7 then
				item.set_attribute(0, apply.MAX_SP_PCT, 2)
				bonus = "%2 SP Artýþý "
			else
				return
			end
			say_title("Saç Stili: ")
			say("")
			say_reward(string.format("Eklenen bonus: %s", bonus))
			say_reward("Bonus ekleme iþlemi baþarýlý!")
		end
	end
end