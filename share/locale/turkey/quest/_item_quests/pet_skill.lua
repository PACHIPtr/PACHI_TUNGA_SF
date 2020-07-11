--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest petskill begin
	state start begin
		when 55028.use begin
			if newpet.count_summoned() < 1 then
				say_title("Pet Revertus")
				say("")
				say_reward("Evcil hayvanýný çaðýrmýþ olmalýsýn.")
			else
				say_title("Pet Revertus")
				say("")
				say_reward("Kaçýncý beceriyi sýfýrlamak istiyorsun ?")
				say_reward("0 - 1 - 2")
				local input = tonumber(input(''))
				if input == nil then
					say_title("Pet Revertus")
					say("")
					say_reward("Boþ býrakamazsýn.")
				elseif input < 0 then
					say_title("Pet Revertus")
					say("")
					say_reward("Sýfýrdan düþük bir rakam giremezsin.")
				elseif pc.count_item(55028) < 1 then
					say_title("Pet Revertus")
					say("")
					say_reward("Envanterinde Pet Revertus bulamadým.")
				else
					local ret = newpet.resetskill(input)
					if ret == true then
						say_title("Pet Revertus")
						say("")
						say_reward("Evcil Hayvanýnýzýn becerisi baþarýyla sýfýrlandý.")
						say_reward("Evcil hayvanýnýzý gönderin ve tekrar çaðýrýn.")
						pc.remove_item(55028,1)
					else
						say_title("Pet Revertus")
						say("")
						say_reward("Bilinmeyen bir hata oluþtu.")
					end
				end
			end
		end
		when 55029.use begin
			if newpet.count_summoned() < 1 then
				say_title("Pet Revertus")
				say("")
				say_reward("Evcil hayvanýný çaðýrmýþ olmalýsýn.")
			else
				say_title("Pet Revertus")
				say("")
				say_reward("Evcil Hayvanýn Tüm Becerilerini Sýfýrlamak Ýstiyor musun ?")
				local s = select("Evet","Hayýr")
				if s == 1 then
					if pc.count_item(55029) < 1 then
						say_title("Pet Revertus")
						say("")
						say_reward("Envanterinde Pet Beceri Parþomeni bulamadým.")
					else
						newpet.resetskill(0)
						newpet.resetskill(1)
						newpet.resetskill(2)
						say_reward("Evcil Hayvanýnýzýn becerileri sýfýrlandý.")
						say_reward("Evcil hayvanýnýzý gönderin ve tekrar çaðýrýn.")
						pc.remove_item(55029,1)
					end
				else
				end
			end
		end
	end
end