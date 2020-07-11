--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest istiridye begin
	state start begin
		when 27987.use begin
			if pc.is_busy() == true then
				syschat("Meşgul durumda olduğun için bu işlemi gerçekleştiremezsin.")
				return
			elseif pc.is_dead() == true then
				syschat("Ölü durumda olduğun için bu işlemi gerçekleştiremezsin.")
				return
			end
			if pc.get_empty_inventory_count() >= 1 then
				pc.remove_item("27987",1)
				local cikacaklar = {
				[1] = {0,0}, -- Boş
				[2] = {0,0}, -- Boş
				[3] = {0,0}, -- Boş
				[4] = {0,0}, -- Boş
				[5] = {0,0}, -- Boş
				[6] = {0,0}, -- Boş
				[7] = {0,0}, -- Boş
				[8] = {0,0}, -- Boş
				[9] = {0,0}, -- Boş
				[10] = {1,0}, -- Boş
				[11] = {0,0}, -- Boş
				[12] = {0,0}, -- Boş
				[13] = {0,0}, -- Boş
				[14] = {0,0}, -- Boş
				[15] = {1,27990}, -- Taş
				[16] = {1,27990}, -- Taş
				[17] = {1,27990}, -- Taş
				[18] = {1,27990}, -- Taş
				[19] = {1,27990}, -- Taş
				[20] = {1,27990}, -- Taş
				[21] = {1,27990}, -- Taş
				[21] = {1,27990}, -- Taş
				[22] = {1,27992}, -- Beyaz İnci
				[23] = {1,27993}, -- Mavi İnci
				[24] = {1,27994}, -- Kırmızı İnci
				}
				local sans = number(1, table.getn(cikacaklar))
				if sans >= 1 and sans <= 14 then
					syschat("İstiridye kayboldu. ")
				elseif sans >= 15 and sans <= 21 then
					pc.give_item2("27990",1)
					syschat("İstiridyenin içinde sıradan bir taş parçası var. ")
				elseif sans == 22 then
					pc.give_item2("27992",1)
					syschat("İstiridyenin içinde Beyaz İnci var. ")
				elseif sans == 23 then
					pc.give_item2("27993",1)
					syschat("İstiridyenin içinde Mavi İnci var. ")
				elseif sans == 24 then
					pc.give_item2("27994",1)
					syschat("İstiridyenin içinde Kırmızı İnci var. ")
				end
			else
				syschat("Envanterde yeteri kadar yerin yok!")
			end
		end
	end
end