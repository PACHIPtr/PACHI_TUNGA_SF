--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest dragon_soul begin
	state start begin
		when login or levelup with pc.get_level() >= 1 begin
			set_state(farming)
		end
	end
	state farming begin
		when kill with not npc.is_pc() begin
			if pc.get_level() < 30 then
				return
			end
			if pc.get_level() < npc.get_level() - 25 then
				return
			end
			pc.setqf("ds_points",pc.getqf("ds_points")+1)
			if pc.getqf("ds_points") >= 165 then
				pc.give_item2("50255",1)
				syschat(string.format("<Ejderha Taþý Simyasý> Cor puanýn yükseldiði için %s adet %s kazandýn.", 1, item_name(50255)))
				pc.setqf("ds_points", pc.getqf("ds_points")-165)
			end
		end
		when 30270.pick begin
			if pc.count_item(30270) >= 5 then
				pc.remove_item(30270, 5)
				pc.give_item2(50255)
			end
		end
	end
end