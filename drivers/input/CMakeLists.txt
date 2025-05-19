# Copyright (c) 2025 George Norton
# SPDX-License-Identifier: MIT

zephyr_library_amend()

zephyr_library_sources_ifdef(CONFIG_INPUT_PIXART_PMW3360 input_pmw3360.c)
