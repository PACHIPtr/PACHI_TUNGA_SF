--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest boss_parchment begin
	state start begin
		-- mavi ejderha
		when 82004.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82104,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82104,1)
					end
				end
			end
		end
		when 82104.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(2493, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- setaou generali
		when 82005.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82105,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82105,1)
					end
				end
			end
		end
		when 82105.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(2492, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- kirmizi ejderha
		when 82006.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82106,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82106,1)
					end
				end
			end
		end
		when 82106.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(2291, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- alev kral
		when 82007.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82107,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82107,1)
					end
				end
			end
		end
		when 82107.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(2206, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- dokuz kuyruk
		when 82008.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82108,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82108,1)
					end
				end
			end
		end
		when 82108.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(1901, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- dev col kaplumbagasi
		when 82009.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82109,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82109,1)
					end
				end
			end
		end
		when 82109.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(2191, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- maymun lordu
		when 82010.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82110,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82110,1)
					end
				end
			end
		end
		when 82110.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(5163, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- nemere
		when 82011.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82111,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82111,1)
					end
				end
			end
		end
		when 82111.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(6191, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- buz cadisi parsomeni
		when 82012.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82112,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82112,1)
					end
				end
			end
		end
		when 82112.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(1192, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- karanlik lider parsomeni
		when 82013.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82113,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82113,1)
					end
				end
			end
		end
		when 82113.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(791, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- ork reisi parsomeni
		when 82014.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82114,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82114,1)
					end
				end
			end
		end
		when 82114.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(691, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- lusifer parsomeni
		when 82015.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82115,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82115,1)
					end
				end
			end
		end
		when 82115.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(1093, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- kralice orumcek parsomeni
		when 82016.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82116,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82116,1)
					end
				end
			end
		end
		when 82116.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(2091, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- jotun thyrim parsomeni
		when 82017.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82117,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82117,1)
					end
				end
			end
		end
		when 82117.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(6192, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- en tai hukumdari parsomeni
		when 82018.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82118,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82118,1)
					end
				end
			end
		end
		when 82118.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(6407, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- general kappa parsomeni
		when 82019.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82119,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82119,1)
					end
				end
			end
		end
		when 82119.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(3490, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- triton parsomeni
		when 82020.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82120,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82120,1)
					end
				end
			end
		end
		when 82120.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(3491, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- general istakoz parsomeni
		when 82021.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82121,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82121,1)
					end
				end
			end
		end
		when 82121.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(3690, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- kral yengec parsomeni
		when 82022.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82122,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82122,1)
					end
				end
			end
		end
		when 82122.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(3691, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- arges parsomeni
		when 82023.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82123,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82123,1)
					end
				end
			end
		end
		when 82123.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(3190, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- razador parsomeni
		when 82024.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82124,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82124,1)
					end
				end
			end
		end
		when 82124.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(6091, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- kizil reis parsomeni
		when 82026.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82126,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82126,1)
					end
				end
			end
		end
		when 82126.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(3591, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
		-- orumcek barones parsomeni
		when 82027.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			else
				if item.get_count() >= 25 then
					if item.get_count() == 25 then
						item.remove()
						pc.give_item2(82127,1)
					else
						item.set_count(item.get_count()-25)
						pc.give_item2(82127,1)
					end
				end
			end
		end
		when 82127.use begin
			if pc.is_busy() == true then
				syschat("Meþgul durumda iken bunu kullanamazsýn.")
			elseif pc.is_dead() == true then
				syschat("Ölü durumda iken bunu kullanamazsýn.")
			elseif pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 or pc.get_map_index() == 3 or pc.get_map_index() == 23 or pc.get_map_index() == 43 then
				syschat("Bu haritada bu eþyayý kullanamazsýn.")
			else
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
				spawn_mob0(2092, pc.get_map_index(), pc.get_local_x(), pc.get_local_y())
			end
		end
	end
end