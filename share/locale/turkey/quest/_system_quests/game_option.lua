--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest game_option begin
	state start begin
		when login begin
			local val=0
			local msg = "Engelle: "

			if pc.getqf("block_exchange") == 1 then
				msg = msg.."Ticaret "
				val = val + 1
			end

			if pc.getqf("block_guild_invite") == 1 then
				val = val + 4
				msg = msg.."Lonca "
			end

			if pc.getqf("block_messenger_invite") == 1 then
				msg = msg.."Arkadaþlar "
				val = val + 16
			end

			if pc.getqf("block_party_invite") == 1 then
				msg = msg.."Grup "
				val = val + 2
			end

			if pc.getqf("block_party_request") == 1 then
				msg = msg.."Rica "
				val = val + 32
			end

			if pc.getqf("block_whisper") == 1 then
				msg = msg.."Fýsýltý "
				val = val + 8
			end
			
			if pc.getqf("block_equipment_request") == 1 then
				msg = msg.."Ekipman "
				val = val + 64
			end
			
			if pc.getqf("hide_health_board") == 1 then
				msg = msg.."Yaþam Göstergesi "
			end
			
			if pc.getqf("hide_level") == 1 then
				msg = msg.."Seviye "
			end
			
			if pc.getqf("hide_costume") == 1 then
				msg = msg.."Zýrh Kostümü "
			end
			
			if pc.getqf("hide_costume_w") == 1 then
				msg = msg.."Silah Kostümü "
			end
			
			if pc.getqf("hide_costume_h") == 1 then
				msg = msg.."Saç Kostümü "
			end
			
			if pc.getqf("disable_gunsam") == 1 then
				msg = msg.."Dans Etkinliði "
			end
			
			if pc.getqf("hide_level") == 1 then
				msg = msg.."Seviye "
			end

			if val != 0 then
				syschat(msg)
			end

			pc.send_block_mode(val)
		end
	end
end
