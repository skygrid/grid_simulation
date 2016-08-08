import random
number_of_tiers = 7
number_of_links = 138

LINK_NAMES = []
LINK_NAMES.extend(["CERN-CNAF-LHCOPN-001", "CERN-GRIDKA-LHCOPN-001",  "CERN-IN2P3-LHCOPN-001", "CERN-PIC-LHCOPN-001", "CERN-RAL-LHCOPN-001", "CERN-SARA-LHCOPN-001", "CERN-RRCK1-LHCOPN-001"])
LINK_NAMES.extend(["CNAF-GRIDKA-LHCOPN-001", "GRIDKA-IN2P3-LHCOPN-001", "GRIDKA-SARA-LHCOPN-001"])

"""for i in range(0, number_of_tiers):
	filename = "Failure/h_" + str(i) 
	f = open(filename, "w")
	f.write("PERIODICITY 10.0\n")
	f.write("0.0 1\n")
	f.write(str(random.randint(600, 1000)) + " 0");
	f.close()"""

for i, link in enumerate(LINK_NAMES):

	if link == "CERN-RAL-LHCOPN-001":
		f = open(filename, "w")
		filename = "Failure/" + link + ".state"
		f.write("PERIODICITY 1000000.0\n")
		f.write("0.0 1\n")
		f.write(str(moment_break) + " 1\n")
		f.write(str(moment_break + break_period) + " 1\n")
		f.close()
		continue

	if link == "CERN-PIC-LHCOPN-001":
		f = open(filename, "w")
		filename = "Failure/" + link + ".state"
		f.write("PERIODICITY 1000000.0\n")
		f.write("0.0 1\n")
		f.write(str(moment_break) + " 0.1\n")
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


	
