# noled - (xerpi, rereprep)
noled is a simple plugin to turn off the blue home button led of PS Vita. Especially useful at night. With the release of v1.2, it will now disable both the Home Button LED and Game Cartridge LED.

# vitabright - (devnoname120)
vitabright is a plugin to further diminish the screen's minimum brightness - marked as not working, probably since the setting wouldn't be reapplied after resuming from sleep

# GN
GN is simply the above two plugins' source codes united so that, upon resuming from sleep, noled will reapply vitabright's patch together with its own.

v1.3 - added modification of color space, less saturated colors on OLED vita


It is a kernel plugin so enable it in kernel section of your config.txt.

copy GN.skprx to tai folder (ux0:/tai)
edit config.txt:

*KERNEL

ux0:tai/GN.skprx

after that, reboot your vita and restart henkaku.

All credits to xerpi, rereprep, devnoname120 - all i did was combine their two projects to fix vitabright's resume from sleep bug and add a couple lines of code.

Tested on OLED wifi vita - Just like with NoLed, there seems to be a slight delay after resuming from sleep.
