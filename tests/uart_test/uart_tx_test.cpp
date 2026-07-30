#include <cstring>
#include <hal/uart.hpp>
#include <hal/error.hpp>
#include <hal/clock.hpp>

int main()
{
    hal::Clock clock{};
    clock.init();


    hal::GpioConfig ld2_config{
        .port = hal::Gpio::Port::A,
        .pin = 5,
        .mode = hal::Gpio::Mode::Output,
        .pull = hal::Gpio::PullMode::None,
        .speed = hal::Gpio::Speed::Low,
        .output_type = hal::Gpio::OutputType::PushPull,
        .alternate_function = 0,
    };

    hal::Gpio ld2{};
    ld2.setup(ld2_config);

    hal::Uart uart{};
    hal::UartConfig config{
        .device = hal::Uart::Device::U1,
        .baud_rate = hal::Uart::BaudRate::Baud9600
    };

    hal::Error err = uart.setup(config);

    char const *message = "Hello, world!\r\n";

    for (int i = 0; i < 6; i++)
    {
        ld2.toggle();
        for (volatile int i = 0; i < 100000; i++);

    }
    while (1)
    {
        err = uart.send_polling(reinterpret_cast<uint8_t const*>(message), strlen(message));
        if (err == hal::Error::OK)
        {
            err = uart.wait_for_transmit_complete(10000);
        }

        if (err == hal::Error::OK)
        {
            ld2.toggle();
        }

        for (volatile int i = 0; i < 100000; i++);
    }

}
