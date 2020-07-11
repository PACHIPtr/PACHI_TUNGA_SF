--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest lonca_turnuva begin
	state start begin
		when login with pc.getqf("turnuvadasin") == 0 and pc.isguildmaster() and game.get_event_flag("turnuva") == 1 and pc.getqf("uyarildin") == 0 begin
			send_letter("*Lonca Turnuvası ")
		end
		when button or info begin
			addimage(20, 12, "gtournament.tga")
			say("")
			say("")
			say("")
			say("")
			say("Savaş sorumlusu seni bekliyor.")
			say("")
			say_reward("Arven2 Takımın.")
			send_letter("*Lonca Turnuvası Turnuvası ")
		end
		
		when login or levelup or logout with game.get_event_flag("turnuva") == 0 begin
			if pc.getqf("turnuvadasin") == 1 then
			pc.delqf("turnuvadasin")
			pc.delqf("uyarildin")
			pc.delqf("odulver")
			elseif pc.getqf("uyarildin") == 1 then
			pc.delqf("turnuvadasin")
			pc.delqf("uyarildin")
			pc.delqf("odulver")
			elseif pc.getqf("odulver") == 1 then
			pc.delqf("turnuvadasin")
			pc.delqf("uyarildin")
			pc.delqf("odulver")		
			end
		end
		
		when 11001.chat."Lonca Savaşları Turnuvası ödülü " or 11003.chat."Lonca Savaşları Turnuvası ödülü " or 11005.chat."Lonca Savaşları Turnuvası ödülü " with game.get_event_flag("turnuva") == 3 begin
			if game.get_event_flag(guild.name(pc.get_guild())) == 1 then
				if pc.getqf("odulver") == 0 then
				say_title("Savaş Sorumlusu:")
				say("")
				say("Seni ve loncanı tebrik ederim "..pc.get_name().."")
				say("Gerçekten bu ödülü hak ettiniz. Bir sonraki")
				say("turnuvada görüşmek dileğiyle.")
				say("")
				pc.give_item2("80014",1)
				pc.give_item2("80014",1)
				pc.give_item2("80014",1)
				pc.give_item2("83008",1)
				pc.give_item2("83008",1)
				pc.setqf("odulver",1)
				elseif pc.getqf("odulver") == 1 then
				say_title("Savaş Sorumlusu:")
				say("")
				say("Sen zaten ödülünü almamış mıydın?")
				say("Yeterli gelmediyse bir dahaki turnuvayı ")
				say("kazanmak için çalış.")
				say("")
				end
			else
			say_title("Savaş Sorumlusu:")
			say("")
			say("Üzgünüm, bu turnuvayı sen kazanmadın.")
			say("Ödülü de sen almayacaksın.")
			say("")
			end
		end
		
		when 11001.chat."Turnuvaya katılanların listesine bak" or 11003.chat."Turnuvaya katılanların listesine bak" or 11005.chat."Turnuvaya katılanların listesine bak" with game.get_event_flag("turnuva") == 1 and pc.hasguild() begin
		local guild_list = game.mysql_query("SELECT * FROM player.guild_tournement WHERE guild_tournement.guild_name NOT LIKE '[%]%' ORDER BY guild_tournement.number DESC")
		local inbox_table = {}
			for i = 1,table.getn(guild_list),1 do
			table.insert(inbox_table,guild_list.guild_name[i])
			end
		table.insert(inbox_table,"Kapat")
			if(table.getn(guild_list) == 0) then
			say_title("Savaş Sorumlusu:")
			say("Henüz turnuvaya katılan lonca yok.")
			return
			end
		local m = select_table(inbox_table)
			if(table.getn(inbox_table) == m) then
			return
			elseif(table.getn(inbox_table) >= m) then
			say_title(guild_list.guild_name[m].." loncası ")
			say("")
			say("Lonca lideri: "..guild_list.guild_master[m]..", Krallık: "..guild_list.empire[m].."")
			say("Üye sayısı: "..guild.get_member_count(guild_list.guild_id[m])..", Sıralama: "..guild.get_rank(guild_list.guild_id[m]).."")
			say("")
			return
			end
		end
		
		when 11001.chat."Lonca Savaşları Turnuvası'na katıl" or 11003.chat."Lonca Savaşları Turnuvası'na katıl" or 11005.chat."Lonca Savaşları Turnuvası'na katıl" with game.get_event_flag("turnuva") == 1 and pc.isguildmaster() begin
			if pc.getqf("turnuvadasin") == 1 then
			say_title("Savaş Sorumlusu:")
			say("")
			say("Zaten turnuvaya kayıt oldun. Savaş zamanını ")
			say("bekle.")
			say("")
			return
			else
			say_title("Savaş Sorumlusu:")
			say("")
			say("Hey, loncana güveniyor musun? Dostların gerçekten")
			say("güçlü mü? Seni yarı yolda bırakmayacaklarına")
			say("emin misin? O zaman Lonca Savaşları Turnuvası'na")
			say("katılarak en güçlünün "..guild.name(pc.get_guild()).." loncası ")
			say("olduğunu ispatla!")
			say("")
			wait()
				if guild.level(pc.get_guild()) < 20 and not pc.is_gm() then
				say_title("Savaş Sorumlusu:")
				say("")
				say("Lonca seviyesinin 20 olması gerekiyor.")
				say("Bu haldeyken turnuvaya katılmanıza gerek yok.")
				say("Zaten kazanma şansınızın olmadığı açıkça")
				say("görülüyor. Belki loncanız için biraz daha")
				say("fedakârlık etmelisiniz.")
				elseif guild.get_member_count(pc.get_guild()) < 30 and not pc.is_gm() then
				say_title("Savaş Sorumlusu:")
				say("")
				say("Loncanın en az 30 üyeye sahip olması gerekir.")
				say("")
				else
				say_title("Savaş Sorumlusu:")
				say("")
				say("Herhangi bir engel görünmüyor. Dilersen")
				say("loncanı turnuvaya hemen kaydedebilirim.")
				say("Gerçekten katılmak istiyor musun?")
				say("")
				local gardas = select ("Evet, istiyorum.","Hayır, katılmayacağım.")
					if gardas == 1 then
					say_title("Savaş Sorumlusu:")
					say("")
					say_reward("Lonca savaşına en az 10 oyuncu katılmalı.")
					say_reward("5 dakika oyuncuların alana giriş yapması ")
					say_reward("için beklenir. Eğer 10 kişi tamamlanmazsa")
					say_reward("lonca turnuvadan elenir.")
					say("")
						if not pc.is_gm() then
						pc.setqf("turnuvadasin",1)
						end
					lonca_turnuva.guild_tournement_create()
					notice_all(""..guild.name(pc.get_guild())..", Lonca Savaşları Turnuvası'na katılıyor!")
					return
					else
					return
					end
				end
			end
		end
		
		function guild_tournement_create()
			if pc.get_empire() == 1 then
			game.mysql_query("INSERT INTO guild_tournement(guild_id,guild_name,guild_master,empire) VALUES ('"..pc.get_guild().."','"..guild.name(pc.get_guild()).."','"..pc.get_name().."','Shinso')")
			elseif pc.get_empire() == 2 then
			game.mysql_query("INSERT INTO guild_tournement(guild_id,guild_name,guild_master,empire) VALUES ('"..pc.get_guild().."','"..guild.name(pc.get_guild()).."','"..pc.get_name().."','Chunjo')")
			elseif pc.get_empire() == 3 then
			game.mysql_query("INSERT INTO guild_tournement(guild_id,guild_name,guild_master,empire) VALUES ('"..pc.get_guild().."','"..guild.name(pc.get_guild()).."','"..pc.get_name().."','Jinno')")
			end
		end
	end
end