#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#define GPIO_BASE           0xC8834000      // Odroid-XU4 GPIO controller base address
#define GPIO_SIZE           0x1000

#define GPPUD_OFFSET        0x94
#define GPPUDCLK0_OFFSET    0x98

static unsigned int irq_number;
static unsigned int gpio_button = 18;

MODULE_LICENSE("GPL");

uint32_t *mem;

void set_gpio_pulldown(unsigned int gpio)
{
    int register_index = gpio / 32;
    unsigned int value = (1 << (gpio % 32));

    mem = (uint32_t *)ioremap(GPIO_BASE, GPIO_SIZE);
    iowrite32(0x01, mem + GPPUD_OFFSET);
    udelay(2000);
    iowrite32(value, mem + GPPUDCLK0_OFFSET + register_index);
    udelay(2000);
    iowrite32(0x00, mem + GPPUD_OFFSET);
    iowrite32(0x00, mem + GPPUDCLK0_OFFSET + register_index);

    iounmap(mem);
}

void print_context(void)
{
  if(in_interrupt())
  {
    pr_info ("\nCode is running in interrupt context\n"); // hard irq handler
  }
  else
  {
    pr_info ("\nCode is running in process context\n"); // soft irq handler
  }
}

// soft irq handler
void my_action(struct softirq_action *h)
{
    pr_info("my_action\n");
    print_context();
}

// hard irq handler
static irqreturn_t button_handler(int irq, void *dev_id)
{
    pr_info("irq:%d\n", irq);
    print_context();
    raise_softirq(MY_SOFTIRQ);
    return IRQ_HANDLED;
}

static int test_hello_init(void)
{
    pr_info("%s: In init\n", __func__);

    if (!gpio_is_valid(gpio_button))
    {
        pr_info("Invalid GPIO:%d\n", gpio_button);
        return -ENODEV;
    }

    pr_info("gpio button:%d is valid\n", gpio_button);
    if (gpio_request(gpio_button, "my_button"))
    {
        pr_info("GPIO Request Failed on gpio:%d\n", gpio_button);
        return -EINVAL;
    }

    pr_info("GPIO Request successful on gpio:%d\n", gpio_button);

    gpio_direction_input(gpio_button);
    gpio_set_debounce(gpio_button, 1000);
    set_gpio_pulldown(gpio_button);
    irq_number = gpio_to_irq(gpio_button);
    pr_info("irq number:%d\n", irq_number);
    open_softirq(MY_SOFTIRQ, my_action);

    return request_irq(irq_number, button_handler,
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

