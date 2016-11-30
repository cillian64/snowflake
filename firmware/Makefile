##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

BINARY = snowflake

LDSCRIPT = stm32f042f6p6.ld

include libopencm3.target.mk

dfu: snowflake.bin
	dfu-util -d '0483:df11' -a 0 -s 0x08000000 -D snowflake.bin

debug: snowflake.elf
	arm-none-eabi-gdb \
		-ex 'target extended-remote /dev/cu.usbmodemD6CBAEC1' \
		-ex 'mon swdp_scan' \
		-ex 'attach 1' \
		-ex 'file snowflake.elf'

program: snowflake.elf
	arm-none-eabi-gdb --batch \
		-ex 'target extended-remote /dev/cu.usbmodemD6CBAEC1' \
		-ex 'mon swdp_scan' \
		-ex 'attach 1' \
		-ex 'load snowflake.elf'

power:
	arm-none-eabi-gdb --batch \
		-ex 'target extended-remote /dev/cu.usbmodemD6CBAEC1' \
		-ex 'monitor tpwr enable'

unpower:
	arm-none-eabi-gdb --batch \
		-ex 'target extended-remote /dev/cu.usbmodemD6CBAEC1' \
		-ex 'monitor tpwr disable'
