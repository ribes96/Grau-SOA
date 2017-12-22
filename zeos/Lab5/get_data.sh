#!/bin/bash

display_usage() {
	echo "\nUsage: ./launch_make.sh server_name"
    echo "Possible names"
    echo "  BoCoServerSocket"
    echo "  UnCoServerSocket"
    echo "  ExBoServerSocket"
	}

if [  $# -le 0 ]
	then
		display_usage
		exit 1
	fi

for last; do true; done


# Crea los directorios necesarios
for nclients in {10..40..5}
do
    for nit in {50..500..50}
    do
        mkdir -p stats/$last/$nclients/$nit
    done
done


for nclients in {10..40..5}
do
    for nit in {50..500..50}
    do
        ./launchClient $nclients $nit localhost 1234 $last
    done
done
