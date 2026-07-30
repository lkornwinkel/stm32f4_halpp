#include <hal/clock.hpp>

#include <cstdint>

#include "stm32f446xx.h"

namespace hal {
    Clock::Clock() {
        error_ = Error::OK;
    }
    void Clock::init() {

        set_160mhz_hse8();
    }

    void Clock::set_flash_latency(uint32_t sysclk_hz) {
        uint32_t latency = FLASH_ACR_LATENCY_0WS;
        if (sysclk_hz >  30000000u) latency = FLASH_ACR_LATENCY_1WS;
        if (sysclk_hz >  60000000u) latency = FLASH_ACR_LATENCY_2WS;
        if (sysclk_hz >  90000000u) latency = FLASH_ACR_LATENCY_3WS;
        if (sysclk_hz > 120000000u) latency = FLASH_ACR_LATENCY_4WS;
        if (sysclk_hz > 150000000u) latency = FLASH_ACR_LATENCY_5WS;

        FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | latency;
        FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;
    }

    void Clock::enable_hse_bypass_8mhz() {
        // HSE aus, damit wir BYPASS sauber setzen können
        RCC->CR &= ~RCC_CR_HSEON;
        while (RCC->CR & RCC_CR_HSERDY) {}

        // Nucleo: HSE kommt meist als externes Clock-Signal -> BYPASS
        RCC->CR |= RCC_CR_HSEBYP;
        RCC->CR |= RCC_CR_HSEON;
        while ((RCC->CR & RCC_CR_HSERDY) == 0) {}
    }

    void Clock::clear_pll_config() {
        RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk |
                  RCC_PLLCFGR_PLLN_Msk |
                  RCC_PLLCFGR_PLLP_Msk);
    }

    void Clock::set_multipliers(uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq) {
        volatile uint32_t temp = RCC->PLLCFGR;
        temp |= (pllm & 0x3f) << RCC_PLLCFGR_PLLM_Pos;
        temp |= (plln & 0x1ff) << RCC_PLLCFGR_PLLN_Pos;
        temp |= (pllp & 0x3) << RCC_PLLCFGR_PLLP_Pos;
        temp |= (pllq & 0x7) << RCC_PLLCFGR_PLLQ_Pos;
        RCC->PLLCFGR = temp;

    }


    void Clock::switch_sysclk_to_pll_from_hse(uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq, uint32_t sysclk_hz) {
        // PWR + Voltage scaling (für hohe Frequenzen)
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS; // Scale 1

        set_flash_latency(sysclk_hz);

        enable_hse_bypass_8mhz();

        clear_pll_config();

        set_multipliers(pllm, plln, pllp, pllq);

        RCC->PLLCFGR |= 1 << RCC_PLLCFGR_PLLSRC_Pos;

        RCC->CR |= RCC_CR_PLLON;

        while (!(RCC->CR & RCC_CR_PLLRDY)) {}

        // APB1 max 45 MHz -> /4, APB2 max 90 MHz -> /2
        // APB1 = 160 / 4 = 40 MHz
        // APB2 = 160 / 2 = 80 MHz
        RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;

        // Switch SYSCLK to PLL
        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}

        SystemCoreClockUpdate();
    }

    void Clock::set_160mhz_hse8() {
        // HSE=8MHz: M=8, N=320, P=/2 (bits=0) => SYSCLK=160MHz
        switch_sysclk_to_pll_from_hse(
            8u,        // PLLM  HSE / 8
            360u,      // PLLN  * 360
            0u,        // PLLP bits: 00 => /2
            7u,        // PLLQ
            180000000u // SYSCLK
        );
    }

    void Clock::set_160mhz_hsi() {
        // HSI=16MHz: M=8, N=160, P=/2 => VCO=320MHz, SYSCLK=160MHz
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        (void)RCC->APB1ENR;
        PWR->CR |= PWR_CR_VOS; // Scale 1

        set_flash_latency(160000000u);

        clear_pll_config();
        // PLLSRC bit stays 0 -> HSI source
        set_multipliers(8u, 160u, 0u, 0u);

        RCC->CR |= RCC_CR_PLLON;
        while (!(RCC->CR & RCC_CR_PLLRDY)) {}

        // APB1 = 160/4 = 40 MHz, APB2 = 160/2 = 80 MHz
        RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;

        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}

        SystemCoreClockUpdate();
    }


}