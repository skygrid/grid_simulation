import numpy as np

inp_file, inp_size, loc1, loc2, loc3, loc4, stor1, stor2, stor3, stor4 = np.loadtxt('input.csv', skiprows=1, dtype=np.str, delimiter=',', usecols=(4,5,7,8,9,10,11,12,13,14), unpack=True, ndmin=2)

FILE_NAMES = ["CERN", "CNAF", "GRIDKA", "IN2P3", "PIC", "RAL", "RRCKI", "SARA"]

for file_name in FILE_NAMES:
    f_name_online = "Platform/Content/" + file_name + "_content_online.txt"
    f_name_nearline = "Platform/Content/" + file_name + "_content_nearline.txt"
    f1 = open(f_name_online, "w")
    f2 = open(f_name_nearline, "w")
    for i in range(len(inp_file)):
        if file_name == loc1[i]:
            if stor1[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            else:
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
        if file_name == loc2[i]:
            if stor2[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            else:
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
        if file_name == loc3[i]:
            if stor3[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            else:
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
        if file_name == loc4[i]:
            if stor4[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            else:
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
    f1.close()
    f2.close()
