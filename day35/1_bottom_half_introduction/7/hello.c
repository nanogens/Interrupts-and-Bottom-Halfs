#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>


#define GPIO_BASE               0x3f200000      // GPIO controller 
#define GPIO_SIZE               0xb4

#define GPFSEL0_OFFSET          0x00
#define GPSET0_OFFSET           0x07
#define GPCLR0_OFFSET           0x0A
#define GPPUD_OFFSET		0x25
#define GPPUDCLK0_OFFSET	0x26

static unsigned int irq_number;
static unsigned int gpio_button = 15;

MODULE_LICENSE("GPL");
uint32_t *mem;



void set_gpio_pulldown(unsigned int gpio)
{
	int register_index = gpio/32;
	unsigned int value = (1 << (gpio % 32));

	mem = (uint32_t *)ioremap(GPIO_BASE, GPIO_SIZE);
	iowrite32(0x01, mem + GPPUD_OFFSET); //enable pull down
	// Wait 150 cycles
	udelay(2000);
	//Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to modify
	iowrite32(value, mem + GPPUDCLK0_OFFSET + register_index);	
	// Wait 150 cycles
	udelay(2000);
	//Write to GPPUD to remove the control signal
	iowrite32(0x00, mem + GPPUD_OFFSET);
	//Write to GPPUDCLK0/1 to remove the clock
	iowrite32(0x00, mem + GPPUDCLK0_OFFSET + register_index);	
	
	
	
	iounmap(mem);
}

static irqreturn_t  button_handler(int irq, void *dev_id)
{
        pr_info("irq:%d\n", irq);
        return IRQ_WAKE_THREAD;
}

static irqreturn_t  button_threaded_handler(int irq, void *dev_id)
{
        pr_info("threaded irq:%d\n", irq);
	mdelay(4000);
        return IRQ_HANDLED;
}



static int test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);

	if (!gpio_is_valid(gpio_button)){
		pr_info("Invalid GPIO:%d\n", gpio_button);
		return -ENODEV;
	}

	pr_info("gpio button:%d is valid\n", gpio_button);
	if (gpio_request(gpio_button, "my_button")) {
		pr_info("GPIO Request Failed on gpio:%d\n", gpio_button);
		return -EINVAL;
	}
	pr_info("GPIO Request successful on gpio:%d\n", gpio_button);
	gpio_direction_output(gpio_button, 0);
	gpio_direction_input(gpio_button);
	gpio_set_debounce(gpio_button, 1000);      // Debounce the button with a delay of 1000ms
	set_gpio_pulldown(gpio_button);
	irq_number = gpio_to_irq(gpio_button);
        pr_info("irq number:%d\n", irq_number);

	return request_threaded_irq(irq_number, button_handler, button_threaded_handler,
			IRQF_TRIGGER_FALLING, 
			"button_interrupt",
			NULL);

}

static void test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	free_irq(irq_number, NULL);
	gpio_free(gpio_button);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
