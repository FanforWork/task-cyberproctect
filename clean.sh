#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "try sudo"
  exit
fi



if systemctl is-active --quiet process_monitor.service; then
    systemctl stop process_monitor.service
fi
if systemctl is-enabled --quiet process_monitor.service; then
    systemctl disable process_monitor.service
fi



if [ -f /etc/systemd/system/process_monitor.service ]; then
    rm /etc/systemd/system/process_monitor.service
fi

systemctl daemon-reload


if [ -f /usr/local/bin/process_monitor ]; then
    rm /usr/local/bin/process_monitor
fi
if [ -f /usr/local/bin/client_app ]; then
    rm /usr/local/bin/client_app
fi



if [ -f /var/log/process_monitor.log ]; then
    rm /var/log/process_monitor.log
fi


if [ -d build ]; then
    rm -rf build
fi



echo "всё почистили)"