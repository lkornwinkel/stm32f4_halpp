#ifndef STM32F4_HALPP_GPIO_HPP
#define STM32F4_HALPP_GPIO_HPP

#include <hal/error.hpp>

#include <stm32f4xx.h>

#include <cstdint>

namespace hal {
    struct GpioConfig;

    class Gpio {
    public:
        enum class Port { A, B, C, D, E, F, G };
        enum class PullMode { None, PullUp, PullDown };
        enum class Mode { Input, Output, Alternate, Analog };
        enum class Speed { Low, Medium, Fast, High };
        enum class OutputType { PushPull, OpenDrain };

    public:
        Gpio();

        [[nodiscard]]
        Port port() const {
            return port_;
        }

        [[nodiscard]]
        uint8_t pin() const {
            return pin_;
        }

        void set();
        void reset();

        [[nodiscard]] bool read() const;
        void toggle();

        [[nodiscard]] Error setup(GpioConfig const &config);
        [[nodiscard]] Error error() const;

        void set_mode(Mode mode);
        void set_speed(Speed speed);
        void set_pull(PullMode pull);
        void set_output_type(OutputType output_type);
        void set_alternate_function(uint8_t alt_function);
        void enable_clock();
        void disable_clock();

    protected:
        [[nodiscard]]
        GPIO_TypeDef *object();

        [[nodiscard]]
        GPIO_TypeDef const *object() const;

    private:
        Error error_;
        Port port_;
        uint8_t pin_;
    };

    struct GpioConfig {
        Gpio::Port port;
        uint8_t pin;
        Gpio::Mode mode;
        Gpio::PullMode pull;
        Gpio::Speed speed;
        Gpio::OutputType output_type;
        uint8_t alternate_function;
    };
} // namespace hal


#endif // STM32F4_HALPP_GPIO_HPP
