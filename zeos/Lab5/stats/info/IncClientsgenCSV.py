#!/usr/bin/python3

import json
import sys

with open(sys.argv[1], 'r') as fp:
    data = json.load(fp)

name = sys.argv[1].split(".")[0]
prefix = "Info_" + name + "/IncClients/"
for iters in range(50,500,50):
    nits = str(iters)
    outf = open(prefix + nits + "_iters.csv", "x")
    outf.write(name + "With " + nits + " iterations,,;\n")
    outf.write("Requested clients, real clients, time;\n")


    # keylist = data.keys()
    # keylist.sort()
    for req in sorted(data):
        real = data[req][nits]['ack_clients']
        t = data[req][nits]['avg_time']
        outf.write(str(req) + "," +  str(real) + "," +  str(t) + ";\n")

    outf.close()
print("File has been generated succesfully!!")
