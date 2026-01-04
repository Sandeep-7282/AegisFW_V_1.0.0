/*
 * uart_driver.c
 *
 *  USART2 driver with interrupt-driven RX ring buffer.
 *
 */

#include "uart_driver.h"
#include "stm32f446xx.h"

/* APB1 clock frequency (must match clock config) */
#define APB1_CLK_HZ        16000000UL
#define UART2_RX_BUF_SIZE  128U

typedef struct
{
    volatile uint8_t  buf[UART2_RX_BUF_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint8_t  overflow;
} ringbuf_t;

static ringbuf_t uart_rx;

/* ---------------- GPIO ---------------- */

static void uart_gpio_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA2 = TX, PA3 = RX */
    GPIOA->MODER &= ~((3U << 4) | (3U << 6));
    GPIOA->MODER |=  ((2U << 4) | (2U << 6));

    GPIOA->OSPEEDR |= ((3U << 4) | (3U << 6));

    GPIOA->AFR[0] &= ~((0xFU << 8) | (0xFU << 12));
    GPIOA->AFR[0] |=  ((7U << 8) | (7U << 12));
}

/* ---------------- Core ---------------- */

static void uart_core_init(uint32_t baud)
{
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    USART2->CR1 &= ~USART_CR1_UE;

    USART2->BRR = APB1_CLK_HZ / baud;

    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 |= USART_CR1_RXNEIE;
    USART2->CR1 |= USART_CR1_UE;

    /* RTOS-safe priority (must be >= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) */
    NVIC_SetPriority(USART2_IRQn, 7);
    NVIC_EnableIRQ(USART2_IRQn);
}

void uart_init(uint32_t baud)
{
    uart_rx.head = 0;
    uart_rx.tail = 0;
    uart_rx.overflow = 0;

    uart_gpio_init();
    uart_core_init(baud);
}

/* ---------------- TX ---------------- */

void uart_send_byte(uint8_t byte)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = byte;
}

void uart_send_str(const char *s)
{
    while (*s)
    {
        if (*s == '\n')
            uart_send_byte('\r');
        uart_send_byte(*s++);
    }
}

/* ---------------- RX ---------------- */

int uart_rx_available(void)
{
    return (uart_rx.head != uart_rx.tail);
}

/* Blocking read (busy wait – use carefully) */
char uart_read(void)
{
    while (uart_rx.head == uart_rx.tail);

    char byte = uart_rx.buf[uart_rx.tail];
    uart_rx.tail = (uart_rx.tail + 1U) % UART2_RX_BUF_SIZE;
    return byte;
}

/* Non-blocking read: returns -1 if empty */
char uart_read_nonblock(void)
{
    if (USART2->SR & USART_SR_RXNE)
        return (char)(USART2->DR);

    return -1;   // no data
}

uint8_t uart_get_overflow_flag(void)
{
    return uart_rx.overflow;
}

void uart_clear_overflow_flag(void)
{
    uart_rx.overflow = 0;
}

/* ---------------- ISR ---------------- */

void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_RXNE)
    {
        uint8_t byte = USART2->DR;
        uint16_t next = (uart_rx.head + 1U) % UART2_RX_BUF_SIZE;

        if (next == uart_rx.tail)
        {
            uart_rx.overflow = 1U;   /* Buffer full, drop byte */
        }
        else
        {
            uart_rx.buf[uart_rx.head] = byte;
            uart_rx.head = next;
        }
    }
}
