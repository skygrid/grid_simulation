import random
number_of_tiers = 7
number_of_links = 138

LINK_NAMES = []
LINK_NAMES.extend(["CERN-CNAF-LHCOPN-001", "CERN-GRIDKA-LHCOPN-001",  "CERN-IN2P3-LHCOPN-001", "CERN-PIC-LHCOPN-001", "CERN-RAL-LHCOPN-001", "CERN-SARA-LHCOPN-001", "CERN-RRCK1-LHCOPN-001"])
LINK_NAMES.extend(["CNAF-GRIDKA-LHCOPN-001", "GRIDKA-IN2P3-LHCOPN-001", "GRIDKA-SARA-LHCOPN-001"])



for i, link in enumerate(LINK_NAMES):

	if link in ["CERN-RAL-LHCOPN-001", "CERN-PIC-LHCOPN-001"]:
		filename = "Failure/" + link + ".bw"
		f = open(filename, "w")
		moment_break = random.randint(2000, 10000)
		break_period = random.randint(40, 80)
		f.write("PERIODICITY 1000000.0\n")
		f.write("0.0 1\n")
		if link == "CERN-PIC-LHCOPN-001":
			x = 0.1
		else:x = 1
		f.write(str(moment_break) + " " + str(x) + "\n")
		f.write(str(moment_break + break_period) + " 1\n")
		f.close()
		continue


	filename = "Failure/" + link + ".fail"
	f = open(filename, "w")
	moment_break = random.randint(2000, 10000)
	break_period = random.randint(40, 80)
	f.write("PERIODICITY 1000000.0\n")
	f.write("0.0 1\n")
	f.write(str(moment_break) + " 0\n")
	f.write(str(moment_break + break_period) + " 1\n")
	f.close()


	
