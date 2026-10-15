#include "sys.h"
#include "uart1.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "CAN.h"

uint8_t data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t received_data[8] = {0};

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */                       
    uart1_init(115200);
    printf("running...\r\n");
    CAN_Init(); 
    key_init();
    uint8_t i=0;
    while(1)
    { 
        if(key_scand()==1)
        {
            for(i=0;i<8;i++)
            {
                data[i]++; // Increment data for testing
            }
            CAN_send_data(0x12345678, data, 8); // Send CAN message with ID 0x12345678
        }
        
        CAN_receive_data(received_data); // Check for received CAN messages
        
    }
}

