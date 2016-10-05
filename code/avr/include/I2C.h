#ifndef __I2C__
#define __I2C__

#include <stdint.h>

class I2C
{
	public:
	I2C (void);
	static void isr (void*);
	void init (uint8_t ownSlaveAddress, bool enableGeneralCall = false);
	
	private:
	void check (void);
	uint8_t status (void);
	uint8_t read8 (void);
	void write8 (uint8_t data);
	void enableDataAck (void);
	
	private: // Callbacks
	void null_cb (void);
	
	void slaw_cb (void);
	void slaw_arbitrationLost_cb (void);
	void gca_cb (void);
	void gca_arbitrationLost_cb (void);
	void slaw_read_cb (void);
	void slaw_read_noAck_cb (void);
	void gca_read_cb (void);
	void gca_read_noAck_cb (void);
	void slaw_stop_cb (void);
	
	void slar_cb (void);
	void slar_arbitrationLost_cb (void);
	void slar_write_cb (void);
	void slar_write_noAck_cb (void);
	void slar_lastWrite_withAck_cb (void);
		
	public:
	virtual void reset () {}
	virtual void push  (uint8_t data) {}
	virtual uint8_t pull () {return 0xFF;}
	virtual void stop () {}

};

#endif
