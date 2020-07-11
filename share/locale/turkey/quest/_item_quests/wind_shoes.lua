--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest wind_shoes begin
	state start begin
		when 72701.use begin
			if pc.getqf("wind_shoes") > 0 and pc.getqf("wind_shoes") > get_time() then
				syschat("Zaten Rüzgarýn Ayakkabý'sýný kullanmýþsýn.")
				return
			end
			affect.add_wind_shoes(8,30,259200)
			syschat("Rüzgarýn Ayakkabýsý etkisi 72 saat boyunca karakterinde kalacak.")
			item.remove()
			pc.setqf("wind_shoes",get_time()+259200)
			pc.show_effect("d:/ymir work/effect/etc/buff/buff_windschuhe.mse")
		end
	end
end
