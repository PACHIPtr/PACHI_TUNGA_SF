--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest goto_empire_castle begin
	state start begin
		when 20097.chat."Kale Savaşı " begin

			local empire = pc.get_empire()
			if empire==1 then
				a= select("Milaard kalesi", "Listhmos kalesi ", "Dendera kalesi ", "İptal")
				if 1==a then
					
					say("Milaard kalesine ışınlanıyorsun... ")
					wait()
					pc.warp(975200, 133000)
				elseif 2==a then
					say("Listhmos kalesine ışınlanıyorsun... ")
					wait()
					pc.warp(978000, 199500)
				elseif 3==a then
					say("Dendera kalesine ışınlanıyorsun... ")
					wait()
					pc.warp(967900, 199400)
				else
					return
				end
			else  
				say("Bu kapıdan sadece Shinsoo İmparatorluğu'nun[ENTER]cesur savaşçıları geçebilir. ")
			end
		end

		when 20098.chat."Kale Savaşı " begin

			local empire = pc.get_empire()
			if empire==2 then
				a= select("Milaard kalesi", "Listhmos kalesi ", "Dendera kalesi ", "İptal")
				if 1==a then
					say("Milaard kalesi için ışınlan. ")
					wait()
					pc.warp(977400, 150100)
				elseif 2==a then
					say("Listhmos kalesi için ışınlan. ")
					wait()
					pc.warp(977200, 185000)
				elseif 3==a then
					say("Dendera kalesine ışınlanıyorsun... ")
					wait()
					pc.warp(925600, 201200)
				else
					return
				end	
			else  
				say("Bu kapıdan sadece Chunjo İmparatorluğu'nun[ENTER]cesur savaşçıları geçebilir. ")
			end
		end
		when 20099.chat."Kale Savaşı " begin

			local empire = pc.get_empire()
			if empire==3 then
				a= select("Milaard kalesi", "Listhmos kalesi ", "Dendera kalesi ", "İptal")
				if 1==a then
					say("Milaard kalesi için ışınlan. ")
					wait()
					pc.warp(977400, 150100)
				elseif 2==a then
					say("Listhmos kalesi için ışınlan. ")
					wait()
					pc.warp(977200, 185000)
				elseif 3==a then
					say("Dendera kalesine ışınlanıyorsun... ")
					wait()
					pc.warp(925600, 201200)
				else
					return
				end	
			else  
				say("Bu kapıdan sadece Jinno İmparatorluğu'nun[ENTER]cesur savaşçıları geçebilir. ")
			end
		end
	end
end
