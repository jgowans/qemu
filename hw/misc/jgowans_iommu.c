#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "qemu/log.h"
#include "hw/irq.h"
#include "hw/pci/pci.h"

#define TYPE_JGOWANS_IOMMU "jgowans-iommu"
#define JGOWANS_IOMMU(obj) OBJECT_CHECK(JgowansIommuState, (obj), TYPE_JGOWANS_IOMMU)

typedef struct JgowansIommuState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    qemu_irq irq;
    int irq_level;
} JgowansIommuState;

static uint64_t lkmc_platform_device_read(void *opaque, hwaddr offset,
                           unsigned size)
{
    printf("lkmc_platform_device_read offset=%llx size=%llx\n",
    		(unsigned long long)offset, (unsigned long long)size);
    return 0;
}

static void lkmc_platform_device_write(void *opaque, hwaddr offset,
                        uint64_t value, unsigned size)
{
    JgowansIommuState *s = (JgowansIommuState *)opaque;

    printf("lkmc_platform_device_write offset=%llx value=%llx size=%llx\n",
    		(unsigned long long)offset, (unsigned long long)value, (unsigned long long)size);
	s->irq_level = !s->irq_level;
	qemu_set_irq(s->irq, s->irq_level);
}

static const MemoryRegionOps lkmc_platform_device_ops = {
    .read = lkmc_platform_device_read,
    .write = lkmc_platform_device_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void lkmc_platform_device_init(Object *obj)
{
    JgowansIommuState *s = JGOWANS_IOMMU(obj);
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);

    printf("lkmc_platform_device_init\n");

	s->irq_level = 0;
    memory_region_init_io(&s->iomem, obj, &lkmc_platform_device_ops, s, "lkmc_platform_device", 0x1000);
    sysbus_init_mmio(sbd, &s->iomem);
    sysbus_init_irq(sbd, &s->irq);
}

static void jgowns_iommu_class_init(ObjectClass *klass, void *data) {
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->user_creatable = true;
    dc->desc = "Jgowans IOMMU";
}

static void jgowans_iommu_register_types(void)
{
    printf("jgowans jgowans iommu register\n");
    static const TypeInfo device_info = {
        .name          = TYPE_JGOWANS_IOMMU,
        .parent        = TYPE_SYS_BUS_DEVICE,
        .instance_size = sizeof(JgowansIommuState),
        .instance_init = lkmc_platform_device_init,
	.class_init    = jgowns_iommu_class_init,
    };
    type_register_static(&device_info);
}

void jgowans_iommu_create(void);

void jgowans_iommu_create(void) {
    printf("jgowans creating iommu\n");
    sysbus_create_simple(TYPE_JGOWANS_IOMMU, 0xfec10000, NULL);
}

type_init(jgowans_iommu_register_types)

