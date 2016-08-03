import random
number_of_tiers = 7
number_of_links = 138

for i in range(0, number_of_tiers):
	filename = "Failure/h_" + str(i) 
	f = open(filename, "w")
	f.write("PERIODICITY 10.0\n")
	f.write("0.0 1\n")
	f.write(str(random.randint(600, 1000)) + " 0");
	f.close()

for i in range(0, number_of_links):
	filename = "Failure/l_" + str(i)
	f = open(filename, "w")
	f.write("PERIODICITY 10.0\n")
	f.write("0.0 1\n")
	f.write(str(random.randint(20, 50)) + " 0");
	f.close()


	
