#!/usr/bin/env python
# -*- coding: UTF-8 -*-

NUMBER_OF_TIERS = 7
NODE_OF_NODE_TIER1_1 = 3
NODE_OF_NODE_TIER1_2 = 5
NODE_OF_NODE_TIER1_3 = 5
NODE_OF_NODE_TIER1_4 = 4
NODE_OF_NODE_TIER1_5 = 5
NODE_OF_NODE_TIER1_6 = 3
NODE_OF_NODE_TIER1_7 = 4
list_of_nodes = [3, 5, 5, 4, 5, 3, 4]
SPEED = 133.3
LATENCY = 0.0
BANDWIDTH = 3.432

#STORAGE OF TIER1
STORAGE_CERN = 3,680,824 #GB
STORAGE_CNAF = 5,566,296 #GB
STORAGE_GRIDKA = 2,463,730 #GB
STORAGE_IN2P3 = 1,550,202 # lapp-se01.in2p3.fr
STORAGE_PIC = 6,957,442	# ? SINGLE VALUE OF
STORAGE_RAL = 3,940,035 # GB
STORAGE_SARA = 8,224,639 #	SINGLE VALUE OF	


f = open("platform.xml", "w")
f.write("<?xml version='1.0'?>\n")
f.write("<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n")
f.write("<platform version=\"4\">\n")
f.write("\n\n")
f.write("\t<AS id=\"AS_BIG\" routing=\"Full\">\n")

#DEFINING STORAGES





# DEFINING TIER0
f.write("\t\t<AS id=\"Tier0X\" routing=\"Full\">\n")
f.write("\t\t\t<host id=\"Tier0\" speed=\"10Mf\"/>\n")
f.write("\t\t\t<router id=\"Router0\"/>\n")
f.write("\t\t\t<link id=\"Tier0_Router0\" bandwidth=\"" + str(BANDWIDTH) + "Bps\" latency=\"" + str(LATENCY) + "ms\"/>\n")
f.write("\t\t\t<route src=\"Tier0\" dst=\"Router0\"> <link_ctn id=\"Tier0_Router0\"/></route>\n")

f.write("\t\t</AS>\n")


for i in range(NUMBER_OF_TIERS):
    f.write("\t\t<AS id=\"Tier1" + str(i+1) + "\" routing=\"Full\">\n\n")
    f.write("\t\t\t<!-- DEFINING ALL HOST OF TIER-->\n")
    for n in range(list_of_nodes[i]):
        f.write("\t\t\t<host id=\"Tier1_" + str(i+1) + "_" + str(n+1) + "\" speed=\"" + str(SPEED) + "Mf\"/>\n")
    f.write("\t\t\t<host id=\"Tier1_" + str(i+1) + "\" speed=\"10Mf\"/>\n")
    f.write("\t\t\t<router id=\"Router1_" + str(i + 1) + "\"/>\n")
    f.write("\n")

    # links
    f.write("\t\t\t<!-- DEFINING ALL LINKS BETWEEN HOSTS-->\n")
    for a in range(list_of_nodes[i]):
        for b in range(a+1, list_of_nodes[i]):
            f.write("\t\t\t<link id=\"1_" + str(i+1) + "_" + str(a+1) + "__" + "1_" + str(i+1) + "_" + str(b+1) + "\" bandwidth=\"" + str(BANDWIDTH) + "Bps\" latency=\"" + str(LATENCY) + "ms\"/>\n")
        f.write("\n")

    # links between dispatcher and hosts
    f.write("\t\t\t<!-- DEFINING ALL LINKS BETWEEN DISPATCHER AND HOSTS-->\n")
    for item in range(list_of_nodes[i]):
        f.write("\t\t\t<link id=\"DSP1_" + str(i+1) + "__1_" + str(i+1) + "_" + str(item+1) + "\" bandwidth=\"" + str(BANDWIDTH) + "Bps\" latency=\"" + str(LATENCY) + "ms\"/>\n")
    f.write("\t\t\t<link id=\"DSP1_" + str(i + 1) + "__Router1_" + str(i + 1)+ "\" bandwidth=\"" + str(BANDWIDTH) + "Bps\" latency=\"" + str(LATENCY) + "ms\"/>\n")
    f.write("\n")

    # 
    f.write("\t\t\t<!-- LINKS BETWEEN ROUTERS AND HOSTS-->\n")
    for z in range(list_of_nodes[i]):
        f.write("\t\t\t<link id=\"Router1_" + str(i+1) + "__1_" + str(i+1) + "_" + str(z+1) + "\" bandwidth=\"" + str(BANDWIDTH)+ "Bps\" latency=\"" + str(LATENCY) + "ms\"/>\n")
    


    # route between dispatcher and hosts
    f.write("\n\t\t\t<!-- DEFINING ROUTES BETWEEN DISPATCHER AND HOSTS-->\n")
    for item in range(list_of_nodes[i]):
        f.write("\t\t\t<route src=\"Tier1_" + str(i + 1) + "\" dst=\"Tier1_" + str(i + 1) + "_" + str(item + 1) + "\"> <link_ctn id=\"DSP1_" + str(i + 1) + "__1_" + str(i + 1) + "_" + str(item + 1) + "\"/></route>\n")
    f.write("\t\t\t<route src=\"Tier1_" + str(i + 1) + "\" dst=\"Router1_" + str(i + 1) +"\"> <link_ctn id=\"DSP1_" + str(i + 1) + "__Router1_" + str(i + 1)+ "\"/></route>\n")
    f.write("\n")

    # routes
    f.write("\t\t\t<!-- DEFINING ROUTES BETWEEN ALL HOSTS-->\n")
    for a in range(list_of_nodes[i]):
        for b in range(a+1, list_of_nodes[i]):
            f.write("\t\t\t<route src=\"Tier1_" + str(i+1) + "_" + str(a+1) + "\" dst=\"Tier1_" + str(i+1) + "_" + str(b+1) + "\"> <link_ctn id=\"1_" + str(i+1) + "_" + str(a+1) + "__1_" + str(i+1) + "_" + str(b+1) + "\"/></route>\n")
        f.write("\n")

    # routers
    f.write("\t\t\t<!-- DEFINING ROUTERS. ROUTES BETWEEN THEM AND HOSTS-->\n")
    # f.write("\t<router id=\"Router1_" + str(i + 1) + "\"/>\n")
    for z in range(list_of_nodes[i]):
        f.write("\t\t\t<route src=\"Router1_" + str(i + 1) + "\" dst=\"Tier1_" + str(i + 1) + "_" + str(z + 1) + "\"> <link_ctn id=\"Router1_" + str(i + 1) + "__1_" + str(i + 1) + "_" + str(z + 1) + "\"/></route>\n")
        #f.write("\n")
    f.write("\t\t</AS>\n\n")

# links  between AS
for x in range(NUMBER_OF_TIERS):
    for y in range(x+1, NUMBER_OF_TIERS):
        f.write("\t<link id=\"RL1" + str(x+1) + "_1" + str(y+1) + "\" bandwidth=\"" + str(BANDWIDTH) + "Bps\" latency=\"" + str(LATENCY) + "ms\"/>\n")
    # link between tier0 and other AS    
    f.write("\t<link id=\"Tier0_Tier1_" + str(x+1) + "\" bandwidth=\"" + str(BANDWIDTH) + "Bps\" latency=\"" + str(LATENCY) + "ms\"/>\n")
    f.write("\n")

# routs between AS
for x in range(NUMBER_OF_TIERS):

    # routes between AS and TIER0
    f.write("<ASroute src=\"Tier0X\" dst=\"Tier1" + str(x+1) + "\"\n")
    f.write("\tgw_src=\"Router0\" gw_dst=\"Router1_" + str(x+1) + "\">\n")
    f.write("\t<link_ctn id=\"Tier0_Tier1_" + str(x+1) + "\"/>\n")
    f.write("</ASroute>\n")
    f.write("\n")    
    
    for y in range(x+1, NUMBER_OF_TIERS):
        f.write("<ASroute src=\"Tier1" + str(x+1) + "\" dst=\"Tier1" + str(y+1) + "\"\n")
        f.write("\tgw_src=\"Router1_" + str(x+1) + "\" gw_dst=\"Router1_" + str(y+1) + "\">\n")
        f.write("\t<link_ctn id=\"RL1" + str(x+1) + "_1" + str(y+1) + "\"/>\n")
        f.write("</ASroute>\n")
        f.write("\n")
    f.write("\n")
f.write("\t</AS>\n")
f.write("</platform>\n")
f.close()


# DEPLOYMENT FILE
f = open("deployment.xml", "w")
f.write("<?xml version='1.0'?>\n")
f.write("<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n")
f.write("<platform version=\"4\">\n")
f.write("\n")
f.write("\t<process host=\"Tier0\" function=\"scheduler\">\n")
f.write("\t\t<argument value=\"0\"/>  <!-- Input mailbox -->\n")
f.write("\t\t<argument value=\"1\"/>  <!-- Input mailbox -->\n")
f.write("\t\t<argument value=\"7\"/>  <!-- Input mailbox -->\n")
f.write("\t\t</process>\n")
f.write("\n")

for i in range(0, NUMBER_OF_TIERS):
    f.write("\t<process host=\"Tier1_" + str(i + 1) + "\" function=\"dispatcher\">\n")
    f.write("\t\t<argument value=\"" + str(i+1) + "\"/>\n")
    f.write("\t\t<argument value=\"" + str(list_of_nodes[i]) + "\"/>\n")
    f.write("\t</process>\n\n")
    for num in range(1, list_of_nodes[i]+1):
        f.write("\t<process host=\"Tier1_" + str(i+1) + "_" + str(num) + "\" function=\"tier1\">\n")
        f.write("\t\t<argument value=\"" + str(i+1) + "\"/>\n")
        f.write("\t\t<argument value=\"" + str(num) + "\"/>\n")
        f.write("\t</process>\n\n")
        # f.write("\t<process host=\"Tier1_" + str(i+1) + "_" + str(num) + "\" function=\"LHCb.SenderDownloader\">\n")
        # f.write("\t\t<argument value=\"" + str(0) + "\"/>\n")
        # f.write("\t</process>\n\n")
    f.write("\n\n\n")
f.write("</platform>")
f.close()




