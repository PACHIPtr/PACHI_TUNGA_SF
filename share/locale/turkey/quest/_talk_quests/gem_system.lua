--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest red_gem_shop_open begin
	state start begin
		when 20504.click begin
			local s1=select("Boss Gaya Maðazasý ", "Close")
			if s1==1 then
				if s1==1 then
					pc.open_gem_shop(0)
				end
			end
		end
		when 20503.click begin
			local s1=select("Metin Gaya Maðazasý ", "Kapat")
			if s1==1 then
				if s1==1 then
					pc.open_gem_shop(1)
				end
			end
		end
	end
end
