echo run armv7 with qemu logging feature 'qemu.log'
qemu-system-arm -M vexpress-a15 -cpu cortex-a15 -m 128  -nographic \
-singlestep \
-d exec,cpu,guest_errors,in_asm -D qemu.log -kernel kernel.elf 
