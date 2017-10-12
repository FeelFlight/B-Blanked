#!/usr/bin/env bash

sed -i "s/WLANSSID/$WLANSSID/g"     src/main.cpp
sed -i "s/WLANPASSWD/$WLANPASSWD/g" src/main.cpp
