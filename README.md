# PMW3360 optical mouse sensor driver for ZMK

A ZMK driver for the Pixart PMW3360 optical mouse sensor. This sensor has relativley high power consumption, its not recommended for wireless builds.

## Features

- Configurable CPI
- Configurable Orientation
- Can run in polled mode (don't define irq-gpios) or interrupt mode (recommended if you have a motion pin wired up)
- Can run on either the system work queue, or a private one.

## Config options

- `CONFIG_INPUT_PIXART_PMW3360_USE_OWN_THREAD` - process events on a private work queue. This will consume additional memory, but should improve responsiveness (default: y).
- `CONFIG_INPUT_PIXART_PMW3360_THREAD_PRIORITY` - the priority of the drivers work queue thread. Lower values take precedence (default: 5).
- `CONFIG_INPUT_PIXART_PMW3360_THREAD_STACK_SIZE` - the amount of memory to reserve for the drivers work queue thread (default: 768).

## Sample configuration

Use pinctrl to configure your SPI pins (Chip select, Clock, MOSI and MISO).

```
&pinctrl {
	spi0_default: spi0_default {
		group1 {
			pinmux = <SPI0_CSN_P21>, <SPI0_SCK_P22>, <SPI0_TX_P23>;
		};
		group2 {
			pinmux = <SPI0_RX_P20>;
			input-enable;
		};
	};
};
```

Assign the driver to the SPI bus, configure the CPI and orientation here. If you do not have a motion pin wired up, remove `irq-gpios` to run in polled mode.

```
&spi0 {
	status = "okay";
	cs-gpios = <&gpio0 21 GPIO_ACTIVE_LOW>;
	mouse: mouse@0 {
		compatible = "pixart,pmw3360";
		status = "okay";
		reg = <0>;
		spi-max-frequency = <2000000>;

		// pmw3360 driver parameters
		irq-gpios = <&gpio0 27 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>;
		cpi = <600>;
		rotate-90;
	};
};
```

Create and input listener to process the mouse events.

```
/ {
	mouse_listener {
		compatible = "zmk,input-listener";
		device = <&mouse>;
	};
};
```