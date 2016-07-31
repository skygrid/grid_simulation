#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import numpy as np
f = open("tier2.csv", "w")
f.write("Name, CPU, Storage\n")

nameU, Pledgetype, LHCb = np.loadtxt("pledges.csv", delimiter=',', usecols=(1, 2, 9), dtype=np.str, skiprows=1, unpack=True)

indexes = np.where((LHCb != "") & (LHCb != "0") & (Pledgetype == "CPU (HEP-SPEC06)"))
array = LHCb[indexes]

for i in range(len(indexes[0])):
    f.write(str(nameU[indexes][i]) + ",")
    f.write(str(float(LHCb[indexes][i])/100) + ",")
    f.write(str(LHCb[indexes[0]+1][i]) + "TB" + "\n")
f.close()
