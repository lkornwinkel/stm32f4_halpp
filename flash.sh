#!/bin/sh

set +x

if [ "$#" -ne 1 ]; then
  echo "specify bin file"
  exit 1
fi

openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c init -c "reset halt" -c "flash write_image erase $1 0x08000000" -c reset -c shutdown

