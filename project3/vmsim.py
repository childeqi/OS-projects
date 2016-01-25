################################################################
################################################################
################        	vmsim         		################
################            mew115              ################
################################################################
################################################################

from array import *
import sys, getopt
import random

filename = ""
algor = ""
num_frame = 0
refresh = 0
tau = 0
memory_accesses = 0
page_faults = 0
disk_write = 0
all_frame_dict = {}
frames_dict = {}
pointer = -1
reference_dict = {}
frames_list = []
nru_dict = {}
time_dict = {}

def results():
	print(algor.title())
	print("Number of frames: " + str(num_frame))
	print("Total memory accesses: " + str(memory_accesses))
	print("Total page faults: " + str(page_faults))
	print("Total writes to disk: " + str(disk_write))

def predict():
	global all_frame_dict
	mem_accesses = 0
	fp = open (filename, 'r')
	for line in fp:
		mem_accesses = mem_accesses + 1
		sss = line.split(" ")
		lll = list(sss[0].strip())
		address = ''.join(lll[0:5])
		mode = sss[1].strip()
		history = list()			
		if ((len(all_frame_dict) > 0 ) & (address in all_frame_dict)):
			history = all_frame_dict[address]
			history.append(mem_accesses)
			all_frame_dict[address] = history
		else:
			history.append(mem_accesses)
			all_frame_dict[address] = history
	fp.close()

def nru_refresh():
	global reference_dict
	for key in reference_dict:
		reference_dict[key] = 0

def work_refresh(mem_accesses):
	global reference_dict
	global time_dict
	for key in reference_dict:
		reference_dict[key] = 0
	for key in time_dict:
		time_dict[key] = mem_accesses

def opt_algor(mem_accesses, addr, dirt):
	global all_frame_dict
	global frames_dict
	global disk_write
	kick = ""
	next = 0

	for key in frames_dict:
		next_t = -1
		lll = len(all_frame_dict[key])			
		for i in all_frame_dict[key]:
			if (i > mem_accesses):
				next_t = i
				index = all_frame_dict[key].index(i)
				all_frame_dict[key] = all_frame_dict[key][index:]
				break
		
		if (next_t == -1):
			kick = key
			all_frame_dict[key] = []
			break
		else:
			if(next_t > next):
				next = next_t
				kick = key
	if (frames_dict[kick] == 1):
		disk_write = disk_write + 1
	del frames_dict[kick]
	frames_dict[addr] = dirt

def clock_algor(mem_accesses, addr, dirt):
	global frames_dict
	global disk_write
	global reference_dict
	global frames_list
	global pointer

	kick = ""
	key = frames_list[pointer]

	for i in range(0, num_frame + 1):
		if (reference_dict[key] == 1):
			reference_dict[key] = 0
			pointer = (pointer + 1) % num_frame
			key = frames_list[pointer]
		elif (reference_dict[key] == 0):
			kick = key
			break
	if (frames_dict[kick] == 1):
		disk_write = disk_write + 1
	del frames_dict[kick]
	del reference_dict[kick]
	frames_list.remove(kick)
	frames_list.insert(pointer, addr)
	frames_dict[addr] = dirt
	pointer = (pointer + 1) % num_frame

def nru_algor(mem_accesses, addr, dirt):
	global frames_dict
	global disk_write
	global reference_dict
	global refresh
	global nru_dict

	kick = ""

	for i in range(0, 4):
		nru_dict[i] = list()
		for key in frames_dict:
			p = (reference_dict[key] << 1) + frames_dict[key]
			if (p == i):
				nru_dict[i].append(key)
	for i in range(0, 4):
		l = len(nru_dict[i])
		if (l == 1):
			kick = nru_dict[i][0]
			break
		elif (l > 1):
			rand = random.randint(0, (l-1))
			kick = nru_dict[i][rand]
			break
	if (frames_dict[kick] == 1):
		disk_write = disk_write + 1
	del frames_dict[kick]
	del reference_dict[kick]
	frames_dict[addr] = dirt

def w_s_algor(mem_accesses, addr, dirt):
	global frames_dict
	global disk_write
	global reference_dict
	global refresh
	global time_dict
	global tau
	global frames_list
	global pointer

	kick = ""

	for i in range(pointer, num_frame):
		key = frames_list[i]
		if (reference_dict[key] == 1):
			pass
		elif ((reference_dict[key] == 0) & (frames_dict[key] == 0)):
			kick = key
			pointer = i
			break
		elif((reference_dict[key] == 0) & (frames_dict[key] == 1)):
			age = mem_accesses - time_dict[key]
			if (age > tau):
				frames_dict[key] = 0
				disk_write = disk_write + 1
				pass

	if (kick == ""):
		old = sorted(time_dict.values())
		for key in frames_dict:
			if(old[0] == time_dict[key]):
				kick = key
				break
		pointer = frames_list.index(kick)

	if (frames_dict[kick] == 1):
		disk_write = disk_write + 1

	del frames_dict[kick]
	del reference_dict[kick]
	del time_dict[kick]
	frames_list.remove(kick)
	frames_list.insert(pointer, addr)
	frames_dict[addr] = dirt
	time_dict[addr] = mem_accesses

def main():
	global filename
	global algor
	global num_frame
	global memory_accesses
	global page_faults
	global disk_write
	global all_frame_dict
	global refresh
	global tau
	global pointer

	conter_r = 0
						#read from arguments
	l = len(sys.argv)
	if ((l == 6)| (l == 8) | (l == 10)):
		pass
	else:
		print("Wrong arguments!!")
		exit()
	if (sys.argv[1] == "-n"):
		pass
	else:
		print("Wrong arguments!!")
		exit()
	if (sys.argv[2].isdigit() == True):
		num_frame = int(sys.argv[2])
	else:
		print("Wrong arguments!!")
		exit()
	if (sys.argv[3] == "-a"):
		pass
	else:
		print("Wrong arguments!!")
		exit()
	if ((sys.argv[4] == "opt") | (sys.argv[4] == "clock") | (sys.argv[4] == "nru") | (sys.argv[4] == "work")):
		algor = sys.argv[4]
	else:
		print("Wrong arguments!!")
		exit()
	if (l == 6):
		filename = sys.argv[5]
	if (l == 8):
		if((sys.argv[5] == "-r") & (sys.argv[6].isdigit() == True)):
			refresh = int(sys.argv[6])
			filename = sys.argv[7]
		else:
			print("Wrong arguments!!")

	if (l == 10):
		if((sys.argv[5] == "-r") & (sys.argv[6].isdigit() == True)):
			refresh = int(sys.argv[6])
			if((sys.argv[7] == "-t") & (sys.argv[8].isdigit() == True)):
				tau = int(sys.argv[8])
				filename = sys.argv[9]
			else:
				print("Wrong arguments!!")

	if (algor == "opt"):
		predict()

	fp = open (filename, 'r')
	for line in fp:		
		memory_accesses = memory_accesses + 1
		if (algor == "nru"):
			if (conter_r == refresh):
				nru_refresh()
				conter_r = 0
			conter_r = conter_r + 1
		if (algor == "work"):
			if (conter_r == refresh):
				work_refresh(memory_accesses)
				conter_r = 0
			conter_r = conter_r + 1
		sss = line.split(" ")
		lll = list(sss[0].strip())
		address = ''.join(lll[0:5])
		mode = sss[1].strip()
		if(mode == "W"):
			d = 1
		elif(mode == "R"):
			d = 0
		else:
			print("Error!!")
			exit()
		if (len(frames_dict) == 0):			# Empty! Page fault happen!!
			page_faults = page_faults + 1
			frames_dict[address] = d
			if ((algor == "clock") | (algor == "work")):
				reference_dict[address] = 1
				pointer = 0
				frames_list.append(address)
			if (algor == "nru"):
				reference_dict[address] = 1
			if (algor == "work"):
				reference_dict[address] = 1
				time_dict[address] = memory_accesses			
		else:
			if (address in frames_dict):	# no page fault
				d = (d | frames_dict[address])
				frames_dict[address] = d 	# update
				if ((algor == "clock") | (algor == "nru") | (algor == "work")):
					reference_dict[address] = 1
				if (algor == "work"):
					time_dict[address] = memory_accesses
			else:							# page fault!
				page_faults = page_faults + 1
				if ((algor == "clock") | (algor == "nru") | (algor == "work")):
					reference_dict[address] = 1
				if (len(frames_dict) == num_frame):	# some frame need to be picked out
					if (algor == "opt"):
						opt_algor(memory_accesses, address, d)
					if (algor == "clock"):
						clock_algor(memory_accesses, address, d)
					if (algor == "nru"):
						nru_algor(memory_accesses, address, d)
					if (algor == "work"):
						w_s_algor(memory_accesses, address, d)
				else:						# just add a frame
					frames_dict[address] = d
					if (algor == "clock"):		
						frames_list.append(address)						
					if (algor == "work"):
						frames_list.append(address)	
						time_dict[address] = memory_accesses
	fp.close()

main()
results()