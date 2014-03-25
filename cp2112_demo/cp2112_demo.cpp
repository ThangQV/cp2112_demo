#include <windows.h>
#include <stdio.h>
#include "battery.h"

#define BITRATE_HZ          50000
#define ACK_ADDRESS         0x02
#define AUTO_RESPOND        FALSE
#define WRITE_TIMEOUT_MS    1000
#define READ_TIMEOUT_MS     1000
#define TRANSFER_RETRIES    3
#define SCL_LOW_TIMEOUT     FALSE
#define RESPONSE_TIMEOUT_MS 1000

int main(int argc, char* argv[])
{
    HID_SMBUS_DEVICE    m_hidSmbus;
    BYTE                buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE];

    // Open device
    if(SMBus_Open(&m_hidSmbus) != 0)
    {
        fprintf(stderr,"ERROR: Could not open device.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        fprintf(stderr,"Device successfully opened.\r\n");
    }

    // Configure device
    if(SMBus_Configure(&m_hidSmbus, BITRATE_HZ, ACK_ADDRESS, AUTO_RESPOND, WRITE_TIMEOUT_MS, READ_TIMEOUT_MS, SCL_LOW_TIMEOUT, TRANSFER_RETRIES, RESPONSE_TIMEOUT_MS) != 0)
    {
        fprintf(stderr,"ERROR: Could not configure device.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        fprintf(stderr,"Device successfully configured.\r\n");
    }
    
    // Read Voltage [0x09]
    if(SMBus_Read(&m_hidSmbus, buffer, SLAVE_WRITE_ADDRESS, VOLTAGE, 2) != 0)
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        UINT16 voltage_mV = (buffer[1] << 8) | buffer[0];
        fprintf(stderr, "Voltage = %d mV\r\n", voltage_mV);
    }

    // Read Current [0x0A]
    if(SMBus_Read(&m_hidSmbus, buffer, SLAVE_WRITE_ADDRESS, CURRENT, 2) != 0)
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        INT16 current_mA = (buffer[1] << 8) | buffer[0];
        fprintf(stderr, "Current = %d mA\r\n", current_mA);
    }

    // Relative State of Charge [0x0D]
    if(SMBus_Read(&m_hidSmbus, buffer, SLAVE_WRITE_ADDRESS, RELATIVE_STATE_OF_CHARGE, 1) != 0)
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        UINT16 rsoc = (buffer[1] << 8) | buffer[0];
        fprintf(stderr, "RSOC = %d %%\r\n", rsoc);
    }

    // Remaining Capacity [0x0F]
    if(SMBus_Read(&m_hidSmbus, buffer, SLAVE_WRITE_ADDRESS, REMAINING_CAPACITY, 2) != 0)
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        UINT16 remCap = (buffer[1] << 8) | buffer[0];
        fprintf(stderr, "Remaining Capacity = %d mAh\r\n", remCap);
    }

    // Average Time to Empty [0x12]
    if(SMBus_Read(&m_hidSmbus, buffer, SLAVE_WRITE_ADDRESS, AVERAGE_TIME_TO_EMPTY, 2) != 0)
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(&m_hidSmbus);
        return -1;
    }
    else
    {
        UINT16 avgTimeToEmpty = (buffer[1] << 8) | buffer[0];
        fprintf(stderr, "Average Time to Empty = %d min(s)\r\n", avgTimeToEmpty);
    }

    // Success
    fprintf(stderr, "Done! Exiting...\r\n");
    SMBus_Close(&m_hidSmbus);
    return 0;
}