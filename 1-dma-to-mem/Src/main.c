#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"

#define BUFFER_SIZE     5

uint16_t sensor_data[BUFFER_SIZE] = { 892, 731, 1234, 90, 23 };
uint16_t data_transfer[BUFFER_SIZE];

volatile uint8_t g_transfer_complete;

int main(void)
{
    g_transfer_complete = 0;

    uart2_tx_init();

    dmaMemToMemConfig();

    dmaTransferStart((uint32_t) sensor_data, (uint32_t) data_transfer, BUFFER_SIZE);

    /* Wait until transfer complete */
    while (!g_transfer_complete)
    {
        /* Do nothing */
    }

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("Temporary buffer[%d]: %d\r\n", i, data_transfer[i]);
    }

    g_transfer_complete = 0;

    while (1)
    {

    }
}

void DMA2_Stream0_IRQHandler(void)
{

    /* Check if transfer complete interrupt occurred */
    if ((DMA2->LISR) & LISR_TCIF0)
    {
        g_transfer_complete = 1;

        /* Clear flag */
        DMA2->LIFCR |= LIFCR_CTCIF0;
    }

    /* Check if transfer error occurred */
    if ((DMA2->LISR) & LISR_TEIF0)
    {
        /* Do something... */
        /* Clear flag */
        DMA2->LIFCR |= LIFCR_CTEIF0;

    }

}
