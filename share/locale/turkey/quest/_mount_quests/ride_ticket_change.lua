--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest ride_ticket_change begin
	state start begin
		when 20349.chat."Sertifikayý binek ile deðiþtir "  begin 
			say_title(mob_name (20349))
			say("Sertifikaný bir binek ile mi deðiþtirmek[ENTER]istiyorsun? Bakalým hangisini getirdin. Çok[ENTER]sayýda sertifikaya sahipsen, önümüzdeki adýmlarda[ENTER]aralarýndan bir tane seçebilirsin. ")
			wait()
			local items = {pc.get_sig_items(10032)}
			local ticket = nil
			if table.getn (items) > 1 then
				for i, v in ipairs (items) do
					item.select (v)
					say_title(mob_name(20349))
					say ("Bu bineði almak istiyorsun, öyle mi?[ENTER]Öyleyse 'Evet' ile onayla ve bir sonraki adýmda onun[ENTER]için bir bonus seç. Sonraki sertifikaya gitmek için 'Hayýr' seç. ")
					say_item_vnum (item.vnum)
					say ("Emin misin? ")
					local s = select ("Evet ", "Hayýr ", "Ýptal ")
					if s == 3 then
						return
					end
					if s == 1 then 
						ticket = v
						break
					end
				end
			else
				ticket = items [1]
			end
			if ticket == nil then
				say_title (mob_name (20349))
				say ("Demek sertifikaný deðiþtirmek istemiyorsun.[ENTER]Hazýr olduðunda tekrar gel. ")
				return 
			end
			say_title (mob_name (20349))
			say("Þimdi de bir bonus seç. ")
			item.select (ticket)
			local s = select ("Canavarlara karþý güç ", "Tecrübe (EXP) ","Hayat Puaný (HP) ", "Savunma (SAV) ", "Saldýrý deðeri (SD) ", "Hayýr, þimdi deðil. ")
			if s == 6 then
				say_title (mob_name (20349))
				say ("Henüz karar veremiyorsun galiba. Hazýr olduðunda[ENTER]tekrar gel. ")
				return
			end
			local summon_item = item.get_value (s-1)
			say_title (mob_name (20349))
			say("Ýyi bir seçim. Baþarýlar! ")
			if pc.give_item2 (summon_item) == 0 then
				say_title (mob_name (20349))
				say ("Hm, bir þey yanlýþ gitmiþ gibi gözüküyor.")
				return
			end
			item.remove()
		end
	end
end
