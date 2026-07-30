#ifndef STM32F4_HAL_UART_HPP
#define STM32F4_HAL_UART_HPP

#include <hal/gpio.hpp>


namespace hal {
    struct UartConfig;

    class Uart {
    public:
        enum class BaudRate { Baud9600 = 9600, Baud115200 = 115200 };
        enum class Device { U1, U2, U3 };

        [[nodiscard]]
        Error setup(const UartConfig &config);

        [[nodiscard]]
        USART_TypeDef *object(Device device);

        [[nodiscard]]
        USART_TypeDef const *object(Device device) const;

        void enable();
        void disable();
        void enable_transmitter();
        void disable_transmitter();
        void enable_receiver();
        void disable_receiver();

        [[nodiscard]]
        Error wait_for_transmit_complete(uint32_t tries);
        [[nodiscard]]
        Error wait_for_receive_complete(uint32_t tries);

        [[nodiscard]]
        Error transmit_byte(uint8_t byte);
        [[nodiscard]]
        Error receive_byte(uint8_t &byte);
        [[nodiscard]]
        Error send_polling(std::uint8_t const *data, std::size_t size);

    protected:
        Error init_gpio();
        void init_uart();
        void enable_clock();


    protected:
        Device device_;
        BaudRate baud_rate_;
        Gpio tx_pin_;
        Gpio rx_pin_;
    };

    struct UartConfig {
        Uart::Device device;
        Uart::BaudRate baud_rate;
    };

} // namespace hal

#endif // STM32F4_HAL_UART_HPP
