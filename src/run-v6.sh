qemu-system-arm -M versatilepb -m 128 -cpu arm1176  -nographic -singlestep \
-d exec,cpu,guest_errors -D qemu.log -kernel kernel.elf -s -S
# -S stop at begin
# -s : debug at tcp::1234
