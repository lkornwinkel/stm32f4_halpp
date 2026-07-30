#include <hal/gpio.hpp>

#include <stm32f4xx.h>

#include <cstdint>

namespace hal {
    Gpio::Gpio() : error_(Error::NotInitialized)
    {
        port_ = Port::A;
        pin_ = 0;
    }

    void Gpio::set() { object()->BSRR = (uint32_t{1} << pin_); }

    void Gpio::reset() { object()->BSRR = (uint32_t{1} << (pin_ + 16)); }

    bool Gpio::read() const { return object()->ODR & (uint32_t{1} << pin_); }

    void Gpio::toggle() { object()->ODR ^= (uint32_t{1} << pin_); }

    void Gpio::set_mode(Mode mode) {
        uint32_t shift = pin_ * 2;
        uint32_t mask = 0b11 << shift;
        uint32_t value = static_cast<uint32_t>(mode) << shift;
        std::uint32_t tmp = object()->MODER;
        tmp &= ~mask;
        tmp |= value;
        object()->MODER = tmp;
    }

    void Gpio::set_speed(Speed speed) {
        uint32_t shift = pin_ * 2;
        uint32_t mask = 0b11 << shift;
        uint32_t value = static_cast<uint32_t>(speed) << shift;
        std::uint32_t tmp = object()->OSPEEDR;
        tmp &= ~mask;
        tmp |= value;
        object()->OSPEEDR = tmp;
    }

    void Gpio::set_pull(PullMode pull) {
        uint32_t shift = pin_ * 2;
        uint32_t mask = 0b11 << shift;
        uint32_t value = static_cast<uint32_t>(pull) << shift;
        std::uint32_t tmp = object()->PUPDR;
        tmp &= ~mask;
        tmp |= value;
        object()->PUPDR = tmp;
    }

    void Gpio::set_output_type(OutputType type) {
        uint32_t mask = 0b1 << pin_;
        uint32_t value = static_cast<uint32_t>(type) << pin_;
        std::uint32_t tmp = object()->OTYPER;
        tmp &= ~mask;
        tmp |= value;
        object()->OTYPER = tmp;
    }

    void Gpio::set_alternate_function(uint8_t af) {
        uint32_t shift = (pin_ % 8) * 4;
        uint32_t mask = 0b1111 << shift;
        uint32_t value = static_cast<uint32_t>(af) << shift;
        object()->AFR[pin_ / 8] &= ~mask;
        object()->AFR[pin_ / 8] |= value;
    }

    void Gpio::enable_clock() {
        switch (port_) {
            case Port::A:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
                break;
            case Port::B:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
                break;
            case Port::C:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
                break;
            case Port::D:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
                break;
            case Port::E:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
                break;
            case Port::F:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
                break;
            case Port::G:
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
                break;
        }
    }


    void Gpio::disable_clock() {
        switch (port_) {
            case Port::A:
                RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
                break;
            case Port::B:
                RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOBEN;
                break;
            case Port::C:
                RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
                break;
            case Port::D:
                RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIODEN;
                break;
            case Port::E:
                RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOEEN;
                break;
            case Port::F:
                RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOFEN;
                break;
            case Port::G:
                RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOGEN;
                break;
        }
    }

    GPIO_TypeDef *Gpio::object() {
        GPIO_TypeDef *result = nullptr;

        switch (port_) {
            case Port::A:
                result = GPIOA;
                break;
            case Port::B:
                result = GPIOB;
                break;
            case Port::C:
                result = GPIOC;
                break;
            case Port::D:
                result = GPIOD;
                break;
            case Port::E:
                result = GPIOE;
                break;
            case Port::F:
                result = GPIOF;
                break;
            case Port::G:
                result = GPIOG;
                break;
        }

        return result;
    }
    GPIO_TypeDef const *Gpio::object() const {
        GPIO_TypeDef *result = nullptr;

        switch (port_) {
            case Port::A:
                result = GPIOA;
                break;
            case Port::B:
                result = GPIOB;
                break;
            case Port::C:
                result = GPIOC;
                break;
            case Port::D:
                result = GPIOD;
                break;
            case Port::E:
                result = GPIOE;
                break;
            case Port::F:
                result = GPIOF;
                break;
            case Port::G:
                result = GPIOG;
                break;
        }

        return result;
    }

    Error Gpio::setup(GpioConfig const &config) {
        Error err = Error::OK;

        pin_ = config.pin;
        port_ = config.port;

        enable_clock();

        if (config.mode == Gpio::Mode::Output)
        {
            reset();
        }

        if (config.mode == Mode::Alternate) {
            set_alternate_function(config.alternate_function);
        }
        set_mode(config.mode);
        set_speed(config.speed);
        set_pull(config.pull);
        set_output_type(config.output_type);

        return err;
    }
} // namespace hal
