#include "stm32f1xx.h"

void SystemClock_Config(void)
{
    // 1. Bật HSE
    RCC->CR |= RCC_CR_HSEON;

    // Chờ HSE ổn định
    while (!(RCC->CR & RCC_CR_HSERDY));

    // 2. Flash: 2 wait states cho 72 MHz
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    // 3. AHB = SYSCLK
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    // 4. APB1 = HCLK / 2
    // APB1 tối đa 36 MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    // 5. APB2 = HCLK
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    // 6. PLL = HSE x 9
    // 8 MHz x 9 = 72 MHz
    RCC->CFGR |= RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLMULL9;

    // 7. Bật PLL
    RCC->CR |= RCC_CR_PLLON;

    // Chờ PLL ổn định
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // 8. Chọn PLL làm SYSCLK
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Chờ PLL trở thành SYSCLK
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // 9. Cập nhật SystemCoreClock
    SystemCoreClock = 72000000;
}

void delay_ms(uint32_t ms)
{
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;
    SysTick->VAL = 0;

    SysTick->CTRL =
        SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_ENABLE_Msk;

    for (uint32_t i = 0; i < ms; i++)
    {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }

    SysTick->CTRL = 0;
}

int main(void)
{
    // Clock = 72 MHz
    SystemClock_Config();

    // Bật clock GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // PC13 = Output Push-Pull, 2 MHz
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1;

    // LED ban đầu tắt
    GPIOC->ODR |= GPIO_ODR_ODR13;

    while (1)
    {
        // LED ON
        GPIOC->ODR &= ~GPIO_ODR_ODR13;
        delay_ms(1000);

        // LED OFF
        GPIOC->ODR |= GPIO_ODR_ODR13;
        delay_ms(1000);
    }
}
