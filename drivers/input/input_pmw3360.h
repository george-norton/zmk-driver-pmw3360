#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

struct pmw3360_data {
    const struct device *dev;
    bool ready;
    bool motion_burst_active;
    struct k_work trigger_work;
    struct k_work_delayable init_work;
    struct gpio_callback irq_gpio_cb; // motion pin irq callback
};

struct pmw3360_config {
    struct spi_dt_spec spi;
    struct gpio_dt_spec cs_gpio;
    struct gpio_dt_spec irq_gpio;
    uint16_t cpi;
};

#ifdef __cplusplus
}
#endif