#!/usr/bin/python3
import glob, os, fileinput
os.chdir(".")
for file in glob.glob("*.txt"):
    with fileinput.FileInput(file, inplace=True, backup=None) as file:
        for line in file:
            print(line.replace(",", ", "), end='')