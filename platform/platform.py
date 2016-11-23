#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import numpy as np
import sys
from random import randint

ratio = float(sys.argv[1])

quo, q = "\"", "\""

NUMBER_OF_TIERS = 7
LATENCY = 0.0


#  CPU OF TIERS BENCHMARK 
CERN_CPU = 5100
CNAF_CPU = 2808
GRIDKA_CPU = 2440
IN2P3_CPU = 2300
PIC_CPU = 949
RAL_CPU = 4680
SARA_CPU = 1708
RRCKI_CPU = 1640

CPU = map(int, np.array([5100, 2808, 2440, 2300, 949, 4680, 1708, 1640])  / ratio)

# ONLINE STORAGE OF TIER1 TheraByte
STORAGE_CERN = 7600  # GB
STORAGE_CNAF = 3150  # GB
STORAGE_GRIDKA = 2500  # GB
STORAGE_IN2P3 = 2230  # lapp-se01.in2p3.fr
STORAGE_PIC = 969  # ? SINGLE VALUE OF
STORAGE_RAL = 4050  # GB
STORAGE_SARA = 1743  # SINGLE VALUE OF
STORAGE_RRCKI = 1260

# NEARLINE STORAGES
NEARLINE_CERN = 20600
NEARLINE_CNAF = 7578
NEARLINE_GRIDKA = 4300
NEARLINE_IN2P3 = 4360  # ??
NEARLINE_PIC = 1677
NEARLINE_RAL = 12630
NEARLINE_SARA = 3019
NEARLINE_RRCKI = 1677

# OFFLINE STORAGE ???

# NAMES OF CONTENT FILES
CERN_CONTENT = "CERN_content"
CNAF_CONTENT = "CNAF_content"
GRIDKA_CONTENT = "GRIDKA_content"
IN2P3_CONTENT = "IN2P3_content"
PIC_CONTENT = "PIC_content"
RAL_CONTENT = "RAL_content"
SARA_CONTENT = "SARA_content"
RRCKI_CONTENT = "RRCKI_content"

# LIST OF ONLINE STORAGE
LIST_ONLINE_STORAGE = []
LIST_NEARLINE_STORAGE = []
LIST_CONTENT = []
LIST_CONTENT_TIER2 = []


LIST_ONLINE_STORAGE.extend([STORAGE_CERN, STORAGE_CNAF, STORAGE_GRIDKA, STORAGE_IN2P3, STORAGE_PIC, STORAGE_RAL, STORAGE_SARA, STORAGE_RRCKI])
LIST_NEARLINE_STORAGE.extend([NEARLINE_CERN, NEARLINE_CNAF, NEARLINE_GRIDKA, NEARLINE_IN2P3, NEARLINE_PIC, NEARLINE_RAL, NEARLINE_SARA, NEARLINE_RRCKI])
LIST_CONTENT.extend([CERN_CONTENT, CNAF_CONTENT, GRIDKA_CONTENT, IN2P3_CONTENT, PIC_CONTENT, RAL_CONTENT, SARA_CONTENT, RRCKI_CONTENT])

LIST_STORAGE_STRING = ["CERN1", "CNAF1", "GRIDKA1", "IN2P31", "PIC1", "RAL1", "SARA1", "RRCKI1"]
LIST_NEARLINE_STRING = ["CERN0", "CNAF0", "GRIDKA0", "IN2P30", "PIC0", "RAL0", "SARA0", "RRCKI0"]
LIST_OF_TIERS = ["CERN", "CNAF", "GRIDKA", "IN2P3", "PIC", "RAL", "SARA", "RRCKI"]
LIST_OF_TIER1S = ["CNAF", "GRIDKA", "IN2P3", "PIC", "RAL", "SARA", "RRCKI"]


# NAMES OF LINKS CONVENTION (T0-T1 AND T1-T1)
LINK_NAMES10 = []
LINK_NAMES11 = []
LINK_NAMES10.extend(["CERN-CNAF-LHCOPN-001", "CERN-GRIDKA-LHCOPN-001",  "CERN-IN2P3-LHCOPN-001", "CERN-PIC-LHCOPN-001", "CERN-RAL-LHCOPN-001", "CERN-SARA-LHCOPN-001", "CERN-RRCK1-LHCOPN-001"])
LINK_NAMES11.extend(["CNAF-GRIDKA-LHCOPN-001", "GRIDKA-IN2P3-LHCOPN-001", "GRIDKA-SARA-LHCOPN-001"])

# BANDWIDTH OF LINKS
bandwidth = 10.  / ratio
LINK_NAMES10_BW10 = []
LINK_NAMES10_BW11 = []
LINK_NAMES10_BW10.extend([str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G"])
LINK_NAMES10_BW11.extend([str(bandwidth)+"G", str(bandwidth)+"G", str(bandwidth)+"G"])

LINK_INFO10 = dict(zip(LINK_NAMES10, LINK_NAMES10_BW10))
LINK_INFO11 = dict(zip(LINK_NAMES11, LINK_NAMES10_BW11))

NAMES_TIER2, CPU_TIER2, STORAGE_TIER2 = np.loadtxt("tier2/tier2.csv", skiprows=1, dtype=(np.str), usecols=(0, 1, 2), delimiter=",", unpack=True)

for i in range(1, len(NAMES_TIER2)+1):
	LIST_CONTENT_TIER2.append("Tier2_" + str(i) + ".txt")

f = open("platform.xml", "w")
f.write("<?xml version='1.0'?>\n")
f.write("<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n")
f.write("<platform version=\"4\">\n")
f.write("\n")
f.write("\t<AS id=\"AS_BIG\" routing=\"Full\">\n")

for i in range(len(LIST_ONLINE_STORAGE)):
    f.write("\t\t<storage_type id=" + quo + "single_HDD" + str(i) + quo + " size=" + quo + str(LIST_ONLINE_STORAGE[i]) + "TB" + quo + " model=" + quo + "linear_no_lat" + quo + "\n")
    f.write("\t\t\t\tcontent=" + quo + "Content/Tier1/" + str(LIST_CONTENT[i]) + "_online.txt" + quo + " content_type=" + quo + "txt_unix" + quo + ">\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bwrite" + quo + " value=" + quo + "500MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bread" + quo + " value=" + quo + "118MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bconnection" + quo + " value=" + quo + "1000000000.2GBps" + quo + "/>\n")
    f.write("\t\t</storage_type>\n\n")

for i in range(len(LIST_ONLINE_STORAGE)):
    f.write("\t\t<storage_type id=" + quo + "NEARLINE_HDD" + str(i) + quo + " size=" + quo + str(LIST_NEARLINE_STORAGE[i]) + "TB" + quo + " model=" + quo + "linear_no_lat" + quo + "\n")
    f.write("\t\t\t\tcontent=" + quo + "Content/Tier1/" + str(LIST_CONTENT[i]) + "_nearline.txt" + quo + " content_type=" + quo + "txt_unix" + quo + ">\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bwrite" + quo + " value=" + quo + "500MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bread" + quo + " value=" + quo + "118MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bconnection" + quo + " value=" + quo + "100000000000.2GBps" + quo + "/>\n")
    f.write("\t\t</storage_type>\n\n")

f.write("\n")
f.write("\t\t<!--Storages types of tier2 storages-->\n")
for i in range(1, len(NAMES_TIER2)+1):
    f.write("\t\t<storage_type id=" + quo + "TIER2_HDD" + str(i) + quo + " size=" + quo + str(STORAGE_TIER2[i-1]) + quo + " model=" + quo + "linear_no_lat" + quo + "\n")
    f.write("\t\t\t\tcontent=" + quo + "Content/Tier2/" + str(LIST_CONTENT_TIER2[i-1]) + quo + " content_type=" + quo + "txt_unix" + quo + ">\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bwrite" + quo + " value=" + quo + "500MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bread" + quo + " value=" + quo + "118MBps" + quo + "/>\n")
    f.write("\t\t\t<model_prop id=" + quo + "Bconnection" + quo + " value=" + quo + "100000000000.2GBps" + quo + "/>\n")
    f.write("\t\t</storage_type>\n\n")



# DEFINING STORAGE OF TIER0, TIER1 AND TIER2
for i in range(len(LIST_ONLINE_STORAGE)):
    f.write("\t\t<storage id=" + quo + LIST_STORAGE_STRING[i] + quo + " typeId=" + quo + "single_HDD" + str(i) + quo + " attach=" + quo + str(LIST_OF_TIERS[i]) + quo + "/>\n")

f.write("\n")
for i in range(len(LIST_ONLINE_STORAGE)):
    f.write("\t\t<storage id=" + quo + LIST_NEARLINE_STRING[i] + quo + " typeId=" + quo + "NEARLINE_HDD"+ str(i) + quo + " attach=" + quo + str(LIST_OF_TIERS[i]) + quo + "/>\n")

f.write("\n")

for i in range(1, len(NAMES_TIER2)+1):
    f.write("\t\t<storage id=" + quo + "Tier2_" + str(i) + quo + " typeId=" + quo + "TIER2_HDD"+ str(i) + quo + " attach=" + quo + "Tier2_" + str(i) + quo + "/>\n")

f.write("\n")


# DEFINING TIER0
f.write("\n")
f.write("\t\t<host id=\"CERN\" speed=\"1Gf\" core=\"" + str(CPU[0])  + "\">\n")
for j in range(len(LIST_STORAGE_STRING)):
    f.write("\t\t\t<mount storageId=" + q + LIST_STORAGE_STRING[j] + q + " name=" + q+ "/" + LIST_OF_TIERS[j] + "1" + q + "/>\n")
    f.write("\t\t\t<mount storageId=" + q + LIST_NEARLINE_STRING[j] + q + " name=" + q+ "/" + LIST_OF_TIERS[j] +"0" + q + "/>\n")
f.write("\t\t</host>\n")
# DEFINING TIER1s

for i in range(NUMBER_OF_TIERS):
    f.write("\t\t<host id=\"" + LIST_OF_TIER1S[i] + "\" speed=\"1Gf\" core=\"" + str(CPU[i+1]) + "\" >\n")
    # MOUNT TIERS STORAGE
    f.write("\n")
    for j in range(len(LIST_STORAGE_STRING)):
        f.write("\t\t\t<mount storageId=" + q + LIST_STORAGE_STRING[j] + q + " name=" + q+ "/" + LIST_OF_TIERS[j] + "1" + q + "/>\n")
        f.write("\t\t\t<mount storageId=" + q + LIST_NEARLINE_STRING[j] + q + " name=" + q+ "/" + LIST_OF_TIERS[j] + "0" + q + "/>\n")
    f.write("\t\t</host>\n\n")
f.write("\n\n")

###################################################################
#                  DEFINING THE LAYER                             #
#                     OF TIER2                                    #
###################################################################
f.write("<!--DEFINING THE LAYER OF TIER2 -->\n")

for i in range(1, len(NAMES_TIER2)+1):
    f.write("\t\t<host id=\"" + "Tier2_" + str(i)  + "\" speed=\"1Gf\" core=\"" + str((CPU_TIER2[i-1])) + "\">\n")
    # MOUNT TIERS STORAGE
    # f.write("\n")
    f.write("\t\t\t<mount storageId=" + q + "Tier2_" + str(i) + q + " name=" + q+ "/" + "Tier2_" + str(i) + q + "/>\n")
    f.write("\t\t</host>\n\n")
f.write("\n")


#####################################################################################################################################

f.write("\n")

f.write("\t\t<link id=\"loopback\" bandwidth=\"498MBps\" latency=\"0us\" sharing_policy=\"FATPIPE\"/>\n")
for i in range(NUMBER_OF_TIERS):
    f.write("\t\t<link id=" + quo + str(LINK_NAMES10[i]) + quo + " bandwidth=\"" + str(LINK_NAMES10_BW10[i]) + "Bps\"" + " latency=\"" + str(LATENCY) + "ms\"/>\n")
f.writelines("\n")
for i in range(len(LINK_NAMES11)):
    f.write("\t\t<link id=" + quo + str(LINK_NAMES11[i]) + quo + " bandwidth=\"" + str(LINK_NAMES10_BW11[i]) + "Bps\"" + " latency=\"" + str(LATENCY) + "ms\"/>\n")
f.write("\n")

#TIER0-TIER2 LINKS

f.write("\t\t<!--TIER0-TIER2 LINKS-->\n")
for i in range(1, len(NAMES_TIER2)+1):
	f.write("\t\t<link id=" + quo + "0-Tier2_" + str(i) + quo + " bandwidth=\"10G" + "Bps\"" + " latency=\"" + str(LATENCY) + "ms\"/>\n")
f.write("")
	
#######################################################################################################################################
n = 0

f.write("\t\t<!--routes between TIER1-TIER1-->\n")
with open("links_delete.xml", "r") as mylinks:
    data = mylinks.read()
f.write(data.replace("\n", "\n\t\t") + "\n\n")


# CERN-CERN ROUTE
f.write("\t\t<route src=\"CERN\" dst=\"CERN" + "\">")
f.write("<link_ctn id=\"loopback" + "\"/>")
f.write("</route>\n\n")

# routs between AS and TIER1
f.write("\t\t<!--routes between CERN and TIER1-->\n")

for x in range(NUMBER_OF_TIERS):

    # routes between CERN and TIER1S
    f.write("\t\t<route src=\"CERN\" dst=\"" + LIST_OF_TIER1S[x] + "\">")
    f.write("<link_ctn id=\"" + LINK_NAMES10[x] + "\"/>")
    f.write("</route>\n")
f.write("\n")

# TIER0-TIER2 ROUTES
f.write("\t\t<!--CERN-TIER2 ROUTES-->\n")
for i in range(1, len(NAMES_TIER2)+1):
	f.write("\t\t<route src=\"CERN\" dst=\"" + "Tier2_" + str(i) + "\">")
	f.write("<link_ctn id=\"" + "0-Tier2_" + str(i) + "\"/>")
	f.write("</route>\n")
f.write("\n")
##################################################################################

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
f.write("\t<process host=\"CERN\" function=\"scheduler\">\n")
f.write("\t\t<argument value=\"scheduler\"/>  <!-- Input mailbox -->\n")
f.write("\t</process>\n")
f.write("\n")

for i in range(0, len(LIST_OF_TIERS)):
    f.write("\t<process host=\"" + LIST_OF_TIERS[i] + "\" function=\"tier1\">\n")
    f.write("\t\t<argument value=\"" + LIST_OF_TIERS[i] + "\"/>\n")
    f.write("\t\t<argument value=\"" + str(int(CPU[i] )) + "\"/>\n")
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
f.write("\t<process host=\"CERN\" function=\"killer\">\n")
f.write("\t</process>\n\n")

f.write("\t<process host=\"CERN\" function=\"initialize\">\n")
f.write("\t\t<argument value=\"518400\"/>\n")
f.write("\t</process>\n\n")

f.write("\t<process host=\"CERN\" function=\"delete_unpop_file\">\n")
f.write("\t\t<argument value=\"518400\"/>\n")
f.write("\t</process>\n\n")

f.write("\t<process host=\"CERN\" function=\"tracer\">\n")
f.write("\t\t<argument value=\"518400\"/>\n")
f.write("\t</process>\n\n\n")
####################################################################################


# TIER2 processes 
for i in range(1, len(NAMES_TIER2)+1):
    f.write("\t<process host=\"Tier2_" + str(i) + "\" function=\"tier1\">\n")
    f.write("\t\t<argument value=\"Tier2_" + str(i) + "\"/>\n")
    f.write("\t\t<argument value=\"" + str(int(float(CPU_TIER2[i-1])) / 1) + "\"/>\n")
    f.write("\t</process>\n")
    f.write("\n")
f.write("</platform>")
#####################################################################################
f.close()
