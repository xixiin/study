#include "CAN.h"
#include "stdio.h"

CAN_HandleTypeDef can_handle = {0};

void CAN_Init(void)
{
    can_handle.Instance = CAN1;
    can_handle.Init.Mode = CAN_MODE_LOOPBACK; // Set CAN mode to loopback for testing
    can_handle.Init.Prescaler = 4; // Set prescaler for CAN baud rate
    can_handle.Init.TimeSeg1 = CAN_BS1_9TQ;
    can_handle.Init.TimeSeg2 = CAN_BS2_8TQ;
    can_handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
    can_handle.Init.AutoBusOff = DISABLE;
    can_handle.Init.AutoWakeUp = DISABLE;
    can_handle.Init.AutoRetransmission = DISABLE;
    can_handle.Init.ReceiveFifoLocked = DISABLE;
    can_handle.Init.TimeTriggeredMode = DISABLE;
    can_handle.Init.TransmitFifoPriority = DISABLE;

    HAL_CAN_Init(&can_handle);

    CAN_FilterTypeDef can_filter = {0};

    can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter.FilterIdHigh = 0x0000;
    can_filter.FilterIdLow = 0x0000;
    can_filter.FilterMaskIdHigh = 0x0000;
    can_filter.FilterMaskIdLow = 0x0000;

    can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    can_filter.FilterBank = 0;
    can_filter.FilterActivation = ENABLE;
    can_filter.SlaveStartFilterBank  = 14;

    HAL_CAN_ConfigFilter(&can_handle, &can_filter); // Configure CAN filter
    HAL_CAN_Start(&can_handle); // Start CAN peripheral
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    __HAL_RCC_CAN1_CLK_ENABLE(); // Enable CAN1 clock
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock
    
    GPIO_InitTypeDef gpio_init={0};
	gpio_init.Pin = GPIO_PIN_12; // CAN TX pin
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
	gpio_init.Pin = GPIO_PIN_11; // CAN RX pin
	gpio_init.Mode = GPIO_MODE_AF_INPUT;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
}

void CAN_send_data(uint32_t id, uint8_t* data, uint8_t length)
{
    CAN_TxHeaderTypeDef tx_header = {0};
    uint32_t tx_mailbox = CAN_TX_MAILBOX0;
    tx_header.ExtId = id;
    tx_header.IDE = CAN_ID_EXT;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = length;

    HAL_CAN_AddTxMessage(&can_handle, &tx_header, data, &tx_mailbox); // Send CAN message

    while(HAL_CAN_GetTxMailboxesFreeLevel(&can_handle) != 3); // Wait until the mailbox is free

    uint8_t i=0;
    for(i=0;i<length;i++)
    {
        printf("data[%d]: %02X", i, data[i]); // Print sent data for debugging

    }
    printf("\r\n");
}

uint8_t CAN_receive_data(uint8_t *buf)
{
    uint8_t i=0;
    CAN_RxHeaderTypeDef rx_header = {0};

    if(HAL_CAN_GetRxFifoFillLevel(&can_handle, CAN_RX_FIFO0) == 0)
    {
        return 0; // No data available
    }

    HAL_CAN_GetRxMessage(&can_handle, CAN_RX_FIFO0, &rx_header, buf); // Receive CAN message
        for(i=0;i<rx_header.DLC;i++)
    {
        printf("data[%d]: %02X", i, buf[i]); // Print received data for debugging
       
    }
     printf("\r\n");
    return rx_header.DLC; // Return the data length code
}
