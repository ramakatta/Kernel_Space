cmd_/home/rama/Downloads/Manipal/kernel_space/raspberry/rgb_led/rgbled.mod := printf '%s\n'   rgbled.o | awk '!x[$$0]++ { print("/home/rama/Downloads/Manipal/kernel_space/raspberry/rgb_led/"$$0) }' > /home/rama/Downloads/Manipal/kernel_space/raspberry/rgb_led/rgbled.mod