#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xac902c3f, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xff178f6, "__aeabi_idivmod" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xc7034798, "device_destroy" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0x1639159e, "__register_chrdev" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0x27e1a049, "printk" },
	{ 0x7f18aa89, "class_unregister" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xc7f3a5f0, "device_create" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0xadb5559d, "param_ops_byte" },
	{ 0xfe990052, "gpio_free" },
	{ 0x36f8040, "class_destroy" },
	{ 0xb81960ca, "snprintf" },
	{ 0xe5d95985, "param_ops_ulong" },
	{ 0x7617bcba, "__class_create" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

