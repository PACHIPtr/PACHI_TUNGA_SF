--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest gaya begin
    state start begin
        function CreateGaya()
			say("Bana 10 süs taþý, 5000 Yang ve ")
			say("+0, +1, +2, ya da +3.basamak bir zihin taþý lazým, ")
			say("bunlarla kýymetli bir Gaya üretebilirim. ")
			say("")
			say("Baþarý oraným 30%, ha, maliyeti de 5000 Yang. ")
			say("Devam edeyim mi? ")
			say("")
			
			local s = select("Evet", "Hayýr")
			if s == 1 then
				if pc.get_gold() < 5000 then
					return
				end

				local SELECTED_CELL = select_item_ex()
				if SELECTED_CELL == 6969 then
					return
				end
				
				local old_item = item.get_id()
				if not item.select_cell_ex(SELECTED_CELL) then
					say("Bu eþya gerekmiyor.")
					say("")
					return
				end

				if item.vnum < 28000 or item.vnum >= 28400 then
					say("Sadece taþ eþyalarý gerekli.")
					say("")
					return
				end

				item.select(old_item)
				
				if pc.create_gaya(5000, 30, SELECTED_CELL, 10) then
					syschat("[Gaya] Kazanýlan Gaya: 1")
					syschat("[Gaya] Bileme baþarýlý.")
				else
					syschat("[Gaya] Bileme baþarýsýz.")
				end
				
				gaya.CreateGayaWithoutText()
			end
		end
		
        function CreateGayaWithoutText()
			local SELECTED_CELL = select_item_ex()
			if SELECTED_CELL == 6969 then
				return
			end
			
			local old_item = item.get_id()
			if not item.select_cell_ex(SELECTED_CELL) then
				say("Bir eþya seçmelisin.")
				say("")
				return
			end

			if item.vnum < 28000 or item.vnum >= 28400 then
				say("Yanlýþ bir eþya tercih ettin.")
				say("")
				return
			end

			item.select(old_item)
			
			if pc.create_gaya(5000, 30, SELECTED_CELL, 10) then
				syschat("[Gaya] Kazanýlan: 1")
				syschat("[Gaya] Bileme baþarýlý.")
			else
				syschat("[Gaya] Bileme baþarýsýz.")
			end
			
			gaya.CreateGayaWithoutText()
		end

		-- when 20503.take with item.vnum == 50926 begin
			-- gaya.CreateGaya()
		-- end

        -- when 20503.click begin
			-- local s=select("Süs Taþý Bile", "Kapat")
			-- if s==1 then
				-- say("Süs taþlarýný yontararak Gaya haline getirmek ")
				-- say("istiyor musun? ")
				-- say("Lütfen bana bu kadar süs taþý ver: 10 ")
			-- end
        -- end
		
        when 20504.click begin
			local s1=select("Metin Gaya Maðazasý ", "Kapat")
			if s1==1 then
				say("Gaya maðazasýný hiç duydun mu? ")
				say("Hayýr mý? Ah, bir ziyaret etmeye deðer, çünkü ")
				say("burada son derece kiymetli parçalar alabilirsin. ")
				say("Hatta bazen çok arzulanan, nadir hazineler de ")
				say("bulunabiliyor. ")
				say("Gaya maðazasýný þimdi açmak istiyor musun? ")
				say("")
				local s2=select("Evet", "Hayýr")
				if s2==1 then
					pc.open_gem_shop(0)
				end
			end
        end
		
        when 20503.click begin
			local s1=select("Boss Gaya Maðazasý ", "Kapat")
			if s1==1 then
				say("Gaya maðazasýný hiç duydun mu? ")
				say("Hayýr mý? Ah, bir ziyaret etmeye deðer, çünkü ")
				say("burada son derece kiymetli parçalar alabilirsin. ")
				say("Hatta bazen çok arzulanan, nadir hazineler de ")
				say("bulunabiliyor. ")
				say("Gaya maðazasýný þimdi açmak istiyor musun? ")
				say("")
				local s2=select("Evet", "Hayýr")
				if s2==1 then
					pc.open_gem_shop(1)
				end
			end
        end
		
    end
end
