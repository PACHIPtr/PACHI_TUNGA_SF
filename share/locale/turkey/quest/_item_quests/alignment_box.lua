--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest alignment_box begin
	state start begin
		when 81500.use begin
			if item.get_socket(1) > 0 then
				if pc.is_busy() == true then
					syschat("Meþgul durumdayken bu iþlemi gerçekleþtiremezsin.")
				elseif pc.is_dead() == true then
					syschat("Ölü durumdayken bu iþlemi gerçekleþtiremezsin.")
				elseif pc.get_alignment() + item.get_socket(1) > 1000000 then
					syschat(string.format("%s'den fazla Derece taþýyamazsýn.", 1000000))
				elseif pc.count_item(81500) < 1 then
					syschat("Envanterinde Derece kutusu bulamadým.")
				else
					pc.change_alignment(item.get_socket(1))
					if item.get_count() > 1 then
						item.set_count(item.get_count()-1)
					else
						item.remove()
					end
				end
			else
				say_title("Derece Kutusu: ")
				say("")
				say_reward("Ne kadar Derece Yatýrmak istiyorsun ?")
				local sinput = tonumber(input())
				if sinput == nil then
					say_title("Derece Kutusu: ")
					say("")
					say_reward("Boþ býrakamazsýn.")
				elseif pc.get_alignment() < sinput then
					say_title("Derece Kutusu: ")
					say("")
					say_reward("Yeterli miktarda Dereceye sahip deðilsin.")
				elseif sinput > 1000000 then
					say_title("Derece Kutusu: ")
					say("")
					say_reward(string.format("%s'den fazla Dereceye yatýramazsýn.", 1000000))
				elseif sinput < 0 then
					say_title("Derece Kutusu: ")
					say("")
					say_reward("0'dan az derece yatýramazsýn.")
				elseif pc.is_busy() == true then
					say_title("Derece Kutusu: ")
					say("")
					say_reward("Meþgulken bu iþlemi gerçekleþtiremezsin.")
				elseif pc.is_dead() == true then
					say_title("Derece Kutusu: ")
					say("")
					say_reward("Ölü durumdayken bu iþlemi gerçekleþtiremezsin.")
				else
					if pc.count_item(81500) < 1 then
						say_title("Derece Kutusu: ")
						say("")
						say_reward("Envanterinde Derece Kutusu bulamadým.")
					else
						pc.change_alignment(-sinput)
						item.set_socket(1, sinput)
						syschat(string.format("Baþarýlý bir þekilde kutuya %s Derece aktardýn.",sinput))
					end
				end
			end
		end
	end
end