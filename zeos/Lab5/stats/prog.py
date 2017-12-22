#!/usr/bin/python3

from os import listdir
from os.path import isfile, join, isdir
import sys
import json


# Expected mypath: BoCo, UnCo, ExBo
mypath = sys.argv[1]

nclinets_list = [f for f in listdir(mypath) if isdir(join(mypath, f))]


clients_dic = {}
for nclients in nclinets_list:
    print("Hola")
    new_path = mypath + "/" + nclients
    nit_list = [f for f in listdir(new_path) if isdir(join(new_path, f))]
    times = []
    nit_dic = {}
    for nit in nit_list:
        new_new_path = new_path + "/" + nit
        clients_list = [f for f in listdir(new_new_path) if isfile(join(new_new_path, f))]
        times_per_client = []
        for client in clients_list:
            g = open(new_new_path + "/" + client, "r")
            a = g.read()
            b = a.split()
            g.close()
            print(nclients,nit,client, sep = ":")
            if len(b) > 0 and b[1].isdigit():
                c = int(b[1])
                times_per_client.append(c)
        nclis = len(times_per_client)
        if nclis > 0:
            average_time = sum(times_per_client)/nclis
        else: average_time = "no clients served"
        data = {"ack_clients":nclis, "avg_time":average_time}
        nit_dic[int(nit)] = data
    clients_dic[int(nclients)] = nit_dic

with open("info/" + mypath + ".json", 'w') as fp:
    json.dump(clients_dic, fp)


# for f in onlyfiles:
#     g = open(mypath + "/" + f, "r")
#     a = g.read()
#     b = a.split()
#     c = int(b[1])
#     times.append(c)
#
# average = sum(times)/len(times)
# print("Average:", average)
