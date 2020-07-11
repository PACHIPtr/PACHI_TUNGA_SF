import app
import sys
import os
import time
import thread
import dbg
import ui
import uiCommon
from os.path import basename

check_folder=' '.join(os.listdir(os.getcwd()))

def syslog(s):
	file = open('HackShield.Config.Error.txt', 'a')
	file.write('n' + s + 'n')
	file.close()
	
def FileCheck():
	banned_ext=[".mix", ".m3d", ".flt", ".asi", ".bat"]
	for i in banned_ext:
		if check_folder.find(i) != -1:
			syslog("<HShield> mix file detected.")
			app.Exit()
			
FileCheck()