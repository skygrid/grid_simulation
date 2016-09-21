import numpy as np

type, inp_file, inp_size, loc1, loc2, loc3, loc4, loc5, loc6, loc7, loc8, loc9, loc10, stor1, stor2, stor3, stor4, stor5, stor6, stor7, stor8, stor9, stor10 = np.loadtxt('input.csv', skiprows=1, dtype=np.str, delimiter=',', usecols=(1,4,5,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26), unpack=True, ndmin=2)

FILE_NAMES = ["CERN", "CNAF", "GRIDKA", "IN2P3", "PIC", "RAL", "RRCKI", "SARA"]
true_indexes = np.sort(np.unique(inp_file, return_index=True)[1])

for file_name in FILE_NAMES:
    f_name_online = "Platform/Content/" + file_name + "_content_online.txt"
    f_name_nearline = "Platform/Content/" + file_name + "_content_nearline.txt"
    f1 = open(f_name_online, "w")
    f2 = open(f_name_nearline, "w")

    for i in true_indexes:
    #for i in range(len(inp_file)):
        if inp_file[i] == "none":
            continue
        if file_name == loc1[i]:
            if stor1[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor1[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc2[i]:
            if stor2[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor2[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc3[i]:
            if stor3[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor3[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc4[i]:
            if stor4[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor4[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc5[i]:
            if stor5[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor5[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc6[i]:
            if stor6[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor6[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc7[i]:
            if stor7[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor7[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc8[i]:
            if stor8[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor8[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc9[i]:
            if stor9[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor9[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")

        if file_name == loc10[i]:
            if stor10[i] == "1":
                f1.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
            if stor10[i] == "0":
                f2.write("/" + inp_file[i] + " " + inp_size[i] + "\n")
    f1.close()
    f2.close()
