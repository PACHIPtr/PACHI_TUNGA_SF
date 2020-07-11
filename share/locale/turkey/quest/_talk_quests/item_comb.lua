--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest item_comb begin
	state start begin
		when 20406.chat."Kostüm bonuslarýný aktar" with utils.is_feature_combination_enabled() == true begin
			say("Bonuslarýný seçtiðin kostüm 2'den kostüm 1'e")
			say("aktarabilirsin. Önce 'kostüm bonusu aktarýmý' ")
			say("eþyasýný içine çekerek pencereyi etkinleþtir.")
			say("sonra kostümleri seçebilirsin.")
			say("Bonusun kostüm 2'den kostüm 1'e aktarýlmasý iþlemi")
			say("Sýrasýnda Kostüm 2 yok edilir. Devam et?")
			say("")
			local confirm = select("Evet.", "Hayýr")
			if confirm == 2 then
				return
			end
			setskin(NOWINDOW)
			command("item_comb open_costume")
		end

		when 20095.chat."Kitaplarý nasýl takas ederim? " with utils.is_feature_combination_enabled() == true begin
			say('Hoþ geldin! ')
			say('Bende kitap mý takas edeceksin? Harika! Beceri ve ')
			say('Ustalýk kitaplarýný her zaman alýrým. Bana ')
			say('bunlardan 10 tane getirirsen sana sýnýfýna uygun ')
			say('bir beceri kitabý veririm. Adil bir pazarlýk, ')
			say('deðil mi? Ha, neredeyse söylemeyi unutuyordum, bu ')
			say('Takas için 1.000.000 Yang isterim. ')
			say()
		end

		when 20095.chat."Kitap takas et " with utils.is_feature_combination_enabled() == true begin
            setskin(NOWINDOW)
			command("item_comb open_skillbook")
		end
	end
end
