#include "unity.h"
#include "sr04m.h"

static uint8_t fake_rx_buffer[12];
static uint8_t fake_tx_buffer[1];

static uint32_t fake_writeReg(void* handle, uint8_t* p_data, uint8_t length)
{
    memcpy(fake_tx_buffer, p_data, length);
    return 0;
}

static uint32_t fake_readReg(void* handle, uint8_t* p_data, uint8_t length)
{
    memcpy(p_data, fake_rx_buffer, length);
    return 0; // OK
}

void setUp(void)
{
}

void tearDown(void)
{
}
