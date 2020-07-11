import os,sys,shutil
mypath = os.path.dirname(os.path.realpath(__file__))
startm2=0
fu_stop=0
debug=0
while 1:
	print "commands start/stop"
	for packName in str(raw_input("Name of archive: ").lower()).split(" "):
		if packName == "":			pass
		elif packName == "start":	startm2=1
		elif packName == "stop":	startm2=0
		elif packName == "debug":	debug=1
		else:
			pathList = {
				"effect":"d:/ymir work/effect",
				"environment":"d:/ymir work/environment",
				"guild":"d:/ymir work/guild",
				"icon":"icon",
				"item":"d:/ymir work/item",
				"locale":"locale",
				"main":"d:/ymir work",
				"monster":"d:/ymir work/monster",
				"monster2":"d:/ymir work/monster2",
				"npc":"d:/ymir work/npc",
				"npc2":"d:/ymir work/npc2",
				"npc_mount":"d:/ymir work/npc_mount",
				"npc_pet":"d:/ymir work/npc_pet",
				"pc":"d:/ymir work/pc",
				"pc2":"d:/ymir work/pc2",
				"pc3":"d:/ymir work/pc3",
				"property":"property",
				"sound":"sound",
				"special":"d:/ymir work/special",
				"terrain":"d:/ymir work/terrainmaps",
				"textureset":"textureset",
				"tree":"d:/ymir work/tree",
				"ui":"d:/ymir work/ui",
				"zone":"d:/ymir work/zone",
				"update_model2":"d:/ymir work",
				"update_model":"d:/ymir work",
				"update1":"d:/ymir work",
				"update2":"d:/ymir work",
			}
			# if packName == "locale_client":
				# fu_stop = 1
				# os.system("cd locale_es & python write2File.pyexec")
			if fu_stop == 1:	fu_stop = 0
			else:
				f = open("bin/make_xml.xml", "w")
				f.write("<ScriptFile>\n")
				f.write("\t<CreateEterPackXml Input=\"" + packName + ":")
				if packName in pathList:
					f.write(pathList[packName] + "/\" ")
				else:
					f.write("\" ")
				f.write("ArchivePath=\"../bin-directory/pack/" + packName + "\" ")
				f.write("XmlPath=\"bin/_create.xml\">\n")				
				for ignore_ext in ('db','png','psd','pyexec','rar','zip',):
					f.write("\t\t<Ignore Pattern=\"[a-zA-z0-9]+.%s\" />\n" % ignore_ext)					
				for ignore_single_file in ('item_names.txt','mob_names.txt',):
					f.write("\t\t<Ignore Pattern=\"%s\" />\n" % ignore_single_file)					
				f.write("\t</CreateEterPackXml>\n")
				f.write("</ScriptFile>\n")
				f.close()
				os.system("\"" + mypath + "\\bin\\FileArchiver.exe\" bin/make_xml.xml")
				print("XML File created.")

				packFile = []
				f = open("bin/_create.xml", "r")
				for line in f:
					if line.find("/" + packName + "/") != -1:
						searchStr = "/" + packName + "/"
						line = line[:line.find(searchStr)] + line[line.find(searchStr) + len(searchStr):]
					if not (packName in pathList):
						searchStr = "ArchivedPath=\"/"
						if line.find(searchStr) != -1:
							line = line[:line.find(searchStr) + len(searchStr) - 1] + line[line.find(searchStr) + len(searchStr):]
					packFile.append(line)
				f.close()
				os.system("\"" + mypath + "\\bin\\FileArchiver.exe\" " + "bin/_create.xml")
				if debug == 0:
					os.remove("bin/make_xml.xml")
					os.remove("bin/_create.xml")
					
				print("Packing completed.")
	if startm2==1:
		os.system("cd \"" + mypath + "\\..\\bin-directory\\\" & Myte2Client.exe")