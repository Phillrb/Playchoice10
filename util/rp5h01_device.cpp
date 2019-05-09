// Original source from:
// https://github.com/mamedev/mame/blob/master/src/devices/machine/rp5h01.cpp
// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria

// Original source from:
// https://github.com/mamedev/mame/blob/master/src/mame/machine/playch10.cpp
// license:BSD-3-Clause
// copyright-holders:Ernesto Corvi,Brad Oliver

// Minor alterations for 'PC10' by @NES4Life
// BSD 3-Clause License
// Copyright (c) 2019, Phillip Riscombe-Burton
// All rights reserved.

#include "rp5h01_device.h"
#include "../util/util.h"
#include <iostream>

// Ricoh Prom
rp5h01_device::rp5h01_device(unsigned char * data)
	: data(data)
{
    device_reset();
}

void rp5h01_device::device_reset()
{
	m_counter = 0;
	m_counter_mode = COUNTER_MODE_6_BITS;
	m_enabled = 0;
	m_old_reset = 0;
	m_old_clock = 0;

    /* reset the security chip */
	enable_w(1);
	enable_w(0);
	reset_w(0);
	reset_w(1);
}

void rp5h01_device::enable_w(int state)
{
	/* process the /CE signal and enable/disable the IC */
	m_enabled = state ? 0 : 1;
}

void rp5h01_device::reset_w(int state)
{
	/* if it's not enabled, ignore */
	if (!m_enabled)
		return;

	/* now look for a 0->1 transition */
	if (!m_old_reset && state)
	{
		/* reset the counter */
		m_counter = 0;
	}

	/* update the pin */
	m_old_reset = state;
}

void rp5h01_device::cs_w(int state)
{
	std::cout << "CS" << std::endl;
	/* if it's not enabled, ignore */
	if (!m_enabled)
		return;

	if (state)
	{
		/* reset the counter */
		m_counter = 0;
	}
}

void rp5h01_device::clock_w(int state)
{
	/* if it's not enabled, ignore */
	if (!m_enabled)
		return;

	/* now look for a 1->0 transition */
	if (m_old_clock && !state)
	{
		/* increment the counter, and mask it with the mode */
		m_counter++;
	}

	/* update the pin */
	m_old_clock = state;
}

void rp5h01_device::test_w(int state)
{
	/* if it's not enabled, ignore */
	if (!m_enabled)
		return;

	/* process the test signal and change the counter mode */
	m_counter_mode = (state) ? COUNTER_MODE_7_BITS : COUNTER_MODE_6_BITS;
}

int rp5h01_device::counter_r()
{
	/* if it's not enabled, ignore */
	if (!m_enabled)
		return 1; /* high impedance */

	/* return A5 */
	return (m_counter >> 5) & 1;
}

int rp5h01_device::data_r()
{
	/* if it's not enabled, ignore */
	if (!m_enabled)
		return 1; /* high impedance */

	/* get the byte offset and bit offset */
	int byte = (m_counter & m_counter_mode) >> 3;
	int bit = 7 - (m_counter & 7);

	/* return the data */
	return (data[byte] >> bit) & 1;
}

// PLAYCHOICE 
// Playchoice uses the data lines to control the rp5h01 behaviour
int prom_r(rp5h01_device * m_rp5h01, bool promOverride)
{
	int data = 0xe7;

    // PCB replacement of security IC?
    // Override value - provided 'Security IC replacement PCB' - 2 buffers pulling D4, D3 to 5v
    if (promOverride) return data;

    data |= ((~m_rp5h01->counter_r()) << 4) & 0x10;  /* D4 */
    data |= (m_rp5h01->data_r() << 3) & 0x08;        /* D3 */
	
	return data;
}

void prom_w(rp5h01_device * m_rp5h01, int data)
{
    m_rp5h01->test_w(data & 0x10);       /* D4 */
    m_rp5h01->clock_w(data & 0x08);      /* D3 */
    m_rp5h01->reset_w(~data & 0x01);     /* D0 */
}
