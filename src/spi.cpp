#include <hal/cmsis.hpp>
#include <hal/spi.hpp>
#include "hal/error.hpp"
#include "hal/gpio.hpp"
#include "stm32f446xx.h"

namespace hal {
    Spi::Spi() : error_(Error::NotInitialized) {
        device_ = Device::Spi1;
        cr1_ = 0;
        cr2_ = 0;
        clock_ = Gpio{};
        mosi_ = Gpio{};
        miso_ = Gpio{};
    }

    Error Spi::setup(SpiConfig const &config) {
        Error err;
        device_ = config.device;
        cr1_ = config.cr1;
        cr2_ = config.cr2;

        err = enable_clock();

        if (err == Error::OK) {
            err = setup_gpios();
        }

        if (err == Error::OK) {
            err = setup_spi();
        }

        return err;
    }

    Error Spi::setup_spi() {
        Error err = Error::OK;

        SPI_TypeDef *spix = object();

        if (spix == nullptr) {
            err = Error::NotSupported;
        }

        if (err == Error::OK) {
            spix->CR1 = cr1_;
            spix->CR2 = cr2_;
            spix->CR1 |= SPI_CR1_SPE;
        }

        return err;
    }

    Error Spi::setup_gpios() {
        Error err = Error::OK;
        GpioConfig mosi, miso, clock;

        switch (device_) {
            case Spi::Device::Spi1:
                err = Error::NotSupported;
                break;

            case Spi::Device::Spi2:

                mosi = GpioConfig{
                        .port = Gpio::Port::C,
                        .pin = 1,
                        .mode = Gpio::Mode::Alternate,
                        .pull = Gpio::PullMode::None,
                        .speed = Gpio::Speed::High,
                        .alternate_function = 7,

                };

                miso = GpioConfig{
                        .port = Gpio::Port::C,
                        .pin = 2,
                        .mode = Gpio::Mode::Alternate,
                        .pull = Gpio::PullMode::None,
                        .speed = Gpio::Speed::High,
                        .alternate_function = 5,

                };


                clock = GpioConfig{
                        .port = Gpio::Port::B,
                        .pin = 10,
                        .mode = Gpio::Mode::Alternate,
                        .pull = Gpio::PullMode::None,
                        .speed = Gpio::Speed::High,
                        .alternate_function = 5,

                };

                break;

            case Spi::Device::Spi3:
                err = Error::NotSupported;
                break;

            case Spi::Device::Spi4:
                err = Error::NotSupported;
                break;
        }

        if (err == Error::OK) {
            err = mosi_.setup(mosi);
        }
        if (err == Error::OK) {
            err = miso_.setup(miso);
        }
        if (err == Error::OK) {
            err = clock_.setup(clock);
        }

        return err;
    }

    SPI_TypeDef *Spi::object() {
        SPI_TypeDef *result = nullptr;
        switch (device_) {

            case Device::Spi1:
                result = SPI1;
                break;

            case Device::Spi2:
                result = SPI2;
                break;

            case Device::Spi3:
                result = SPI3;
                break;

            case Device::Spi4:
                result = SPI4;
                break;
        }

        return result;
    }

    SPI_TypeDef const *Spi::object() const {
        SPI_TypeDef *result = nullptr;

        switch (device_) {
            case Device::Spi1:
                result = SPI1;
                break;

            case Device::Spi2:
                result = SPI2;
                break;

            case Device::Spi3:
                result = SPI3;
                break;

            case Device::Spi4:
                result = SPI4;
                break;
        }

        return result;
    }

    Error Spi::enable_clock() {
        Error err = Error::OK;

        switch (device_) {
            case Device::Spi1:
                RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
                (void)RCC->APB2ENR;
                break;

            case Device::Spi2:
                RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
                (void)RCC->APB1ENR;
                break;

            case Device::Spi3:
                RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
                (void)RCC->APB1ENR;
                break;

            case Device::Spi4:
                RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
                (void)RCC->APB2ENR;
                break;

            default:
                err = Error::NotSupported;
                break;
        }

        return err;
    }

    void Spi::enable() { object()->CR1 |= SPI_CR1_SPE; }

    void Spi::disable() { object()->CR1 &= ~SPI_CR1_SPE; }

    void Spi::send(std::uint8_t data)
    {
        SPI_TypeDef *spix = object();
        if (spix == nullptr)
            return;

        while (!(spix->SR & SPI_SR_TXE)) {}
        *reinterpret_cast<volatile std::uint8_t *>(&spix->DR) = data;
        while (!(spix->SR & SPI_SR_RXNE)) {}
        (void)*reinterpret_cast<volatile std::uint8_t *>(&spix->DR);
        while (!(spix->SR & SPI_SR_TXE)) {}
        while (spix->SR & SPI_SR_BSY) {}
        (void)spix->SR;
    }

    void Spi::send(uint8_t const* data, uint16_t length)
    {
        SPI_TypeDef *spix = object();
        if (spix == nullptr)
            return;


        for (uint16_t i = 0; i < length; i++)
        {
            while ((spix->SR & SPI_SR_TXE) == 0) {}
            *reinterpret_cast<volatile std::uint8_t *>(&spix->DR) = data[i];
            while ((spix->SR & SPI_SR_RXNE) == 0) {}
            (void)*reinterpret_cast<volatile std::uint8_t *>(&spix->DR);
        }

        while ((spix->SR & SPI_SR_TXE) == 0) {}
        while (spix->SR & SPI_SR_BSY) {}
        (void)spix->SR;

    }
} // namespace hal
