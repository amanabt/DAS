#include "../include/I2C.h"
#include "../sys/include/hardware.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#include "pgmspace"

using namespace std;

#define I2C_SCL_FREQ             (100e3)

#define I2C_PRESCALER_4          (0x1 << TWPS0)
#define I2C_INTERFACE_ENABLE     (0x1 << TWEN)
#define I2C_ACK_ENABLE           (0x1 << TWEA)

#define I2C_INTFLAG_MASK         (0x1 << TWINT)
#define I2C_INTFLAG_CLEAR        (0x1 << TWINT)

#define I2C_START_ASSERT         (0x1 << TWSTA)
#define I2C_STOP_ASSERT          (0x1 << TWSTO)
#define I2C_STOP_MASK            (0x1 << TWSTO)

#define I2C_STATUS_MASK          (0x1F << TWS3)

#define I2C_OPERATION_READ       (0x1)
#define I2C_OPERATION_WRITE      (0x0)

/********         Interrup Setup         *********/

static void defaultCallback (void *);
typedef void (*Callback)(void*);

static void *user_data_ = 0;
static Callback cb_ = defaultCallback;

void defaultCallback (void *)
{}

ISR (TWI_vect)
{
	(*cb_)(user_data_);
}

void setCallback (Callback cb, void* user_data)
{
	cb_ = cb;
	user_data_ = user_data;
}

/************************************************/

enum I2C_Mode
{
	I2C_MODE_WRITE,
	I2C_MODE_READ,
	I2C_MODE_WRITE_THEN_READ
};

I2C::I2C (void)
{
	
	TWSR = I2C_PRESCALER_4;
	TWBR = (uint8_t)(0.5 +
		(cpu_clock() / I2C_SCL_FREQ - 16) / 2 / pow (4, I2C_PRESCALER_4));
	setCallback (isr, this);	
}

/***************************************************************/

uint8_t I2C::status (void)
{
	return (TWSR & I2C_STATUS_MASK) >> TWS3;
}

void I2C::init (uint8_t ownSlaveAddress, bool enableGeneralCall)
{
	TWAR =  (ownSlaveAddress << 1) | enableGeneralCall;
	TWCR =  I2C_INTFLAG_CLEAR | I2C_ACK_ENABLE | I2C_INTERFACE_ENABLE;
	reset ();
}

void I2C::isr (void* user_data)
{
	reinterpret_cast<I2C*> (user_data)->check();
}

void I2C::check (void)
{
	typedef void (I2C::*cb_t)(void);
	static const cb_t cbs[] PROGMEM =
	{
		////////  Master modes   /////////
		&I2C::null_cb, &I2C::null_cb, &I2C::null_cb, &I2C::null_cb,
		&I2C::null_cb, &I2C::null_cb, &I2C::null_cb, &I2C::null_cb,
		&I2C::null_cb, &I2C::null_cb, &I2C::null_cb, &I2C::null_cb,
		/////////////////////////////////////////
		
		////////  Slave Receiver mode   /////////
		&I2C::slaw_cb, 
		&I2C::slaw_arbitrationLost_cb, 
		&I2C::gca_cb, 
		&I2C::gca_arbitrationLost_cb, 
		&I2C::slaw_read_cb,
		&I2C::slaw_read_noAck_cb,
		&I2C::gca_read_cb,
		&I2C::gca_read_noAck_cb,
		&I2C::slaw_stop_cb,
		/////////////////////////////////////////
		
		////////  Slave Receiver mode   /////////
		&I2C::slar_cb,
		&I2C::slar_arbitrationLost_cb,
		&I2C::slar_write_cb,
		&I2C::slar_write_noAck_cb,
		&I2C::slar_lastWrite_withAck_cb,
		/////////////////////////////////////////
		
	};

	const uint8_t stat = status();

	if (stat < sizeof (cbs) / sizeof (cbs[0]))
		(this->*pgm_read(cbs[stat]))();
}

uint8_t I2C::read8 (void)
{
	return TWDR;
}

void I2C::write8 (uint8_t data)
{
	TWDR = data;
}

void I2C::enableDataAck (void)
{
	TWCR =  I2C_INTFLAG_CLEAR | I2C_ACK_ENABLE | I2C_INTERFACE_ENABLE;
}


/***************************************************************/

void I2C::null_cb (void)
{}
		
void I2C::slaw_cb (void)
{
	reset ();
	enableDataAck();
}

void I2C::slaw_arbitrationLost_cb (void)
{
	reset ();
	enableDataAck();
}

void I2C::gca_cb (void)
{
	enableDataAck();
}

void I2C::gca_arbitrationLost_cb (void)
{
	enableDataAck();
}

void I2C::slaw_read_cb (void)
{
	push(read8());
	enableDataAck();
}

void I2C::slaw_read_noAck_cb (void)
{
	push(read8());
	enableDataAck();
}

void I2C::gca_read_cb (void)
{
	enableDataAck();
}

void I2C::gca_read_noAck_cb (void)
{
	enableDataAck();
}

void I2C::slaw_stop_cb (void)
{
	stop ();
	enableDataAck();
}


void I2C::slar_cb (void)
{
	write8 (pull ());
	enableDataAck();
}

void I2C::slar_arbitrationLost_cb (void)
{
	write8 (pull ());
	enableDataAck();
}

void I2C::slar_write_cb (void)
{
	write8 (pull ());
	enableDataAck();
}

void I2C::slar_write_noAck_cb (void)
{
	enableDataAck();
}

void I2C::slar_lastWrite_withAck_cb (void)
{
	enableDataAck();
}

/***************************************************************/
