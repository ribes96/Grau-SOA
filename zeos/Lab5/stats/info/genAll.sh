#!/bin/bash

./IncClientsgenCSV.py BoCoServerSocket.json
./IncClientsgenCSV.py UnCoServerSocket.json
./IncClientsgenCSV.py ExBoServerSocket.json

./IncItersgenCSV.py BoCoServerSocket.json
./IncItersgenCSV.py UnCoServerSocket.json
./IncItersgenCSV.py ExBoServerSocket.json
