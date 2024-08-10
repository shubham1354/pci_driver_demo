#include<linux/module.h>
#include<linux/pci.h>
#include<linux/init.h>

#define 
#define PCI_RTL8822CE_VENDORID 0X10ec
#define PCI_RTL8822CE_DEVICEID 0Xc822

/* Meta information */
unbind C-b
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBHAM");
MODULE_DESCRIPTION("SIMPLE HELLO WORLD CODE");

static struct pci_device_id rtl8822ce_device_ids[] = { {PCI_DEVICE(PCI_RTL8822CE_VENDORID, PCI_RTL8822CE_DEVICEID) }, { 0,} };

MODULE_DEVICE_TABLE(pci, rtl8822ce_device_ids);

/**
 * @brief      This function is called when the pci device is registered
 * 
 * @param dev  Pointer to the pi device
 * @param id   pointer to the correponding id table's entry
 *
 * @return     0 on success
 *             negative error code on failure
 */
static int rtl8822ce_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    printk("rtl8822ce - In probe function\n");
    return 0;
}

/**
 * @brief      This function is called when the pci device is registered
 * 
 * @param dev  Pointer to the pi device
 *
 */
static void rtl8822ce_remove(struct pci_dev *pdev)
{
    printk("rtl8822ce - In remove function\n");
}

/* pci driver struct */
static struct pci_driver rtl8822ce_driver = {
    .name = "my_rtl8822ce",
    .id_table = rtl8822ce_device_ids,
    .probe = rtl8822ce_probe,
    .remove = rtl8822ce_remove,
};

/**
 * @brief This function is called when the module is loaded into the kernel
 */

static int __init my_init(void) {
    printk("In my init function to register the driver\n");
    return pci_register_driver(&rtl8822ce_driver);
}

/**
 * @brief This function is called when the module is removed from the kernel
 */
static void __exit my_exit(void) {
    printk("In my exit function to deregister the driver\n");
    pci_unregister_driver(&rtl8822ce_driver);
}

module_init(my_init);
module_exit(my_exit);
