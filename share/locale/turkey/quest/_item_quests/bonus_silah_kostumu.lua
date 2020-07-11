--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest bonus_silah_kostumu begin
	state start begin
		when 30339.use begin 
		if pc.count_item(30339) < 1 then 
			say_title("Bug Kontrol ") 
			say_title("Envanterindeki nesne kaybolmuþ. ") 
			return
		end
			local nevi2secbakalim = select("Kýlýç ","Hançer ","Yay ","Çift El ","Çan","Yelpaze ","Pençe ","Kullanmak Ýstemiyorum. ")
			if nevi2secbakalim == 1 then
				local silah1 = select ("Yeþil Ejder Kýlýcý+ ","Ejder Avcýsý Kýlýcý+ ","Anka Kuþu Kýlýcý+ ","Antik Melek Kýlýcý+ ","Þeytan Avcýsý Kýlýcý+ ", "Kullanmak Ýstemiyorum. ")
				if silah1 == 1 then
				pc.give_item2("40300", 1)
				pc.remove_item(30339)
				elseif silah1 == 2 then
				pc.give_item2("40308", 1)
				pc.remove_item(30339)
				elseif silah1 == 3 then
				pc.give_item2("40316", 1)
				pc.remove_item(30339)
				elseif silah1 == 4 then
				pc.give_item2("40324", 1)
				pc.remove_item(30339)
				elseif silah1 == 5 then
				pc.give_item2("40332", 1)
				pc.remove_item(30339)
				elseif silah1 == 6 then return end
			end
			if nevi2secbakalim == 2 then
				local hancer1 = select ("Yeþil Ejder Hançeri+ ","Ejder Avcýsý Hançeri+ ","Anka Kuþu Hançeri+ ","Antik Melek Hançeri+ ","Þeytan Avcýsý Hançeri+ ", "Kullanmak Ýstemiyorum. ")
				if hancer1 == 1 then
				pc.give_item2("40303", 1)
				pc.remove_item(30339)
				elseif hancer1 == 2 then
				pc.give_item2("40311", 1)
				pc.remove_item(30339)
				elseif hancer1 == 3 then
				pc.give_item2("40319", 1)
				pc.remove_item(30339)
				elseif hancer1 == 4 then
				pc.give_item2("40327", 1)
				pc.remove_item(30339)
				elseif hancer1 == 5 then
				pc.give_item2("40335", 1)
				pc.remove_item(30339)
				elseif hancer1 == 6 then return end
			end
			if nevi2secbakalim == 3 then
				local yay1 = select ("Yeþil Ejder Yayý+ ","Ejder Avcýsý Yayý+ ","Anka Kuþu Yayý+ ","Antik Melek Yayý+ ","Þeytan Avcýsý Yayý+ ", "Kullanmak Ýstemiyorum. ")
				if yay1 == 1 then
				pc.give_item2("40304", 1)
				pc.remove_item(30339)
				elseif yay1 == 2 then
				pc.give_item2("40312", 1)
				pc.remove_item(30339)
				elseif yay1 == 3 then
				pc.give_item2("40320", 1)
				pc.remove_item(30339)
				elseif yay1 == 4 then
				pc.give_item2("40328", 1)
				pc.remove_item(30339)
				elseif yay1 == 5 then
				pc.give_item2("40336", 1)
				pc.remove_item(30339)
				elseif yay1 == 6 then return end
			end
			if nevi2secbakalim == 4 then
				local ciftel1 = select ("Yeþil Ejder Mýzraðý+ ","Ejder Avcýsý Mýzraðý+ ","Anka Kuþu Mýzraðý+ ","Antik Melek Mýzraðý+ ","Þeytan Avcýsý Mýzraðý+ ", "Kullanmak Ýstemiyorum. ")
				if ciftel1 == 1 then
				pc.give_item2("40302", 1)
				pc.remove_item(30339)
				elseif ciftel1 == 2 then
				pc.give_item2("40310", 1)
				pc.remove_item(30339)
				elseif ciftel1 == 3 then
				pc.give_item2("40318", 1)
				pc.remove_item(30339)
				elseif ciftel1 == 4 then
				pc.give_item2("40326", 1)
				pc.remove_item(30339)
				elseif ciftel1 == 5 then
				pc.give_item2("40334", 1)
				pc.remove_item(30339)
				elseif ciftel1 == 6 then return end
			end
			if nevi2secbakalim == 5 then
				local can1 = select ("Yeþil Ejder Çaný+ ","Ejder Avcýsý Çaný+ ","Anka Kuþu Çaný+ ","Antik Melek Çaný+ ","Þeytan Avcýsý Çaný+ ", "Kullanmak Ýstemiyorum. ")
				if can1 == 1 then
				pc.give_item2("40305", 1)
				pc.remove_item(30339)
				elseif can1 == 2 then
				pc.give_item2("40313", 1)
				pc.remove_item(30339)
				elseif can1 == 3 then
				pc.give_item2("40321", 1)
				pc.remove_item(30339)
				elseif can1 == 4 then
				pc.give_item2("40329", 1)
				pc.remove_item(30339)
				elseif can1 == 5 then
				pc.give_item2("40337", 1)
				pc.remove_item(30339)
				elseif can1 == 6 then return end
			end
			if nevi2secbakalim == 6 then
				local yelpaze1 = select ("Yeþil Ejder Yelpazesi+ ","Ejder Avcýsý Yelpazesi+ ","Anka Kuþu Yelpazesi+ ","Antik Melek Yelpazesi+ ","Þeytan Avcýsý Yelpazesi+ ", "Kullanmak Ýstemiyorum. ")
				if yelpaze1 == 1 then
				pc.give_item2("40306", 1)
				pc.remove_item(30339)
				elseif yelpaze1 == 2 then
				pc.give_item2("40314", 1)
				pc.remove_item(30339)
				elseif yelpaze1 == 3 then
				pc.give_item2("40322", 1)
				pc.remove_item(30339)
				elseif yelpaze1 == 4 then
				pc.give_item2("40330", 1)
				pc.remove_item(30339)
				elseif yelpaze1 == 5 then
				pc.give_item2("40338", 1)
				pc.remove_item(30339)
				elseif yelpaze1 == 6 then return end
			end
			if nevi2secbakalim == 7 then
				local pence1 = select ("Yeþil Ejder Pençesi+ ","Ejder Avcýsý Pençesi+ ","Anka Kuþu Pençesi+ ","Antik Melek Pençesi+ ","Þeytan Avcýsý Pençesi+ ", "Kullanmak Ýstemiyorum. ")
				if pence1 == 1 then
				pc.give_item2("40307", 1)
				pc.remove_item(30339)
				elseif pence1 == 2 then
				pc.give_item2("40315", 1)
				pc.remove_item(30339)
				elseif pence1 == 3 then
				pc.give_item2("40323", 1)
				pc.remove_item(30339)
				elseif pence1 == 4 then
				pc.give_item2("40331", 1)
				pc.remove_item(30339)
				elseif pence1 == 5 then
				pc.give_item2("40339", 1)
				pc.remove_item(30339)
				elseif pence1 == 6 then return end
			end
			end
	end end