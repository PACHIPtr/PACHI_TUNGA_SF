--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest snow_dungeon begin
state start begin
    function get_settings()
        local snow_dungeon_settings = {}
        snow_dungeon_settings.map_index = 352                        --Mapindex
        snow_dungeon_settings.base_cord = {5120, 1536 }             --Base Kordinaten 
        snow_dungeon_settings.outside_entry_pos = {61,4327,1668}     --Mapindex, Exit Position (Eisland)
        snow_dungeon_settings.need_level = 90                        --Ab wann ist der Dungeon betretbar.
        snow_dungeon_settings.need_level_group = 90                 --Benötigte Level für Gruppenmitglieder
        snow_dungeon_settings.WaitBeforOutAllOnExit = 60            --Wartezeit bevor alle alle herausteleportiert werden bei einem erfolgreichen Run. (Empfohlen 60)
        snow_dungeon_settings.NextJumpTime = 5                        --Wann man zur nächsten ebene Gejumpt wird (timer) (Empfohlen 5)
        snow_dungeon_settings.DurationLoopTimer = 15                --Welchen intervall die Level und Missionen geprüft werden sollen. (Empfohlen 45)
        snow_dungeon_settings.wait_duration = 60*60*2                --Ab wann der Dungeon wieder betretbar ist (PartyLeader)
        snow_dungeon_settings.maxlife_time_on_dungeon = 60*60*2    --Maximale Zeit für den Dungeon.
        snow_dungeon_settings.DurationMaxOnReJoinGroup = 60*5        --Maximale Zeit innerhalb wann man wieder in die Gruppe joinen kann.
        snow_dungeon_settings.key_level_2 = 30331                    --KeyVnum für die 2.Ebene    --Frostschlüssel
        snow_dungeon_settings.NpcVnum_level_5 = 20398                --MobVnum von 5.Ebene    --Würfel des Arktos
        snow_dungeon_settings.StoneCount_level_5 = 6                --Wie viele Würfel des Arktos sollen erscheinen? 
        snow_dungeon_settings.Key_on_level_5 = 30332                --KeyVnum von 5.Ebene    --Nordstern
        snow_dungeon_settings.StoneCords_level_5 = {
        {432,508},{437,493},{448,492},{448,476},{467,475},{466,464},
        {462,450},{458,428},{446,439},{431,431},{415,416},{402,427},
        {397,438},{375,434},{374,456},{390,468},{385,492},{402,505},
        {404,489},{386,482}
        }    --Positionen von Würfel des Arktos
        snow_dungeon_settings.MobVnum_level_6 = 8058                --MobVnum von 6.Ebene    --Metin der Kälte
        snow_dungeon_settings.MobVnum_level_7 = 6151                --MobVnum von 7.Ebene    --Szel
        snow_dungeon_settings.SzelCords_level_7 = {
        {752,499},{758,479},{772,452},{763,444},{750,451},{728,441},
        {726,455},{718,482},{715,491},{731,473},{748,429}
        } --Positionen von Szel
        snow_dungeon_settings.Key_on_level_8 = 30333                --KeyVnum von 8.Ebene    --Eisblumenschlüssel
        snow_dungeon_settings.NpcVnum_level_9 = 20399                --MobVnum von 9.Ebene    --Säule des Norddrachen
        snow_dungeon_settings.BossVnumGroup = 6062                    --GroupMobVnum vom Boss --(Group.txt)
        snow_dungeon_settings.BossVnum = 6191                        --MobVnum von 10.Ebene    --Nemere(6191)
        
        snow_dungeon_settings.Position_level_1 = { 171,271 }         --Position ebene 1    --Startposition        --Töte alle Monster auf dieser Ebene.
        snow_dungeon_settings.Position_level_2 = { 761,270 }         --Position ebene 2                        --Finde den richtigen Frostschlüssel, nur ein Schamane kann ihn benutzen.
        snow_dungeon_settings.Position_level_3 = { 187,491 }         --Position ebene 3                        --Töte alle Monster auf dieser Ebene.
        snow_dungeon_settings.Position_level_4 = { 421,259 }         --Position ebene 4                        --Töte alle Monster auf dieser Ebene.
        snow_dungeon_settings.Position_level_5 = { 419,530 }         --Position ebene 5                        --Öffnet die Siegel an den Würfel des Arktos in der richtigen Reihenfolge. (Zieht Nordstern auf die Würfel)
        snow_dungeon_settings.Position_level_6 = { 571,706 }         --Position ebene 6                        --Töte alle Monster auf dieser Ebene. Danach Zersöre den Metin der Kälte (Nur ein Schamane kann ihn Schaden)
        snow_dungeon_settings.Position_level_7 = { 746,534 }         --Position ebene 7                        --Töte alle Monster auf dieser Ebene und Szel
        snow_dungeon_settings.Position_level_8 = { 303,710 }         --Position ebene 8                        --Der Eisblumenschlüssel muss gedroppt werden, jedoch kann dieser nur von Schamanen und Ninja benutzt werden.
        snow_dungeon_settings.Position_level_9 = { 848,693  }         --Position ebene 9                        --Zerstört die Säule des Norddrachen. Nur Ninjas und Schamanen können ihm Schaden zufügen.
        snow_dungeon_settings.Position_level_10 = { 927,391 }         --Position ebene 10 (BossZone)            --Besiege Nemere, den König über Frost und Eis.    
        snow_dungeon_settings.Enable_TestServerMode = false            --Ob TestServer mode aktiviert ist. Wenn ja werden die Aufgaben schneller beendet.
        snow_dungeon_settings.IsNewga_me_re_vi_sion = true                --Need ga_me_re_vi_sion 40301
        return snow_dungeon_settings
    end
    function is_TestServerMode()
        local set = snow_dungeon.get_settings()
        return set.Enable_TestServerMode
    end
    function is_nemeres_dungeon()
        local set = snow_dungeon.get_settings()
        local map = pc.get_map_index()
        if map >= (set.map_index * 10000) and map < (set.map_index * 10000 + 9000) then
            return true
        else
            return false
        end
    end
    
	
	-- --------------------- #Zindana devam et ------------------------
	-- pc.setf("snow_dungeon","dungeon_turn",1)

	-- pc.setf("snow_dungeon","dungeon_turn",0)
	-- ----------------------------------------------------------------
	
	-- --------------------- #Zindana devam et ------------------------
	-- function is_nemere_dungeon_turn_back()
		-- if pc.getf("snow_dungeon","dungeon_turn") == 1 and game.get_event_flag("nemere_dungeon_turn_back") == 0 and game.get_event_flag("dungeon_turn_back") == 0 then
			-- return true
		-- else
			-- return false
		-- end
	-- end
	
	-- when 20395.chat."Nemereye devam et..." with snow_dungeon.is_nemere_dungeon_turn_back() begin
		-- say_npc()
		-- say_reward("Görünüse göre yarim kalan bir macera var")
		-- say_reward("seni içeri tekrar göndermemi ister misin ?")
		-- say("")
		-- local s = select("Beni içeri gönder !","Kapat")
		-- if s == 1 then 
			-- pc.dungeon_turn_back_warp(352)
		-- end
	-- end
	-- ----------------------------------------------------------------
	
    when 20395.chat."Nemere'nin Kulesi (Bireysel)" begin
		addimage(35,35,'nemere.tga')
		if party.is_leader() or party.is_party() then
			say_title(mob_name(20395))
			say("")
			say("")
			say("")
			say("")
			say("")
			say_reward("Bu zindana grup halinde giremezsin")
			say_reward("lütfen önce bulundugun grupdan ayril !")
			return
		end
		-- sys_log(0,"snow_dungeon quest open")
        local set = snow_dungeon.get_settings()
        local need_level = 90
		say_title(mob_name(20395))
		addimage(35,35,'nemere.tga')
		say("")
		say("")
		say("")
		say("")
		say("")
		say("Nemerenin kulesine tek basina giris yapabilirsin")
		say("Biraz tehlikeli bir yolculuk olacak gibi duruyor!")
		say("")
        say_reward("Nemere'nin Gözetleme Kulesi'ne girmek")
		say_reward("istiyor musun?")
        if select("Gir","Girme") == 2 then
            return
        end
		-- ------------ Dungeon Turn Back System ----------------------
		-- pc.remove_turn_back_dungeon()
		-- ------------------------------------------------------------
        if (game.get_event_flag("enable_time_snow") == 1 and pc.getf("snow_dungeon","duration") >= get_global_time()) then
			say_title(mob_name(20395))
			addimage(35,35,'nemere.tga')
			say("")
			say("")
			say("")
			say("")
			say("")
            say("Beta Server; süreyi sýfýrlamak mý istiyorsun?")
            if select("Evet","Hayýr") == 1 then
                pc.setf("snow_dungeon","duration",0)
                return
            end
        end
        if pc.get_level() < need_level then
			say_title(mob_name(20395))
			addimage(35,35,'nemere.tga')
			say("")
			say("")
			say("")
			say("")
			say("")
            say("Kuleye girebilmek için en az "..need_level..".")
            say("seviyede olmalýsýn.")
            return
        end
        if pc.getf("snow_dungeon","duration") >= get_global_time() then
            say_title(mob_name(npc.get_race()))
			addimage(35,35,'nemere.tga')
			say("")
			say("")
			say("")
			say("")
			say("")
            say("Nemere'nin Gözetleme Kulesi'ne yeniden giriþ ")
            say("için bekleme süresi henüz dolmamýþ.")
            say("Kalan süre: "..dungeon_get_duration(pc.getf("snow_dungeon","duration") - get_global_time()).."")
            return
        end
		if pc.count_item(71175) < 1 then
			say_title(mob_name(20395))
			addimage(35,35,'nemere.tga')
			say("")
			say("")
			say("")
			say("")
			say("")
			say("Envanterinde yeterli miktarda yok:")
			say(string.format("%s.", item_name(71175)))
			say("")
			return
		end
		pc.remove_item(71175, 1)
        timer("make_dungeon",2)
    end
    when 20397.chat."Zindani Canlandir !" with d.getf("level") == 0 begin --Eisiger Löwe
		say("Nemere'nin Gözetleme kulesine girdin. Burada")
        say("en fazla bir saat bulunabilirsin. Kuleden")
		say("çýktýðýnda dört saat bekledikten sonra tekrar")
		say("girebilirsin.")
		wait()	
            local set = snow_dungeon.get_settings()
            local loop_durr = set.DurationLoopTimer
            local wait_durations = set.wait_duration
            local t = get_global_time()
            d.setf("LastTimeGetOutPut",0)    
        server_loop_timer("CheckLevelAndCountKillOnMobs",loop_durr,d.get_map_index())
        d.regen_file("data/dungeon/ice_dungeon/zone_1.txt")
        d.setf("Start_Time",t + set.maxlife_time_on_dungeon)
        server_timer("Jp_OutAllFailNotice",(t + set.maxlife_time_on_dungeon) -  t,d.get_map_index())
        --pc.setqf("duration",t + wait_durations)
        d.setqf2("snow_dungeon","duration",t + wait_durations)
        d.setf("level",1)
        d.notice("Bütün düþmanlarý maðlup edin!") 
        npc.purge()
        snow_dungeon._GetLastTime()
        setskin(NOWINDOW)
    end
	
	when 20397.chat."Nemere Taht Salonu" with d.getf("level") == 9 begin --Eisiger Löwe
		local set = snow_dungeon.get_settings()
        local next_jump_time = set.NextJumpTime
        local level = d.getf("level")
        local stone_level_6 = set.MobVnum_level_6
        local stone_level_9 = set.NpcVnum_level_9
        local boss_vnum = set.BossVnum
        local wait_time = set.WaitBeforOutAllOnExit
        local mobvnum = npc.get_race()
	say("Nemere Taht Salonu'na girmek istiyor musun?")
	local s = select("Gir","Belki sonra")
		if s == 2 then
		return
		end
    server_timer("Jp_Boss",next_jump_time,d.get_map_index())
	local v = set.BossVnumGroup
    d.regen_file("data/dungeon/ice_dungeon/zone_boss.txt")
	d.spawn_group(v, 928, 335, 5 ,1, 1)
	npc.purge()
	setskin(NOWINDOW)
	end
	
	
    when make_dungeon.timer begin
        local set = snow_dungeon.get_settings()
        local cord = set.base_cord
        local start_pos = set.Position_level_1
        local t = get_global_time()
		pc.setf("snow_dungeon","dungeon_turn",0)
       -- d.new_jump(set.map_index,(cord[1] + start_pos[1]), (cord[2] + start_pos[2]))
		pc.setf("snow_dungeon","turn_back_time", 0)
		d.new_jump(set.map_index,(cord[1] + start_pos[1]) * 100, (cord[2] + start_pos[2]) * 100)
        snow_dungeon._ClearAlls()
        d.setf("party_leader_pid",pc.get_player_id())
        pc.setf("ice_dungeon_map_index",d.get_map_index())
		
        d.spawn_mob_dir(20397, 173,262,1) --Eisiger Löwe
    end
    function _ClearAlls()
        local d_index = d.get_map_index()
        clear_server_timer("CheckLevelAndCountKillOnMobs",d_index)
        clear_server_timer("Jp_1",d_index)
        clear_server_timer("Jp_2",d_index)
        clear_server_timer("Jp_3",d_index)
        clear_server_timer("Jp_4",d_index)
        clear_server_timer("Jp_5",d_index)
        clear_server_timer("Jp_6",d_index)
        clear_server_timer("Jp_7",d_index)
        clear_server_timer("Jp_8",d_index)
        clear_server_timer("Jp_9",d_index)
        clear_server_timer("Jp_10",d_index)
        clear_server_timer("Jp_OutAll",d_index)
        clear_server_timer("Jp_OutAllFail",d_index)
        clear_server_timer("Jp_OutAllFailNotice",d_index)
        d.setf("level",0)    
        d.setf("party_leader_pid",0)
        d.setf("Start_Time",0)
        d.setf("LastTimeGetOutPut",0)
        d.setf("Stone_5_count",0)
        d.setf("Stone_5_item_fail",0)
        d.clear_regen()
        d.kill_all()
    end
    
    function _CheckMembersLevel()
        local set = snow_dungeon.get_settings()
        local user_fail_level = {}
        local pids = {party.get_member_pids()}
        local need_level = set.need_level_group
        local have_ninja = true
        local have_shaman = true
        if pc.get_level() < need_level then
             say("Seviyen yetersiz.")
        end
        if (is_test_server() and snow_dungeon.is_TestServerMode() == true ) then
            --dchat("TEST modu aktif.")
            return true
        end
        if table.getn(user_fail_level) == 0 then
            return true
        end
    end
    when logout begin
        if snow_dungeon.is_nemeres_dungeon() == true then
			------------ Dungeon Turn Back System ----------------------
			pc.setf("snow_dungeon","dungeon_turn",1)
			-- pc.setf("snow_dungeon","turn_back_time", get_time() + (5 * 60))
			------------------------------------------------------------
        end
    end
	-- ------------ Dungeon Turn Back System ----------------------
    -- when dungeon_turn_back_exit_nemeres.server_timer begin
        -- if d.select(get_server_timer_arg()) then
			-- pc.warp(432000,165000)
		-- end
	-- end
	-- ------------------------------------------------------------

    when login begin
		if pc.get_map_index() == 352 then
			pc.warp(432000,165000)
			return
		end
        if snow_dungeon.is_nemeres_dungeon() == true then
			-- ------------ Dungeon Turn Back System ----------------------
			-- if (get_time() > pc.getf("snow_dungeon","turn_back_time") and get_time() > pc.getf("snow_dungeon","turn_back_time") != 0) then
				-- say_npc()
				-- say("Zindana devam etmek için sana verilen")
				-- say("5 dakika süre doldu!")
				-- say("")
				-- say_reward("Disari gönderiliyorsun !")
				-- server_timer("dungeon_turn_back_exit_nemeres",5 ,d.get_map_index())
				-- return
			-- end
			-- ------------------------------------------------------------

            local set = snow_dungeon.get_settings()
            local backk = set.outside_entry_pos
           -- pc.set_warp_location(backk[1], backk[2],backk[3])
            if pc.get_level() < set.need_level_group then
                chat("Yetersiz seviye!")
                d.exit()
                return
            end
            --chat("Hoþ geldiniz. Burada "..math.ceil((d.getf("Start_Time")  - get_global_time()) /60) .." dakikalýk süreniz var.")
            --notice("Sonraki kata çýkmak için Tüm yaratýklarý öldürün.")
            --snow_dungeon._GetLastTime()
        end
    end
    
    function _GetLastTime()
        local c = get_global_time()
        local t = d.getf("Start_Time") - c
        local o = d.getf("LastTimeGetOutPut")
        --d.notice("CurrentTime: "..c.." Last: "..o.."")
        if c >= o then
            if t >= 60 then
                d.notice("Nemere Gözlemevi'ndeki kalan süre: "..math.ceil(t / 60).." dakika")
				d.notice("Süre bitiminde gözlemevinden dýþarý, giriþe")
				d.notice("ýþýnlanacaksýnýz.")
            else
                d.notice("Nemere Gözlemevi'ndeki kalan süre: "..t.." saniye")
				d.notice("Süre bitiminde gözlemevinden dýþarý, giriþe")
				d.notice("ýþýnlanacaksýnýz.")
            end
            if t >= (60*10) then
                d.setf("LastTimeGetOutPut",c + 60*5)
            elseif t >= (60*5) then
                d.setf("LastTimeGetOutPut",c + 60*2)
            else
                d.setf("LastTimeGetOutPut",c + 10)
            end
            --d.setf("LastTimeGetOutPut",c + 60*number(3,5)) -- Random
        end
    end
    
    when Jp_OutAllFailNotice.server_timer begin
        if d.select(get_server_timer_arg()) then
            d.notice("Zaman doldu.")
            server_timer("Jp_OutAll",1,d.get_map_index())
        end
    end
    
    when CheckLevelAndCountKillOnMobs.server_timer begin
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            local level = d.getf("level")
            local mob_count = d.count_monster()
            local next_jump_time = set.NextJumpTime
            --d.notice("level: "..level.." mob_count: "..mob_count.."")
            if level == 1 and mob_count <= 5 then
                d.notice("Az sonra ikinci kata ýþýnlanacaksýnýz.")
                --d.notice("")
                server_timer("Jp_2",next_jump_time,d.get_map_index())
                d.clear_regen()
                d.kill_all()
                d.set_regen_file("data/dungeon/ice_dungeon/zone_2.txt")
            elseif level == 3 and mob_count <= 5 then
                d.notice("Az sonra dördüncü kata ýþýnlanacaksýnýz.")
                --d.notice("")
                server_timer("Jp_4",next_jump_time,d.get_map_index())
                d.clear_regen()
                d.kill_all()
                d.regen_file("data/dungeon/ice_dungeon/zone_4.txt")
            elseif level == 4 and mob_count <= 5 then
                d.notice("Az sonra beþinci kata ýþýnlanacaksýnýz.")
               -- d.notice("")
                server_timer("Jp_5",next_jump_time,d.get_map_index())
                d.clear_regen()
                d.kill_all()
                d.set_regen_file("data/dungeon/ice_dungeon/zone_5.txt")
            elseif level == 6 and mob_count <= 5 then
                local MobStonelvl6 = set.MobVnum_level_6
                d.notice("Ayaz Metini belirdi!")
                --d.notice("")
                d.clear_regen()
                d.kill_all()
                d.regen_file("data/dungeon/ice_dungeon/zone_6.txt")
                d.spawn_mob(MobStonelvl6,570,649)
            --elseif level == 7 and mob_count <= 5then
            --    d.notice("Az sonra sekizinci kata ýþýnlanacaksýnýz.")
                --d.notice("")
            --    server_timer("Jp_8",next_jump_time,d.get_map_index())
             --   d.clear_regen()
             --   d.kill_all()
             --   d.set_regen_file("data/dungeon/ice_dungeon/zone_8.txt")                
            end
            snow_dungeon._GetLastTime()
        end
    end

    when Jp_2.server_timer begin --Jump Level 2
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            local v = set.key_level_2
            d.setf("level",2)
            local cord = set.base_cord
            local pos = set.Position_level_2
            d.jump_all((cord[1] + pos[1]), (cord[2] + pos[2]))
            d.notice("Bazý canavarlarýn üzerinde buz anahtarý var.")
            d.notice("Buz anahtarlarý ellerinden alýn ve uyan anahtarý bulun! ")
        end
    end
    when 30331.use with snow_dungeon.is_nemeres_dungeon() == true begin --key_level_2
        local set = snow_dungeon.get_settings()
        local next_jump_time = set.NextJumpTime
        local job = pc.get_job()
        local level = d.getf("level")
        if level == 2 then
            if (is_test_server() and snow_dungeon.is_TestServerMode() == true ) then
                --dchat("SET VARIABLE JOB 3")
                job = 3
            end

                if number(1,3) == 1 then --or d.count_monster() < 10 then
                    d.notice("Az sonra üçüncü kata ýþýnlanacaksýnýz.")
                    server_timer("Jp_3",next_jump_time,d.get_map_index())
                    d.clear_regen()
                    d.kill_all()
                    d.regen_file("data/dungeon/ice_dungeon/zone_3.txt")
                    item.remove()
                else
                    d.notice("Doðru anahtarý aramaya devam edin.")
                    item.remove()
                end
        end
    end
    
    when Jp_3.server_timer begin --Jump Level 3
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            d.setf("level",3)
            local cord_MQ = set.base_cord
            local pos = set.Position_level_3
            d.jump_all((cord_MQ[1] + pos[1]), (cord_MQ[2] + pos[2]))
            d.notice("Bütün düþmanlarý maðlup edin!")
        end
    end
    
    when Jp_4.server_timer begin --Jump Level 4
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            d.setf("level",4)
            local cord = set.base_cord
            local pos = set.Position_level_4
            d.jump_all((cord[1] + pos[1]), (cord[2] + pos[2]))
            d.notice("Bütün düþmanlarý maðlup edin! Kutsama size ikinci")
			d.notice("akýnda güç kazandýracak.")
        end
    end
    
    when Jp_5.server_timer begin --Jump Level 5
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            local v = set.NpcVnum_level_5
            d.setf("level",5)
            local cord = set.base_cord
            local pos = set.Position_level_5
            d.jump_all((cord[1] + pos[1]), (cord[2] + pos[2]))
            d.notice("Bir sonraki kata geçmek için Arktos Küpleri'ndeki")
			d.notice("mühürleri sýrasýyla kýrýn.")
			d.notice("Buz canavarlarýný maðlup edin ve ellerinden kuzey")
			d.notice("yýldýzlarýný alýp mühürleri kýrýn!")
            --if set.IsNewga_me_re_vi_sion == true then
            --    d.notice("Die Schlüssel lassen die Monster fallen...")
           -- else
            --    d.notice("Die Schlüssel landen automatisch ins Inventar...")
           -- end
            snow_dungeon._Create_Stones_level5()
        end
    end
    
    function _DropKeyOnLevel5()
        if number(1,1000) >= 900 then --Abbruch der weiteren funtion -> Damit es spannend bleibt bei den würfeln... :P
            return
        end
        local set = snow_dungeon.get_settings()
        local count = set.StoneCount_level_5
        local vnum = set.Key_on_level_5
        local get_random_unique = d.get_unique_vid("stone5_"..number(1,count).."")
        local fail_count = d.getf("Stone_5_item_fail") +1
        local search = d.getf("Stone_5_count")
        local search2 = d.get_unique_vid("stone5_"..search.."")
        if search2 != get_random_unique then
            d.setf("Stone_5_item_fail",fail_count)
        end
        --
       -- if set.IsNewga_me_re_vi_sion == true then
            --New function
            game.drop_item_and_select(vnum,1)
       -- else
            --pc.give_item2_select(vnum,1)
       -- end
        --dchat("Creaty Key fail_count: "..fail_count.."")
        if (is_test_server() and snow_dungeon.is_TestServerMode() == true ) then --Wenn TestServer Modus
            item.set_socket(1,search2)
            d.setf("Stone_5_item_fail",0)
        elseif fail_count > 6 then --Wenn mehr wie x mal nicht richtig erstellt wird, dann kommt der richtige Key
            item.set_socket(1,search2)
            d.setf("Stone_5_item_fail",0)
        else    
            item.set_socket(1,get_random_unique)
        end
    end
    
    function _Create_Stones_level5()
        local set = snow_dungeon.get_settings()
        local stone_level5_count_MQ = set.StoneCount_level_5
        local c = set.StoneCords_level_5 --{ {432,508},{437,493},{448,492},{448,476},{467,475},{466,464}}
        local v = set.NpcVnum_level_5
        for i = 1, stone_level5_count_MQ, 1 do
            local n = number(1,table.getn(c))
            --d.spawn_mob(20398, c[n][1], c[n][2])
            d.set_unique("stone5_"..i.."", d.spawn_mob(v, c[n][1], c[n][2]))
            --d.set_unique("stone5_"..i.."", mob.spawn(20398, c[n][1], c[n][2],1,1,1)) --GEHT
            --d.set_unique("stone5_"..i.."", d.spawn_mob_ac_dir(20398, c[n][1], c[n][2],i)) --GEHT
            table.remove(c,n)
            if i == 1 then
                d.setf("Stone_5_count",1)
                d.setf("Stone_5_item_fail",0)
            end
        end
    end
    
    when 20398.take begin --NpcVnum_level_5
        if snow_dungeon.is_nemeres_dungeon() == true then
            local set = snow_dungeon.get_settings()
            local stone_level5_count = set.StoneCount_level_5
            local next_jump_time = set.NextJumpTime
            local vnum = item.get_vnum()
            local key_level_5 = set.Key_on_level_5 --30332
            local unique_item = item.get_socket(1)
            --dchat("unique_item: "..unique_item.."")
            if (is_test_server() and snow_dungeon.is_TestServerMode() == true ) then
                d.notice("TESTSERVERMODE, Biraz sonra ýþýnlanacaksýnýz.")
                npc.purge()
                item.remove()
                server_timer("Jp_6",next_jump_time,d.get_map_index())
                d.clear_regen()
                d.kill_all()
                d.regen_file("data/dungeon/ice_dungeon/zone_6.txt")
                return
            end
            if vnum == key_level_5 then
                local npc_vid = npc.get_vid()
                local search = d.getf("Stone_5_count")
                local search2 = d.get_unique_vid("stone5_"..search.."")
                --dchat("npc_vid: "..npc_vid.." search:"..search.." search2:"..search2.." ")
                if npc_vid == search2 then
                    if unique_item == npc_vid then
                        local new = search +1 
                        if new > stone_level5_count then
                            d.notice("Az sonra altýncý kata ýþýnlanacaksýnýz.")
                            npc.purge()
                            item.remove()
                            server_timer("Jp_6",next_jump_time,d.get_map_index())
                            d.clear_regen()
                            d.kill_all()
                            d.regen_file("data/dungeon/ice_dungeon/zone_6.txt")
                        else
							if new == 2 then
                            say("Baþardýnýz! Þimdi ikinci mührü açýn.")
							elseif new == 3 then
							say("Baþardýnýz! Þimdi üçüncü mührü açýn.")
							elseif new == 4 then
							say("Baþardýnýz! Þimdi dördüncü mührü açýn.")
							elseif new == 5 then
							say("Baþardýnýz! Þimdi beþinci mührü açýn.")
							elseif new == 6 then
							say("Baþardýnýz! Þimdi altýncý mührü açýn.")
							end
						npc.purge()
                        item.remove()
                        end
                        d.setf("Stone_5_count",new)
                    else
                        say("Sýralama doðru! Fakat anahtar yanlýþ. Baþka")
						say("bir anahtarý deneyin.")
                        item.remove()
                    end
                else
                    say("Sýralamanýn doðru olmasýna dikkat edin! Baþka bir")
					say("küpü deneyin.")
					--say("zaa")
					item.remove()
                end
            end
        end    --if is dungeon end
    end    --when end
    
    when Jp_6.server_timer begin --Jump Level 6
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            local v = set.MobVnum_level_6
            d.setf("level",6)
            local cord = set.base_cord
            local pos = set.Position_level_6
            d.jump_all((cord[1] + pos[1]), (cord[2] + pos[2]))
            
            d.notice("Bütün buz canavarlarýný maðlup edin ve ardýndan")
			d.notice("Ayaz Metini'ni yok edin.")
         ---   d.notice("Nur Schamanen können ihm Schaden hinzufügen.")
            --d.spawn_mob(v,570,649)
            
        end
    end
    
    when Jp_7.server_timer begin --Jump Level 7
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            local v = set.MobVnum_level_7
            d.setf("level",7)
            local cord = set.base_cord
            local pos = set.Position_level_7
            d.jump_all((cord[1] + pos[1]), (cord[2] + pos[2]))
            d.notice("Ayazýn ve buzun efendisi Szel'i bulun ve yok edin.")
			d.notice("Bunun üstesinden ancak ejderhanýn desteðiyle")
			d.notice("gelinebilir.")
        end
    end
    
    when Jp_8.server_timer begin --Jump Level 8
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            local v = set.Key_on_level_8
            d.setf("level",8)
            local cord = set.base_cord
            local pos = set.Position_level_8
            d.jump_all((cord[1] + pos[1]), (cord[2] + pos[2]))
            d.notice("Bütün buz canavarlarýný maðlup edin ve buzçiçeði anahtarýný bulun. ")
        end
    end
    
    when 30333.use with snow_dungeon.is_nemeres_dungeon() == true begin --Key_on_level_8
        local set = snow_dungeon.get_settings()
        local next_jump_time = set.NextJumpTime
        local level = d.getf("level")
        local job = pc.get_job()
        if level == 8 then
            if (is_test_server() and snow_dungeon.is_TestServerMode() == true ) then
                --dchat("SET VARIABLE JOB 3")
                job = 3
            end

                if number(1,3) == 1 then
                    d.notice("Az sonra dokuzunca kata ýþýnlanacaksýnýz.")
                    server_timer("Jp_9",next_jump_time,d.get_map_index())
                    d.clear_regen()
                    d.kill_all()
                    d.regen_file("data/dungeon/ice_dungeon/zone_9.txt")
                    item.remove()
                else
                    say("Yanlýþ anahtardý. Doðru anahtarý bulun!")
                    item.remove()
                end
        end
    end
    
    when Jp_9.server_timer begin --Jump Level 9
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            local v = set.NpcVnum_level_9
            d.setf("level",9)
            local cord = set.base_cord
            local pos = set.Position_level_9
            d.jump_all((cord[1] + pos[1]), (cord[2] + pos[2]))
            d.notice("Kuzeyli Ejderha Sütunu'nu yok edin!")
            d.spawn_mob(v,849,655)
        end
    end
    
    when Jp_Boss.server_timer begin --Jump Level BossZone
        if d.select(get_server_timer_arg()) then
            local set = snow_dungeon.get_settings()
            local v = set.BossVnum
            d.setf("level",10)
            local cord = set.base_cord
            local pos = set.Position_level_10
            d.jump_all((cord[1] + pos[1]), (cord[2] + pos[2]))
            d.notice("Ayazýn ve buzun kralý Nemere'yi maðlup edin.")
        end
    end
    
    when kill begin
        if snow_dungeon.is_nemeres_dungeon() == true then
            local set = snow_dungeon.get_settings()
            local next_jump_time = set.NextJumpTime
            local level = d.getf("level")
            local stone_level_6 = set.MobVnum_level_6
			local stone_level_7 = set.MobVnum_level_7
            local stone_level_9 = set.NpcVnum_level_9
            local boss_vnum = set.BossVnum
            local wait_time = set.WaitBeforOutAllOnExit
            local mobvnum = npc.get_race()
            --dchat("nemeres_wait_dungeon MobVnum = "..mobvnum.."")
            if level == 2 and mobvnum >= 6101 and mobvnum <= 6108 and 1 == number(1,50)  then
                local v = set.key_level_2
                game.drop_item(v,1)
            elseif level == 5 and mobvnum >= 6101 and mobvnum <= 6108 and 1 == number(1,20) then
                snow_dungeon._DropKeyOnLevel5()
            elseif level == 6 and mobvnum == stone_level_6 then
                d.notice("Az sonra yedinci kata ýþýnlanacaksýnýz.")
                --d.notice("")
                server_timer("Jp_7",next_jump_time,d.get_map_index())
                d.clear_regen()
                d.kill_all()
                d.regen_file("data/dungeon/ice_dungeon/zone_7.txt") --Szel
                local v_MQv = set.MobVnum_level_7
                local c = set.SzelCords_level_7
                local n = number(1,table.getn(c))
                d.spawn_mob(v_MQv, c[n][1], c[n][2])
			elseif level == 7 and mobvnum == stone_level_7 then
				d.notice("Az sonra sekizinci kata ýþýnlanacaksýnýz.")
                --d.notice("")
                server_timer("Jp_8",next_jump_time,d.get_map_index())
                d.clear_regen()
                d.kill_all()
                d.set_regen_file("data/dungeon/ice_dungeon/zone_8.txt")
            elseif level == 8 and mobvnum >= 6101 and mobvnum <= 6108 and 1 == number(1,50) then
                local v = set.Key_on_level_8
                game.drop_item(v,1)    
            elseif level == 9 and mobvnum == stone_level_9 then				
                --d.notice("Sonraki seviyeye geçiyorsunuz ve "..mob_name(stone_level_9).." maðlup edildi.")
                --server_timer("Jp_Boss",next_jump_time,d.get_map_index())
                d.clear_regen()
                d.kill_all()
				d.spawn_mob_dir(20397, 849,670,1)
                --d.regen_file("data/dungeon/ice_dungeon/zone_boss.txt")
                --local v = set.BossVnumGroup
                --d.spawn_group(v, 928, 335, 5 ,1, 1) --    (Input from SRC: argument: vnum,x,y,radius,aggressive,count)
            elseif level == 10 and mobvnum == boss_vnum then
				game.drop_item_with_ownership("50034",1)
                d.clear_regen()
                d.kill_all()
                d.notice("Nemere maðlup edildi!")
				d.notice("Bir dakika içerisinde Nemere Gözlemevi'nden dýþarý ")
				d.notice("ýþýnlanacaksýnýz.")
				notice_all("Nemere "..pc.get_name().." tarafýndan yok edildi.")
                server_timer("Jp_OutAll",wait_time,d.get_map_index())
            end
        end
    end
    when Jp_OutAll.server_timer begin --Jump Out Bye bye ;D
        if d.select(get_server_timer_arg()) then
            snow_dungeon._ExitAll()
        end
    end
    function _ExitAll()
        d.exit_all()
    end
end
end  