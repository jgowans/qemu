#include "qemu/osdep.h"
#include "hw/hw.h"
#include "hw/pci/pci.h"
#include "qemu/event_notifier.h"

#define TYPE_PCI_JGOWANS_DMA_TEST_DEVICE "jgowans-dma-test"
#define JGOWANS_DMA_TEST(obj) OBJECT_CHECK(JgowandDmaTestState, obj, TYPE_PCI_JGOWANS_DMA_TEST_DEVICE)

typedef struct {
	PCIDevice pdev;
	MemoryRegion mmio;
} JgowansDmaTestState;

static uint64_t mmio_read(void *opaque, hwaddr addr, unsigned int size) {
	printf(TYPE_PCI_JGOWANS_DMA_TEST_DEVICE " mmio_read addr = 0x%lx size =0x%x\n", addr, size);
	return 0xaabbccdd;
}

static void mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size) {
	// JgowansDmaTestState state = JGOWANS_DMA_TEST(opaque);
	printf(TYPE_PCI_JGOWANS_DMA_TEST_DEVICE " mmio_read addr = 0x%lx val = 0x%lx size 0x%x\n", addr, val, size);
	// pci_set_irq(&state->pdev, 1);
}

static const MemoryRegionOps mmio_ops = {
	.read = mmio_read,
	.write = mmio_write,
	.endianness = DEVICE_NATIVE_ENDIAN
};

static void realize(PCIDevice *pdev, Error **errp) {
	JgowansDmaTestState *state = DO_UPCAST(JgowansDmaTestState, pdev, pdev);
	memory_region_init_io(&state->mmio, OBJECT(state), &mmio_ops, state, TYPE_PCI_JGOWANS_DMA_TEST_DEVICE, 8);
	pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &state->mmio);
}

static void class_init(ObjectClass *klass, void *data) {
	printf("in jgowans clss init\n");
	PCIDeviceClass *pci_class = PCI_DEVICE_CLASS(klass);
	pci_class->realize = realize;
	pci_class->vendor_id = PCI_VENDOR_ID_QEMU;
	pci_class->device_id = 0x6345;
	pci_class->revision = 0;
	pci_class->class_id = PCI_CLASS_OTHERS;
	printf("jgowans finissed class init\n");
}

static void register_types(void) {
	static InterfaceInfo interfaces[] = {
		{ INTERFACE_CONVENTIONAL_PCI_DEVICE },
		{ },
	};
	static const TypeInfo type_info = {
		.name = TYPE_PCI_JGOWANS_DMA_TEST_DEVICE,
		.parent = TYPE_PCI_DEVICE,
		.instance_size = sizeof(JgowansDmaTestState),
		.class_init = class_init,
		.interfaces = interfaces,
	};

	printf("About to register\n");
	type_register_static(&type_info);
	printf("Registeer done\n");
}

type_init(register_types);
