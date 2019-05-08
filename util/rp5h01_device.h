#ifndef RP5H01_DEVICE_H
#define RP5H01_DEVICE_H

#pragma once

class rp5h01_device
{
public:
	rp5h01_device(unsigned char * data);
    void enable_w(int state);
    void reset_w(int state);
    void cs_w(int state);
    void clock_w(int state);
    void test_w(int state);

    int counter_r();
    int data_r();

	void device_reset();

protected:
    

private:
	/* these also work as the address masks */
	enum {
		COUNTER_MODE_6_BITS = 0x3f,
		COUNTER_MODE_7_BITS = 0x7f
	};

	// internal state
	int m_counter;
	int m_counter_mode;   /* test pin */
	int m_enabled;        /* chip enable */
	int m_old_reset;      /* reset pin state (level-triggered) */
	int m_old_clock;      /* clock pin state (level-triggered) */
	
	// Data in prom
    unsigned char * data;    
};  

// Playchoice prom handling
void prom_w(rp5h01_device * m_rp5h01, int data);
int prom_r(rp5h01_device * m_rp5h01, bool promOverride);

#endif
