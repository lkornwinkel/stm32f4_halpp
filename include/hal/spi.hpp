#ifndef STM32F4_HAL_SPI_H
#define STM32F4_HAL_SPI_H
#include <cstdint>

#include <hal/error.hpp>
#include <hal/gpio.hpp>

namespace hal {

    struct SpiConfig;

    class Spi {
    public:
        enum class Device { Spi1, Spi2, Spi3, Spi4 };

    protected:
        Error error_;
        Device device_;
        uint16_t cr1_, cr2_;
        Gpio mosi_, miso_, clock_;

    public:
        Spi();

        [[nodiscard]] Error setup(SpiConfig const &config);

        [[nodiscard]] SPI_TypeDef *object();
        [[nodiscard]] SPI_TypeDef const *object() const;

        void enable();
        void disable();

        void send(std::uint8_t data);
        void send(uint8_t const *data, uint16_t length);

    protected:
        Error enable_clock();
        Error setup_gpios();
        Error setup_spi();
    };

    struct SpiConfig {
        Spi::Device device;
        uint16_t cr1, cr2;
    };

} // namespace hal
#endif // STM32F4_HAL_SPI_H
