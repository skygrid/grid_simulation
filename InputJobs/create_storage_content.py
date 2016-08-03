import numpy as np

inp_file, inp_size, storage_type, loc1, loc2, loc3 = np.loadtxt('../input.csv', skiprows=1, dtype=np.str, delimiter=',', usecols=(3,4,6,7,8,9), unpack=True)

FILE_NAMES = ["CERN", "CNAF", "GRIDKA", "IN2P3", "PIC", "RAL", "RRCKI", "SARA"]

for file_name in FILE_NAMES:
    f_name_online = "../Platform/Content/" + file_name + "_content_online.txt"
    f_name_nearline = "../Platform/Content/" + file_name + "_content_nearline.txt"
    f1 = open(f_name_online, "w")
    f2 = open(f_name_nearline, "w")
    for i in range(len(inp_file)):
        if file_name in [loc1[i], loc2[i], loc3[i]]:
            if storage_type[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            else:
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
    f1.close()
    f2.close()
