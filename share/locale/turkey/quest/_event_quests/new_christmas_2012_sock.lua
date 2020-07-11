--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest new_christmas_2012_sock begin
	state start begin
	end
	state baby_dear_give begin
		when 20126.chat."Çorap ile ne? " begin
			say_title(mob_name(20126))
			say ("Çorap! Neredeyse unutuyordum![ENTER]Çoraplarýmý her zaman Noel Aðacý'nýn[ENTER]üzerine asýyordum. Ama yaratýklar hepsini çaldýlar![ENTER][ENTER]Onlarý geri al, lütfen. ")
			wait()
			say_title(mob_name(20126))
			say ("Sana Ren Geyiði Yavrusu veriyorum.")
			wait()
			say_title(mob_name(20126))
			say ("Çok tatlý deðil mi?[ENTER]Ve mükemmel bir koku alma duyusu vardýr. Bir çorap uzmaný bulun![ENTER]Sizinle 2 saat boyunca kalýr ve arayýþý içinde size yardýmcý olur. ")
			wait()
			say_title(mob_name(20126))
			pc.give_item2_select (53002, 1)
			item.set_socket (0, get_global_time() + 21600)
			pc.setqf ("last_reindeer_give_time", get_global_time())
			set_state(sock_take)
		end
	end
	state sock_take begin
		when 20126.chat."Genç Ren Geyiði " begin
			if get_global_time() - pc.getqf ("last_reindeer_give_time") < 86400 then
				say_title (mob_name (20126))
                say ("Bugün zaten bir Ren Geyiði aldýn.[ENTER]Yarýna kadar beklemelisin.[ENTER]Eðer beklemek istemiyorsan,[ENTER]aþaðýdaki dönen paraya týklayýp alabilirsin. ")
			else
				say_title (mob_name (20126))
                say ("Çok tatlý deðil mi?[ENTER]Ve mükemmel bir koku alma duyusu vardýr. Bir çorap uzmaný buldun![ENTER]Sizinle 2 saat boyunca kalýr ve arayýþý içinde size yardýmcý olur. ")
				pc.give_item2_select (53002, 1)
				item.set_socket (0, get_global_time() + 21600)
				pc.setqf ("last_reindeer_give_time", get_global_time())
			end
		end
		
		when 20126.chat."reset flags / reindeer, socks" with is_test_server() begin
			pc.setqf ("last_reindeer_give_time", 0)
			pc.setqf("last_sock_cooldown_time", 0) 
		end
		
		when 20126.chat."Bana çorap getir! " begin
			local n = pc.count_item(50010)
			if n < 25 then
				say_title(mob_name(20126))
				say ("Bana en az 5 çorap getir.[ENTER]Karþýlýðýnda bir ödül vereceðim.[ENTER]Urgh, yýpranmýþ çorap kokusu dayanýlmaz![ENTER]Mutlu noeller! ")
				wait()
			else
				say_title(mob_name(20126))
				say ("Ha ha ha yeterince birikmiþ çorap var.[ENTER]Sonra bakayým.[ENTER]Urrgh, bu koku ... Bu kesinlikle doðru çorap![ENTER]Mutlu noeller! ")
				wait()
				pc.remove_item (50010, 5)
				say_title(locale.reward)
				
				pc.give_item2(71144)
				say_reward (string.format("%s : 1%s",item_name(71144), locale.count_postfix))
			end
		end
	end
end
