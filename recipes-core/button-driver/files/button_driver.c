#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/input.h>

#define BUTTON_GPIO_PIN 5
#define BUTTON_IRQ gpio_to_irq(BUTTON_GPIO_PIN)

static struct input_dev *button_dev;

static irqreturn_t button_interrupt(int irq, void *dev_id)
{
    input_report_key(button_dev, KEY_ENTER, gpio_get_value(BUTTON_GPIO_PIN));
    input_sync(button_dev);
    printk(KERN_INFO "button_driver.c: Button pressed!\n");
    return IRQ_HANDLED;
}

/*Module's init entry point */
static int __init button_driver_init(void)
{
	int ret;
	
    if (!gpio_is_valid(BUTTON_GPIO_PIN)) {
        printk(KERN_ERR "button_driver.c: Invalid GPIO pin\n");  
        return -ENODEV;
    }
    
    if(gpio_direction_input(BUTTON_GPIO_PIN))
    {
    	printk(KERN_ERR "button_driver.c: Cannot change GPIO direction %d\n", BUTTON_GPIO_PIN);
    	return -EBUSY;
    }
	
    if (request_irq(BUTTON_IRQ, button_interrupt, IRQF_TRIGGER_RISING, "button", NULL)) 
    {
		printk(KERN_ERR "button_driver.c: Can't allocate irq %d\n", BUTTON_IRQ);
        ret = -EBUSY;
        goto err_free_gpio;
    }
	
    button_dev = input_allocate_device();
    if (!button_dev) 
    {
        printk(KERN_ERR "button_driver.c: Not enough memory\n");
        ret = -ENOMEM;
        goto err_free_irq;
    }
    
    button_dev->name = "GPIO Button";
    button_dev->evbit[0] = BIT_MASK(EV_KEY);
    button_dev->keybit[BIT_WORD(KEY_ENTER)] = BIT_MASK(KEY_ENTER);
    
    ret = input_register_device(button_dev);
    if (ret) {
        printk(KERN_ERR "button_driver.c: Failed to register input device\n");
        goto err_free_dev;
    }
	
	pr_info("Button driver loaded\n");
	return 0;

err_free_dev:
	input_free_device(button_dev);
err_free_irq:
	free_irq(BUTTON_IRQ, button_interrupt);
err_free_gpio:
	gpio_free(BUTTON_GPIO_PIN);
	return ret;
}

/*Module's exit entry point */
static void __exit button_driver_exit(void)
{
    input_unregister_device(button_dev);
    free_irq(BUTTON_IRQ, button_interrupt);
    gpio_free(BUTTON_GPIO_PIN);
	pr_info("Button driver unloaded\n");		
}

module_init(button_driver_init);
module_exit(button_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmytro Voznytsia");
MODULE_DESCRIPTION("Simple input device - GPIO Button Driver");

