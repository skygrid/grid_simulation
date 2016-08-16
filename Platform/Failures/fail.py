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
		moment_break = random.randint(0, 50 * 86400)
		break_period = random.uniform(0, 10 * 86400)
		f.write("PERIODICITY 10000000.0\n")
		f.write("0.0 100000000\n")
		if link == "CERN-PIC-LHCOPN-001":
			x = 10000000
		else:x = 100000000
		f.write(str(moment_break) + " " + str(x) + "\n")
		f.write(str(moment_break + break_period) + " 100000000\n")
		f.close()
		continue


	filename = "Failure/" + link + ".fail"
	f = open(filename, "w")
	moment_break = random.randint(0, 50 * 86400)
	break_period = random.uniform(0, 15 * 86400)
	f.write("PERIODICITY 10000000.0\n")
	f.write("0.0 1\n")
	f.write(str(moment_break) + " 0\n")
	f.write(str(moment_break + break_period) + " 1\n")
	f.close()


	
