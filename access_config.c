#include<linux/pci.h>
#include<linux/init.h>
#include<linux/module.h>

#define PCI_RTL8822CE_VENDORID 0X10ec
#define PCI_RTL8822CE_DEVICEID 0Xc822

/* Meta information */
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
static int rtl8822ce_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
    u16 vid = 0;
    u16 did = 0;
    u8 capability_ptr = 0;
    u32 bar0 = 0;
    u32 saved_bar0 = 0;

    printk("rtl8822ce - In probe function\n");
    
    /* Let's read the PCIe VID and DID */
	if(0 != pci_read_config_word(dev, 0x0, &vid)) {
		printk("rtl8822ce - Error reading from config space\n");
		return -1;
	}
	printk("rtl8822ce - VID; 0x%x\n", vid);

	if(0 != pci_read_config_word(dev, 0x2, &did)) {
		printk("rtl8822ce - Error reading from config space\n");
		return -1;
	}
	printk("rtl8822ce - DID; 0x%x\n", did);

	/* Read the pci capability pointer */
	if(0 != pci_read_config_byte(dev, 0x34, &capability_ptr)) {
		printk("rtl8822ce - Error reading from config space\n");
		return -1;
	}

	if(capability_ptr) 
		printk("rtl8822ce - PCI card has capabilities!\n");
	else
		printk("rtl8822ce - PCI card doesn't have capabilities!\n");


	if(0 != pci_read_config_dword(dev, 0x10, &bar0)) {
		printk("rtl8822ce - Error reading from config space\n");
		return -1;
	}

	saved_bar0 = bar0;

	if(0 != pci_write_config_dword(dev, 0x10, 0xffffffff)) {
		printk("rtl8822ce - Error writing to config space\n");
		return -1;
	}

	if(0 != pci_read_config_dword(dev, 0x10, &bar0)) {
		printk("rtl8822ce - Error reading from config space\n");
		return -1;
	}

	if((bar0 & 0x3) == 1) 
		printk("rtl8822ce - BAR0 is IO space\n");
	else
		printk("rtl8822ce - BAR0 is memory space\n");

	bar0 &= 0xFFFFFFFC;
	bar0 = ~bar0;
	bar0++;

	printk("rtl8822ce - BAR0 is %d bytes big\n", bar0);


	if(0 != pci_write_config_dword(dev, 0x10, saved_bar0)) {
		printk("rtl8822ce - Error writing to config space\n");
		return -1;
	}


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
