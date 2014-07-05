./qemu-system-arm -M vexpress-a15 -cpu cortex-a15 -m 128  -nographic \
-singlestep -kernel kernel.elf -s -S \
# -d exec,cpu,guest_errors -D qemu.log 
# skip: -singlestep
# try skip -cpu, as str r0, [fp,#-8] not write onto mem
# -cpu cortex-a15
# -s              shorthand for -gdb tcp::1234
# -S freeze at startup
