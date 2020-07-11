--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest dungeon_manager begin
	state start begin
		when login or enter begin
			dungeonLib.update()
		end

		when button begin
			cmdchat("GetDungeonInfo INPUT#1")
			local cmd = split(input(cmdchat("GetDungeonInfo CMD#")), "#")
			cmdchat("GetDungeonInfo INPUT#0")

			if cmd[1] == "WARP" then
				pc.warp(tonumber(cmd[2]) * 100, tonumber(cmd[3]) * 100)
			end
		end
	end
end