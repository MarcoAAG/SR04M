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

void test_SR04M_t_PrintingMode_CorrectResponse_ShouldParseDistance(void)
{
    SR04M_Object obj;
    uint16_t distance = 0;
    SR04M_Modes mode = MODE5;

    obj.ctx.readReg  = fake_readReg;
    obj.ctx.writeReg = fake_writeReg;
    obj.ctx.handle   = NULL;

    const char* response = "Gap=1234mm\r\n";
    memcpy(fake_rx_buffer, response, 12);

    SR04M_Status status = SR04M_t_PrintingMode(&obj, &distance, mode);

    TEST_ASSERT_EQUAL(SR04M_OK, status);
    TEST_ASSERT_EQUAL_UINT16(1234, distance);
}

void test_SR04M_t_PrintingMode_InvalidPrefix_ShouldReturnError(void)
{
    SR04M_Object obj;
    uint16_t distance = 0;
    SR04M_Modes mode = MODE5;

    obj.ctx.readReg  = fake_readReg;
    obj.ctx.writeReg = fake_writeReg;
    obj.ctx.handle   = NULL;

    const char* response = "Xap=123mm\r\n";
    memcpy(fake_rx_buffer, response, 12);

    SR04M_Status status = SR04M_t_PrintingMode(&obj, &distance, mode);

    TEST_ASSERT_EQUAL(SR04M_ERROR, status);
}
