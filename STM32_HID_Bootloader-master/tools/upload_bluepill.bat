@echo off
stm32flash.exe -g 0x8000000 -b 115200 -w hid_generic_pc13.bin COM1