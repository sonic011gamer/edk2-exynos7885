cd BootShim
rm BootShim.bin
make UEFI_BASE=0x90000000 UEFI_SIZE=0x00900000
rm BootShim.elf
cd ..