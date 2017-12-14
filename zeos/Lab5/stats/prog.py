#!//usr/bin/python3

from os import listdir
from os.path import isfile, join
import sys


# Expected mypath: BoCo, UnCo, ExBo
mypath = sys.argv[1]

onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]

times = []

for f in onlyfiles:
    g = open(mypath + "/" + f, "r")
    a = g.read()
    b = a.split()
    c = int(b[1])
    times.append(c)
    
average = sum(times)/len(times)
print("Average:", average)