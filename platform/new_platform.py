#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import numpy as np
import sys

quo, q = "\"", "\""

#NAMES OF LINKS
LINK_NAMES10 = []
LINK_NAMES11 = []
LINK_NAMES10.extend(["CERN-CNAF-LHCOPN-001", "CERN-GRIDKA-LHCOPN-001",  "CERN-IN2P3-LHCOPN-001", "CERN-PIC-LHCOPN-001", "CERN-RAL-LHCOPN-001", "CERN-SARA-LHCOPN-001", "CERN-RRCK1-LHCOPN-001"])
LINK_NAMES11.extend(["CNAF-GRIDKA-LHCOPN-001", "GRIDKA-IN2P3-LHCOPN-001", "GRIDKA-SARA-LHCOPN-001"])

# BANDWIDTH OF LINKS
LATENCY = 0
bandwidth = 10.
LINK_NAMES10_BW10 = []
LINK_NAMES10_BW11 = []
LINK_NAMES10_BW10.extend([str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G"])
LINK_NAMES10_BW11.extend([str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G"])

rtype = np.dtype([('t', np.str, 15), ('hepspec', np.float32), ('disk', np.str, 20), ('tape', np.str, 20), ('cpu', np.float32), ('names', np.str, 35)])


tier_name, HEPSPEC, DISK_ALL, TAPE_ALL, CPU_ALL, TIER_ALL = np.loadtxt("../InputData/capacities_lhcb.csv", skiprows=1, dtype=(rtype), usecols=(0, 3, 4, 5, 6, 7), delimiter=";", unpack=True)
print len(tier_name)
tier2_count = np.where(tier_name == "Tier 2")[0].size

f = open("platform.xml", "w")
f.write("<?xml version='1.0'?>\n")
f.write("<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n")
f.write("<platform version=\"4\">\n")
f.write("\n")
f.write("\t<AS id=\"AS_BIG\" routing=\"Full\">\n\n")

# DISKS TIER0, TIER1, TIER2
f.write("\t\t<!--Storage types of Tier0, Tier1s, Tier2s-->\n")
for i in range(len(DISK_ALL)):
    f.write("\t\t<storage_type id=" + quo + "HDD" + str(i) + quo + " size=" + quo + str(int(float((DISK_ALL[i])))) + "GB" + quo + " model=" + quo + "linear_no_lat" + quo + "\n")
    f.write("\t\t\t\tcontent=" + quo + "InputData/SEs/" + str(TIER_ALL[i]) + "-DISK.txt" + quo + " content_type=" + quo + "txt_unix" + quo + ">\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bwrite" + quo + " value=" + quo + "500MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bread" + quo + " value=" + quo + "118MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bconnection" + quo + " value=" + quo + "10000GBps" + quo + "/>\n")
    f.write("\t\t</storage_type>\n\n")

# TAPES TIER0, TIER1, TIER2
f.write("\n\n")

for i in range(len(TAPE_ALL)):
    f.write("\t\t<storage_type id=" + quo + "TAPE" + str(i) + quo + " size=" + quo + str(int(float(TAPE_ALL[i]))) + "GB" + quo + " model=" + quo + "linear_no_lat" + quo + "\n")
    f.write("\t\t\t\tcontent=" + quo + "InputData/SEs/" + str(TIER_ALL[i]) + "-TAPE.txt" + quo + " content_type=" + quo + "txt_unix" + quo + ">\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bwrite" + quo + " value=" + quo + "500MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bread" + quo + " value=" + quo + "118MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bconnection" + quo + " value=" + quo + "10000GBps" + quo + "/>\n")
    f.write("\t\t</storage_type>\n\n")


f.write("\n\n")

f.write("\t\t<!--Storages of Tier0, Tier1s, Tier2s-->\n")
# DEFINING STORAGE OF TIER0, TIER1 AND TIER2
for i in range(len(CPU_ALL)):
    f.write("\t\t<storage id=" + quo + TIER_ALL[i] + "-DISK" + quo + " typeId=" + quo + "HDD" + str(i) + quo + " attach=" + quo + str(TIER_ALL[i]) + quo + "/>\n")
for i in range(len(CPU_ALL)):
    f.write("\t\t<storage id=" + quo + TIER_ALL[i] + "-TAPE" + quo + " typeId=" + quo + "TAPE" + str(i) + quo + " attach=" + quo + str(TIER_ALL[i]) + quo + "/>\n")

f.write("\n\n")


# TIER0, TIER1, TIER2
f.write("\t\t<!--HOSTS Tier0, Tier1s, Tier2s-->\n")

for i in range(len(TIER_ALL)):
    f.write("\t\t<host id=\"" + TIER_ALL[i] + "\" speed=\"1Gf\" core=\"" + str((int(CPU_ALL[i]))) + "\">\n")

    for j in range(len(TIER_ALL)):
        f.write("\t\t\t<mount storageId=" + q + TIER_ALL[j] + "-DISK" + q + " name=" + q+ "/" + TIER_ALL[j] + "-DISK" + q + "/>\n")
        f.write("\t\t\t<mount storageId=" + q + TIER_ALL[j] + "-TAPE" + q + " name=" + q+ "/" + TIER_ALL[j] + "-TAPE" + q + "/>\n")

    f.write("\t\t</host>\n\n")
f.write("\n\n")


# TIER0-TIER1 LINKS

f.write("\t\t<!--LINKS of Tier0, Tier1s, Tier2s-->\n")
f.write("\t\t<link id=\"loopback\" bandwidth=\"498MBps\" latency=\"0us\" sharing_policy=\"FATPIPE\"/>\n")

for i in range(len(LINK_NAMES10)):
    f.write("\t\t<link id=" + quo + str(LINK_NAMES10[i]) + quo + " bandwidth=\"" + str(LINK_NAMES10_BW10[i]) + "Bps\"" + " latency=\"" + str(LATENCY) + "ms\"/>\n")
f.writelines("\n")
for i in range(len(LINK_NAMES11)):
    f.write("\t\t<link id=" + quo + str(LINK_NAMES11[i]) + quo + " bandwidth=\"" + str(LINK_NAMES10_BW11[i]) + "Bps\"" + " latency=\"" + str(LATENCY) + "ms\"/>\n")
f.write("\n")

#TIER0-TIER2 LINKS

f.write("\t\t<!--TIER0-TIER2 LINKS-->\n")
for i in range(1, tier2_count+1):
    f.write("\t\t<link id=" + quo + "0-Tier2_" + str(i) + quo + " bandwidth=\"1G" + "Bps\"" + " latency=\"" + str(LATENCY) + "ms\"/>\n")
f.write("\n")



#TIER1-TIER2 LINKS

f.write("\t\t<!--TIER1-TIER2 LINKS-->\n")
for i in range(1, 8):
    for j in range(1, tier2_count+1):
        f.write("\t\t<link id=" + quo + str(i) + "-Tier2_" + str(j) + quo + " bandwidth=\"1G" + "Bps\"" + " latency=\"" + str(LATENCY) + "ms\"/>\n")
    f.write("\n")
f.write("\n")


#######################################################################################################################################
n = 0

f.write("\t\t<!--routes between TIER1-TIER1-->\n")
with open("links_delete.xml", "r") as mylinks:
    data = mylinks.read()
f.write(data.replace("\n", "\n\t\t") + "\n\n")


# CERN-CERN ROUTE
f.write("\t\t<route src=\"CERN-PROD\" dst=\"CERN-PROD" + "\">")
f.write("<link_ctn id=\"loopback" + "\"/>")
f.write("</route>\n\n")

# routs between AS and TIER1
f.write("\t\t<!--routes between CERN and TIER1; CERN and TIER2-->\n")

for i in range(1, len(CPU_ALL)):
    # routes between CERN and TIER1S
    f.write("\t\t<route src=\"CERN-PROD\" dst=\"" + TIER_ALL[i] + "\">")
    if tier_name[i] == "Tier 2":
        f.write("<link_ctn id=\"" + "0-Tier2_" + str(i-7) + "\"/>")
    else:
        f.write("<link_ctn id=\"" + LINK_NAMES10[i-1] + "\"/>")
    f.write("</route>\n")
f.write("\n\n")

f.write("\t\t<!--routes between TIER1s and TIER2s -->\n")

for i in range(1, 8):
    for j in range(8, len(CPU_ALL)):
        f.write("\t\t<route src=\"" + TIER_ALL[i] + "\"" + " dst=\"" + TIER_ALL[j] + "\">")
        f.write("<link_ctn id=\"" + str(i) + "-Tier2_" + str(j-7) + "\"/>")
        f.write("</route>\n")
    f.write("\n")

f.write("\t</AS>\n")
f.write("</platform>\n")
f.close()

#################################################################################
#                             DEPLOYMENT FILE                                   #
#                             DEPLOYMENT FILE                                   #
#################################################################################
f = open("deployment.xml", "w")
f.write("<?xml version='1.0'?>\n")
f.write("<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n")
f.write("<platform version=\"4\">\n")
f.write("\n")
f.write("\t<process host=\"CERN-PROD\" function=\"scheduler\">\n")
f.write("\t\t<argument value=\"scheduler\"/>  <!-- Input mailbox -->\n")
f.write("\t</process>\n")
f.write("\n")

for i in range(0, len(TIER_ALL)):
    f.write("\t<process host=\"" + TIER_ALL[i] + "\" function=\"tier1\">\n")
    f.write("\t\t<argument value=\"" + TIER_ALL[i] + "\"/>\n")
    f.write("\t\t<argument value=\"" + str(int(CPU_ALL[i] )) + "\"/>\n")
    f.write("\t</process>\n")
    f.write("\n")
f.write("\n")

"""for i in range(0, len(LIST_OF_TIERS)):
    f.write("\t<process host=\"" + LIST_OF_TIERS[i] + "\" function=\"evil\">\n")
    f.write("\t\t<argument value=\"" + str(randint(0, 10000)) + "\"/>\n")
    f.write("\t\t<argument value=\"" + str(randint(30, 75)) + "\"/>\n")
    f.write("\t\t<argument value=\"" + str(randint(100, 200)) + "\"/>\n")
    f.write("\t</process>\n")
    f.write("\n")vim
f.write("\n\n")"""

# Additional processes for killing process, data popularity and tracing
f.write("\t<process host=\"CERN-PROD\" function=\"killer\">\n")
f.write("\t</process>\n\n")

f.write("\t<process host=\"CERN-PROD\" function=\"initialize\">\n")
f.write("\t\t<argument value=\"518400\"/>\n")
f.write("\t</process>\n\n")

f.write("\t<process host=\"CERN-PROD\" function=\"delete_unpop_file\">\n")
f.write("\t\t<argument value=\"518400\"/>\n")
f.write("\t</process>\n\n")

f.write("\t<process host=\"CERN-PROD\" function=\"tracer\">\n")
f.write("\t\t<argument value=\"518400\"/>\n")
f.write("\t</process>\n\n\n")
####################################################################################


#####################################################################################
f.write("</platform>")
f.close()