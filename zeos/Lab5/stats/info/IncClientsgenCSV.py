#!/usr/bin/python3

import json
import sys

with open(sys.argv[1], 'r') as fp:
    data = json.load(fp)

for iters in range(50,500,50):
    nits = str(iters)
    name = sys.argv[1].split(".")[0]
    outf = open("Info_" + name + "_" + nits + "_iters.csv", "x")
    outf.write("Requested clients, real clients, time;\n")


    # keylist = data.keys()
    # keylist.sort()
    for req in sorted(data):
        real = data[req][nits]['ack_clients']
        t = data[req][nits]['avg_time']
        outf.write(str(req) + "," +  str(real) + "," +  str(t) + ";\n")

    outf.close()
    print(data.keys())
    print(data['20'].keys())
    print(data['20']['50'].keys())
