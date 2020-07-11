--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest change_guild_master begin
	state start begin
		function change_master( name , level_limit , time_resign_limit , time_be_other_leader , time_be_other_member , iscacheitem )
			local ret = guild.change_master_with_limit( name , level_limit , time_resign_limit , time_be_other_leader , time_be_other_member, iscacheitem)
			local result = false

			if ret == 0 then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Yanlýþ isim girdin.")
				say("")
			elseif ret == 1 then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Bu eþyayý sadece lonca liderleri kullanabilir.")
				say("")
			elseif ret == 2 then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Böyle bir lonca üyesi yok.")
				say("")
			elseif ret == 3 then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Lonca lideri baþarýyla deðiþtirildi.")
				say("")
				result = true
			elseif ret == 4 then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Bir loncada deðilsin.")
				say("")
			elseif ret == 5 then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Bu oyuncu online deðil.")
				say("")
			elseif ret == 6 then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Bu oyuncu lonca lideri olmak için yeterince")
				say("güçlü deðil.")
			elseif ret == 7 then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Bu oyuncu lonca lideri olmak için çok")
				say("tecrübesiz.")
			end

			return result
		end
		
		
		when 71099.use begin
			if pc.is_guild_master() == false then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Bu eþyayý sadece lonca liderleri kullanabilir.")
				say("")
				return
			end
			
			say_title("Yer Deðiþtirme Yüzüðü ")
			say("")
			say("Oyuncunun adýný yaz.")
			say("")

			local s = input()

			if s == pc.name then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Kendi adýný yazamazsýn.")
				say("")
				return
			elseif s == "" then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Boþ býrakamazsýn.")
				say("")
				return
			end

			if not pc.is_near_vid( find_pc_by_name( s ) , 10 ) then
				say_title("Yer Deðiþtirme Yüzüðü ")
				say("")
				say("Oyuncu yakýnlarýnda olmalý.")
				say("")
				return
			end

			if change_guild_master.change_master( s , 40 , 0 , 0 , 0 ,true) == true then
				pc.remove_item(71099)
			elseif ret == 5 then
			elseif ret == 6 then
				say_title("")
				say("")
				say("Oyuncunun seviyesi çok düþük.")
			end

			end

		end
end

