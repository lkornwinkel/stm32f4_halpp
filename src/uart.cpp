#include <hal/gpio.hpp>
#include <hal/uart.hpp>

namespace hal {
    Error Uart::setup(UartConfig const &config) {
        Error err = Error::OK;

        device_ = config.device;
        baud_rate_ = config.baud_rate;

        init_gpio();
        init_uart();

        return err;
    }


    Error Uart::send_polling(std::uint8_t const *data, std::size_t size) {
        Error err;

        enable();
        enable_transmitter();
        for (std::size_t i = 0; i < size; ++i) {
            err = wait_for_transmit_complete(10000);
            if (err != Error::OK)
            {
                break;
            }
            USART1->DR = data[i];
        }

        disable_transmitter();
        disable();

        return err;
    }

    USART_TypeDef *Uart::object(Device device) {
        switch (device) {
            case Device::U1:
                return USART1;
            case Device::U2:
                return USART2;
            case Device::U3:
                return USART3;
            default:
                return nullptr; // Handle invalid device case
        }
    }

    USART_TypeDef const *Uart::object(Device device) const {
        switch (device) {
            case Device::U1:
                return USART1;
            case Device::U2:
                return USART2;
            case Device::U3:
                return USART3;
            default:
                return nullptr; // Handle invalid device case
        }
    }

    Error Uart::init_gpio() {
        Error err;
        GpioConfig tx_config;
        GpioConfig rx_config;

        if (device_ == Device::U1) {
            tx_config = {
                    .port = Gpio::Port::A,
                    .pin = 9,
                    .mode = Gpio::Mode::Alternate,
                    .alternate_function = 7,
            };
            rx_config = {
                    .port = Gpio::Port::A,
                    .pin = 10,
                    .mode = Gpio::Mode::Alternate,
                    .alternate_function = 7,
            };
        }
        if (device_ == Device::U2) {
            tx_config = {
                    .port = Gpio::Port::A,
                    .pin = 2,
                    .mode = Gpio::Mode::Alternate,
                    .alternate_function = 7,
            };
            rx_config = {
                    .port = Gpio::Port::A,
                    .pin = 3,
                    .mode = Gpio::Mode::Alternate,
                    .alternate_function = 7,
            };
        }

        if (device_ == Device::U3) {
            tx_config = {
                    .port = Gpio::Port::B,
                    .pin = 10,
                    .mode = Gpio::Mode::Alternate,
                    .alternate_function = 7,
            };
            rx_config = {
                    .port = Gpio::Port::C,
                    .pin = 5,
                    .mode = Gpio::Mode::Alternate,
                    .alternate_function = 7,
            };
        }
        err = tx_pin_.setup(tx_config);
        if (err == Error::OK)
        {
            err = rx_pin_.setup(rx_config);
        }

        return err;
    }

    void Uart::init_uart() {
        enable_clock();
        volatile USART_TypeDef *uart = object(device_);
        auto rate = static_cast<uint32_t>(baud_rate_);
        auto uartdiv = SystemCoreClock / (2 * 8 * rate);
        auto remainder = (SystemCoreClock - uartdiv * (2 * 8 * rate)) / (rate);
        auto brr = (uartdiv << 4) | (remainder & 0x0F);

        uart->BRR = brr; // Set baud rate register
    }

    void Uart::enable_clock() {
        switch (device_) {
            case Device::U1:
                RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            break;
            case Device::U2:
                RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
                break;
            case Device::U3:
                RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
                break;
        }
    }

    void Uart::enable() {
        USART_TypeDef *uart = object(device_);
        uart->CR1 |= USART_CR1_UE; // Enable USART
    }

    void Uart::disable() {
        USART_TypeDef *uart = object(device_);
        uart->CR1 &= ~USART_CR1_UE; // Disable USART
    }

    void Uart::enable_transmitter() {
        USART_TypeDef *uart = object(device_);
        uart->CR1 |= USART_CR1_TE; // Enable transmitter
    }

    void Uart::disable_transmitter() {
        USART_TypeDef *uart = object(device_);
        uart->CR1 &= ~USART_CR1_TE; // Disable transmitter
    }

    void Uart::enable_receiver() {
        USART_TypeDef *uart = object(device_);
        uart->CR1 |= USART_CR1_RE; // Enable receiver
    }

    void Uart::disable_receiver() {
        USART_TypeDef *uart = object(device_);
        uart->CR1 &= ~USART_CR1_RE; // Disable receiver
    }

    Error Uart::wait_for_transmit_complete(uint32_t tries)
    {
        while (tries-- > 0)
        {
            if ((USART1->SR & USART_SR_TXE)  == USART_SR_TXE)
            {
                return Error::OK;
            }
        }

        return Error::Timeout;
    }
} // namespace hal
