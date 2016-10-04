import sys, os, re
x = str(sys.argv[1]) + ".cs"
zz = x[:(x.find("/")+1)]
os.system("pj_dump " + str(sys.argv[1]) + " > " + x)
ff = open(zz + sys.argv[1][(x.find("/")+1):] + ".csv", "w")
with open(x) as f:
    for line in f:
        if (line.startswith("Variable")):
            ff.write(line)
ff.close()
os.remove(x)
os.remove(sys.argv[1])
