cmd_/home/rama/Downloads/Vector/kernel_modules/modstacking/modules.order := {   echo /home/rama/Downloads/Vector/kernel_modules/modstacking/core_lkm.ko;   echo /home/rama/Downloads/Vector/kernel_modules/modstacking/user_lkm.ko; :; } | awk '!x[$$0]++' - > /home/rama/Downloads/Vector/kernel_modules/modstacking/modules.order