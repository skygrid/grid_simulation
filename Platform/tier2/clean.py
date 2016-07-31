w = open("csv.csv", "w")
with open("clean.csv") as f:
    while True:
        c = f.read(1)
        if not c:
            print "End of file"
            break
        if c == ",":
            c = ""
        w.write(c)
