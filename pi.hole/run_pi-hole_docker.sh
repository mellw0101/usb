#!/bin/sh

docker run -d --cap-add=NET_ADMIN -e ServerIP=192.168.0.14 -e WEBPASSWORD=225588 -p 0.0.0.0:8080:80 --restart=unless-stopped pihole/pihole

