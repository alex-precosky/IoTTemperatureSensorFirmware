# IoT Temperature Sensor Firmware

(put links to the related projects here)

# Prerequisites

- Python 3.6+
- venv

PlatformIO Core

python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"

# PlatformIO 

export PATH=$PATH:~/.platformio/penv/bin

# Building

Run `pio run`

```
❯ pio run
Processing default (framework: arduino; platform: atmelavr; board: ATmega328P)
-------------------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/atmelavr/ATmega328P.html
PLATFORM: Atmel AVR (3.1.0) > ATmega328P/PA
HARDWARE: ATMEGA328P 8MHz, 2KB RAM, 32KB Flash
DEBUG: Current (avr-stub) On-board (avr-stub, simavr)
PACKAGES:
 - framework-arduino-avr-minicore 2.0.9
  - toolchain-atmelavr 1.50400.190710 (5.4.0)
  LDF: Library Dependency Finder -> http://bit.ly/configure-pio-ldf
  LDF Modes: Finder ~ chain, Compatibility ~ soft
  Found 9 compatible libraries
  Scanning dependencies...
  Dependency Graph
  |-- <Low-Power> 1.6.0
Building in release mode
Checking size .pio/build/default/firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]  13.5% (used 277 bytes from 2048 bytes)
Flash: [=         ]  13.2% (used 4268 bytes from 32256 bytes)
========================================================== [SUCCESS] Took 1.29 seconds ==========================================================
```

# Flashing
`pio run --upload`
