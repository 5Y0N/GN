#include <stdio.h>
#include <string.h>
#include <taihen.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/display.h>

void _start() __attribute__ ((weak, alias ("module_start")));

static SceUID hook[2];

int ksceDisplaySetBrightness(int display, int brightness);
int ksceGpioPortClear(int bus, int port);
int vbright = 1;

static tai_hook_ref_t hook_disable_led;
SceUID disable_led_other(int bus, int port) 
{
	if ((bus == 0) && ((port == 7) | (port == 6)))
	{
		return 0;	
	}
	ksceDisplaySetBrightness(0, vbright);
	return TAI_CONTINUE(SceUID, hook_disable_led, bus, port);
}

static tai_hook_ref_t hook_set_brightness;
int dim_screen_other(int brightness)
{
	int ret;

	if (brightness < 100)
		vbright = 1;
	else
		vbright = brightness;

	ret = TAI_CONTINUE(int, hook_set_brightness, vbright);

	ksceDisplaySetBrightness(0, vbright);

	return ret;
}

static void disable_led()
{
	ksceGpioPortClear(0, 7);
	ksceGpioPortClear(0, 6);
	ksceDisplaySetBrightness(0, vbright);
}

int module_start(SceSize argc, const void *args)
{
	disable_led();

	hook[0] = taiHookFunctionExportForKernel(KERNEL_PID,                
                                 &hook_disable_led,        
                                 "SceLowio",             
                                 TAI_ANY_LIBRARY,           
                                 0xD454A584,                  
                                 disable_led_other); 

	hook[1] = taiHookFunctionExportForKernel(KERNEL_PID,
								&hook_set_brightness,
								"SceAVConfig",
								TAI_ANY_LIBRARY,
								0xE0C1B743,
								dim_screen_other);
	  
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
	if (hook[0] >= 0) taiHookReleaseForKernel(hook[0], hook_disable_led);
	if (hook[1] >= 0) taiHookReleaseForKernel(hook[1], hook_set_brightness);

	return SCE_KERNEL_STOP_SUCCESS;
}
