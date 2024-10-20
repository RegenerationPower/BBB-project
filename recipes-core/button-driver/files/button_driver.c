#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>


static struct input_dev *button_dev;
static int button_gpio;
static int button_irq;

static irqreturn_t button_interrupt(int irq, void *dev_id)
{
    input_report_key(button_dev, KEY_ENTER, gpio_get_value(button_gpio));
    input_sync(button_dev);
    printk(KERN_INFO "button_driver.c: Button pressed!\n");
    return IRQ_HANDLED;
}

/* Probe function */
static int button_probe(struct platform_device *pdev)
{
    int ret;
    struct device_node *np = pdev->dev.of_node;

    /* Retrieve GPIO from device tree */
    button_gpio = of_get_named_gpio(np, "gpios", 0);
    if (button_gpio < 0) {
        printk(KERN_ERR "button_driver.c: Failed to get GPIO from DT\n");
        return button_gpio;
    }
    printk(KERN_INFO "button_driver.c: Successfully got GPIO from DT!\n");

    /* Request GPIO */
    ret = devm_gpio_request_one(&pdev->dev, button_gpio, GPIOF_IN, "button");
    if (ret) {
        printk(KERN_ERR "button_driver.c: Failed to request GPIO\n");
        return ret;
    }
    printk(KERN_INFO "button_driver.c: Successfully requested GPIO!\n");

     /* Retrieve IRQ for GPIO */
    button_irq = gpiod_to_irq(gpio_to_desc(button_gpio));
    if (!button_irq) {
        printk(KERN_ERR "button_driver.c: Failed to get IRQ fot GPIO\n");
        return -EINVAL;
    }
    printk(KERN_INFO "button_driver.c: Successfully got IRQ for GPIO!\n");
    
 	printk(KERN_INFO "Button GPIO: %d, IRQ: %d\n", button_gpio, button_irq);

    /* Request IRQ */
    ret = devm_request_irq(&pdev->dev, button_irq, button_interrupt,
                           IRQF_TRIGGER_FALLING, "button", NULL);
    if (ret) {
        printk(KERN_ERR "button_driver.c: Failed to request IRQ\n");
        return ret;
    }
    printk(KERN_INFO "button_driver.c: Successfully requested IRQ!\n");

    /* Allocate input device */
    button_dev = devm_input_allocate_device(&pdev->dev);
    if (!button_dev) {
        printk(KERN_ERR "button_driver.c: Not enough memory\n");
        return -ENOMEM;
    }
    printk(KERN_INFO "button_driver.c: Successfully allocated input device!\n");

    /* Set up input device */
    button_dev->name = "GPIO Button";
    button_dev->evbit[0] = BIT_MASK(EV_KEY);
    button_dev->keybit[BIT_WORD(KEY_ENTER)] = BIT_MASK(KEY_ENTER);

    /* Register input device */
    ret = input_register_device(button_dev);
    if (ret) {
        printk(KERN_ERR "button_driver.c: Failed to register input device\n");
        return ret;
    }
    printk(KERN_INFO "button_driver.c: Successfully registered input device!\n");

    pr_info("Button driver loaded\n");
    return 0;
}

/* Remove function */
static int button_remove(struct platform_device *pdev)
{
    input_unregister_device(button_dev);
    pr_info("Button driver unloaded\n");
    return 0;
}

static const struct of_device_id button_dt_ids[] = {
    { .compatible = "gpio-keys", },
    {},
};

MODULE_DEVICE_TABLE(of, button_dt_ids);

static struct platform_driver button_driver = {
    .probe = button_probe,
    .remove = button_remove,
    .driver = {
        .name = "button_driver",
        .of_match_table = button_dt_ids,
    },
};

/* Module init function */
static int __init button_driver_init(void)
{
    return platform_driver_register(&button_driver);
}

/* Module exit function */
static void __exit button_driver_exit(void)
{
    platform_driver_unregister(&button_driver);
}

module_init(button_driver_init);
module_exit(button_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmytro Voznytsia");
MODULE_DESCRIPTION("Simple device driver - GPIO Button Driver");

