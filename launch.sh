#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "try to use sudo"
  exit
fi

mkdir -p build
cd build
cmake ..
make
make install

systemctl daemon-reload

systemctl enable process_monitor.service
systemctl start process_monitor.service

echo "сервис установлен и запущен!"