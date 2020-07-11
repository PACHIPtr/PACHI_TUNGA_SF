#! /usr/local/bin/python2.7

"""PreQC Details
1) In:
	define __smth__ "a b c d"
 But not in:
	define __smth__ "a\32b\32c\32d"
 The space is not valid and only '"a' will be "used" to replace (bug!)
2) define will replace everything containing that 'token'
 except for:
 a) compound words:
	schu__smth__re
 b) or inside "" strings: (es.)
	say(" __smth__ ")
 d) or with nothing before: (nothing or seps)
		__smth__
 d) or comment:
	-- __smth__
 e) but not for: (bug!)
	--[[
	__smth__]]
3) The define 'token' should stay before any 'quest'
define __smth__ something

Revised by martysama0134 (base version by YMIR)
"""

import os
import shutil
import time

LPATH_DFT="./quest_list"
EPATH_DFT="./pre_qc"
ACOPY_DFT=False
VERBO_DFT=False
BACKP_DFT=False

#if py>=2.4 use @BenchMe instead of BenchMe(fnc, (args), {args)
def BenchMe(fnc, *args, **kw):
	print "--- BenchMe calling for %s with args %s, %s Begin ---" % (fnc.func_name, args, kw)
	t_start = time.time()
	rtn = fnc(*args, **kw)
	print time.time()-t_start
	print "--- BenchMe for %s End ---"%fnc.func_name
	return rtn
#(NoBenchMe,BenchMe)[verbo] just for curiosity
def NoBenchMe(fnc, *args, **kw):
	return fnc(*args, **kw)

class PreQC(object):
	def __init__(self, lpath=LPATH_DFT, epath=EPATH_DFT, verbo=VERBO_DFT, backp=BACKP_DFT, acopy=ACOPY_DFT):
		self.lpath = lpath
		self.epath = epath
		self.verbo = verbo
		self.backp = backp
		self.acopy = acopy
		self.pBM = (NoBenchMe,BenchMe)[verbo]
		if not os.path.exists(self.epath):
			os.makedirs(self.epath)
		else:
			#security
			if (not self.epath[0]=="/") or (self.epath.find("..")==-1) or (self.epath=="."):
				shutil.rmtree(self.epath)
				os.makedirs(self.epath)
		self.lista = []
		self.pBM(self.LoadList)	#self.LoadList()

	def LoadList(self):
		m_list = open(self.lpath, "r")
		for fname in m_list.read().split("\n"): #instead of readlines() to easily clean \r\n
			fname = fname.rstrip()
			if not fname or fname[0]=="#":
				continue
			self.lista.append(fname)
		if self.verbo:
			print "--- PreQC.LoadList Begin ---"
			for ffname in self.lista: print repr(ffname)
			print "--- PreQC.LoadList End ---"
		m_list.close()

	def Generate(self):
		for fname in self.lista:
			self.pBM(run, *(fname, self.epath, self.acopy))

	def Compile(self):
		if os.path.exists("object"):
			if self.backp:
				os.rename("object", "object__%s"%time.strftime("%Y%m%d-%H%M%S", time.localtime()))
			shutil.rmtree("./object")
		#os.system("rm -rf object")
		for fname in self.lista:
			if not fname:
				continue
			#no popen for convenience
			if "/" in fname:
				#fix bug if .quest paths contain /
				#you should not call two quests with the same name anyway
				fname=fname.split("/")[-1]
			if os.system("./qc %s/%s"%(self.epath, fname)):
				print "Error occured on quest %s/%s"%(self.epath, fname)
				os._exit(0)
		os.system("chmod -R ug+rwx object")

def split_by_quat(buf):
	p = False
	l = list(buf)
	l.reverse()
	s = ""
	res = []
	while l:
		c = l.pop()
		if c == '"':
			if p == True:
				s += c
				res += [s]
				s = ""
			else:
				if len(s) != 0:
					res += [s]
				s = '"'
			p = not p
		elif c == "\\" and l[0] == '"':
			s += c
			s += l.pop()
		else:
			s += c

	if len(s) != 0:
		res += [s]
	return res

def AddSepMiddleOfElement(l, sep):
	l.reverse()
	new_list = [l.pop()]
	while l:
		new_list.append(sep)
		new_list.append(l.pop())
	return new_list

def my_split_with_seps(s, seps):
	res = [s]
	for sep in seps:
		new_res = []
		for r in res:
			sp = r.split(sep)
			sp = AddSepMiddleOfElement(sp, sep)
			new_res += sp
		res = new_res
	new_res = []
	for r in res:
		if r != "":
			new_res.append(r)
	return new_res

def my_split(s, seps):
	res = [s]
	for sep in seps:
		new_res = []
		for r in res:
			sp = r.split(sep)
			new_res += sp
		res = new_res
	new_res = []
	for r in res:
		if r != "":
			new_res.append(r)
	return new_res
def MultiIndex(list, key):
	l = []
	i = 0
	for s in list:
		if s == key:
			l.append(i)
		i = i + 1
	return l

def Replace(lines, parameter_table, keys):
	r = []
	for string in lines:
		l = split_by_quat(string)
		for s in l:
			if s[0] == '"':
				r += [s]
			else:
				tokens = my_split_with_seps(s, ("\t", ",", " ", "=", "[", "]","-","<",">","~","!",".","(",")","+","*","/"))
				for key in keys:
					try:
						indices = MultiIndex(tokens, key)
						for i in indices:
							tokens[i] = parameter_table[key][0]
					except:
						pass
				r += tokens
	return r

def MakeParameterTable(lines, parameter_table, keys):
	names = []
	values = []
	group_names = []
	group_values = []
	idx = 0
	for line in lines:
		idx += 1
		line = line.strip("\n")
		if(-1 != line.find("--")):
			line = line[0:line.find("--")]

		tokens = my_split(line, ["\t", ",", " ", "=", "[", "]", "\r", "\n"])
		if len(tokens) == 0:
			continue
		if cmp(tokens[0], "quest") == 0:
			start = idx
			break
		if cmp(tokens[0], "define") == 0:
			if cmp(tokens[1], "group") == 0:
				group_value = []
				for value in tokens[3:]:
					if parameter_table.get(value, 0) != 0:
						value = parameter_table[value]
					group_value.append(value)
				parameter_table[tokens[2]] = group_value
				keys.append(tokens[2])
			elif len(tokens) > 5:
				print "%d Invalid syntax" % idx
				print "define [name] = [value]"
				print "define group [name] = \"[\"[v0],[v1], ... \"]\""
			else:
				value = tokens[2]
				if parameter_table.get(value, 0) != 0:
					value = parameter_table[value]
				parameter_table[tokens[1]] = [value]
				keys.append(tokens[1])
	parameter_table = dict(zip(group_names, group_values))
	return start

def run(filename=LPATH_DFT, w2dump=EPATH_DFT, cp4a=ACOPY_DFT):
	parameter_table = {}
	keys = []

	#filename = filename.strip("\n")
	if not filename:
		return False
	fname = open(filename)
	lines = fname.readlines()
	start = MakeParameterTable(lines, parameter_table, keys)
	#if not cp4a:
	#	if len(keys) == 0:
	#		return False
	#print filename,keys
	#print filename, len(keys), cp4a
	oname = "%s/%s"%(w2dump, filename.split("/")[-1])
	if not len(keys) == 0:
		lines = lines [start-1:]
		r = Replace(lines, parameter_table, keys)

		# dump
		f = open(oname, "w")
		for s in r:
			 f.write(s)
		f.close()
	elif cp4a:
		shutil.copyfile(filename, oname)
		#print filename, oname
	fname.close()
	return True

if __name__ == "__main__":
	def Usage():
		print """Usage:
	--help or -h to show this message
	--lpath	or -l to select the quest_list file path (./quest_list by default)
	--epath or -e to select the folder path to dump everything (./pre_qc by default)
	--all or -a to copy to pre_qc folder all the quests even they have no define tokens
	--compile or -c to automatically run ./qc for each quest (False by default)
	--nopre or -n to just read the list without start processing (False by default)
	--verbose or -v to enable verbosity (False by default)
	--backup or -b to perform a backup of object (False by default)
	# ./pre_qc.py
	# ./pre_qc.py -l mylist -e myout
	# ./pre_qc.py -l ./muhsecret/quest_list -e ./muhdump/pre_qc
	# ./pre_qc.py -ac
	# ./pre_qc.py -bv

Revised by martysama0134 (base version by YMIR)
"""
	import getopt
	import os
	import sys
	try:
		# grab args
		optlist, args = getopt.getopt(sys.argv[1:], "hl:e:acnvb", ("help","lpath","epath","all","compile","nopre","verbose","backup"))
		# config
		v_bcomp	= False
		v_bnpre	= True
		v_cpall	= ACOPY_DFT
		v_lpath = LPATH_DFT
		v_epath = EPATH_DFT
		v_verbo	= VERBO_DFT
		v_backp = BACKP_DFT
		# analyze args
		for o, a in optlist:
			if o in ("-h", "--help"):
				sys.exit(Usage())
			if o in ("-l", "--lpath"):
				v_lpath = a
			elif o in ("-e", "--epath"):
				v_epath = a
			elif o in ("-a", "--all"):
				v_cpall = True
			elif o in ("-c", "--compile"):
				v_bcomp = True
			elif o in ("-n", "--nopre"):
				v_bnpre = False
			elif o in ("-v", "--verbose"):
				v_verbo = True
			elif o in ("-b", "--backup"):
				v_backp = True
			else:
				sys.exit(Usage())
		# check
		if (not v_lpath) or (not v_epath):
			sys.exit(Usage())
		# start
		pQC = PreQC(lpath=v_lpath, epath=v_epath, verbo=v_verbo, backp=v_backp, acopy=v_cpall)
		if v_bnpre:
			pQC.pBM(pQC.Generate)
		if v_bcomp:
			pQC.pBM(pQC.Compile)
	except getopt.GetoptError, err:
		sys.exit(err)
















