#ifndef STM32F4_HAL_CLOCK_HPP
#define STM32F4_HAL_CLOCK_HPP

#include <hal/error.hpp>

#include <cstdint>

namespace hal {
    class Clock {
    public:
        enum class Frequency { HSE_8MHz, HSE_180MHz };
        Clock();
        void init();

    protected:
        void set_flash_latency(std::uint32_t sysclk_hz);
        void enable_hse_bypass_8mhz();
        void switch_sysclk_to_pll_from_hse(std::uint32_t pllm, std::uint32_t plln, std::uint32_t pllp,
                                           std::uint32_t pllq, std::uint32_t sysclk_hz);
        void set_160mhz_hse8();
        void set_160mhz_hsi();
        void clear_pll_config();
        void set_multipliers(std::uint32_t pllm, std::uint32_t plln, std::uint32_t pllp, uint32_t pllq);

        [[nodiscard]] Error get_error() const { return error_; }

    private:
        Error error_;
    };
} // namespace hal
#endif // STM32F4_HAL_CLOCK_HPP
