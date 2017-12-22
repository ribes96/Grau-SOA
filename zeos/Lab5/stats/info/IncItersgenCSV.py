#!/usr/bin/python3

import json
import sys

with open(sys.argv[1], 'r') as fp:
    data = json.load(fp)

name = sys.argv[1].split(".")[0]
prefix = "Info_" + name + "/IncIters/"
for nclients in range(10,40,5):
    nclis = str(nclients)

    outf = open(prefix + nclis + "_clients.csv", "x")
    outf.write(name + "With " + nclis + " clients,;\n")
    outf.write("Number iterations, time;\n")


    # keylist = data.keys()
    # keylist.sort()
    for iters in range(50,500,50):
        nits = str(iters)
        # real = data[req][nits]['ack_clients']
        t = data[nclis][nits]['avg_time']
        outf.write(nits + "," +  str(t) + ";\n")

    outf.close()
print("File has been generated succesfully!!")
