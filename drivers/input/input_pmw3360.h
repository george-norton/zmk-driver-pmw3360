#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

// Indicates the direction of a SPI transaction
#define PMW3360_SPI_READ 0
#define PMW3360_SPI_WRITE 0x80

// Timing values from the datasheet
#define PMW3360_T_SRAD 160
#define PMW3360_T_SCLK_NCS_READ 120
#define PMW3360_T_SCLK_NCS_WRITE 35
#define PMW3360_T_SRR 20  // Same as T_SRW
#define PMW3360_T_SWR 180 // Same as T_SWW
#define PMW3360_T_SRAD_MOTBR 35

// Select register addresses and values
#define PMW3360_REG_PRODUCT_ID          0x00
#define PMW3360_REG_REVISION_ID         0x01
#define PMW3360_REG_MOTION              0x02
#   define PMW3360_MOTION_MOT           0x80
#define PMW3360_REG_DELTA_X_L           0x03
#define PMW3360_REG_DELTA_X_H           0x04
#define PMW3360_REG_DELTA_Y_L           0x05
#define PMW3360_REG_DELTA_Y_H           0x06
#define PMW3360_REG_CONTROL             0x0D
#   define PMW3360_CONTROL_ROTATE_90    0xC0
#   define PMW3360_CONTROL_ROTATE_180   0x60
#   define PMW3360_CONTROL_ROTATE_270   0xA0
#define PMW3360_REG_CONFIG_1            0x0F
#define PMW3360_REG_CONFIG_2            0x10
#   define PMW3360_CONFIG_2_RTP_MOD     0x04
#   define PWM3360_CONFIG_2_REST_EN     0x20

#define PMW3360_REG_ANGLE_TUNE          0x11
#define PMW3360_REG_POWER_UP            0x3A
#define PMW3360_REG_MOTION_BURST        0x50
#define PMW3360_REG_LIFT_CONFIG         0x63

// The motion burst report, this can be exteneded to read more
// data, but we dont use it, so we dont bother reading it.
struct motion_burst {
    uint8_t motion;
    uint8_t observation;
    uint8_t delta_x_l;
    uint8_t delta_x_h;
    uint8_t delta_y_l;
    uint8_t delta_y_h;
} __packed;

// The drivers private data
struct pmw3360_data {
    const struct device *dev;
    bool ready;
    bool motion_burst_active;
    struct k_work trigger_work;
    struct k_work_delayable init_work;
    struct gpio_callback irq_gpio_cb; // motion pin irq callback
};

// The drivers configuration struture - populated by the device tree
struct pmw3360_config {
    struct spi_dt_spec spi;
    struct gpio_dt_spec cs_gpio;
    struct gpio_dt_spec irq_gpio;
    uint16_t cpi;
    bool rotate_90;
    bool rotate_180;
    bool rotate_270;
    int8_t angle_tune;
    bool lift_height_3mm;
};

#ifdef __cplusplus
}
#endif