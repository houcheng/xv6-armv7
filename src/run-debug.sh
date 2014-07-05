./qemu-system-arm -M vexpress-a15 -cpu cortex-a15 -m 128  -nographic \
-singlestep -kernel kernel.elf -s -S \
# -s              shorthand for -gdb tcp::1234
# -S freeze at startup
