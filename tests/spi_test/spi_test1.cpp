#include <type_traits>
#include <hal/gpio.hpp>
#include <hal/clock.hpp>
#include <hal/spi.hpp>

auto nss_config = hal::GpioConfig{
        .port = hal::Gpio::Port::C,
        .pin = 8,
        .mode = hal::Gpio::Mode::Output,
        .pull = hal::Gpio::PullMode::None,
        .speed = hal::Gpio::Speed::Low,
        .output_type = hal::Gpio::OutputType::OpenDrain,
        .alternate_function = 0,
};

auto spi_config = hal::SpiConfig{
        .device = hal::Spi::Device::Spi2,
        .cr1 = (0b10 << SPI_CR1_BR_Pos) | (0b1 << SPI_CR1_MSTR_Pos),
        .cr2 = 0,
};

int main() {

    // Breadboard
    // 15: Clock  -> 0
    // 16: MISO   -> 2
    // 17: MOSI   -> 1
    // 18: NSS   -> 3

    hal::Error err;

    hal::Clock clock{};
    clock.init();

    hal::GpioConfig led_config{
        .port = hal::Gpio::Port::A,
        .pin = 5,
        .mode = hal::Gpio::Mode::Output,
        .pull = hal::Gpio::PullMode::None,
        .speed = hal::Gpio::Speed::Low,
        .output_type = hal::Gpio::OutputType::PushPull,
        .alternate_function = 0,
};


    auto nss_config = hal::GpioConfig{
        .port = hal::Gpio::Port::C,
        .pin = 8,
        .mode = hal::Gpio::Mode::Output,
        .pull = hal::Gpio::PullMode::PullUp,
        .speed = hal::Gpio::Speed::Low,
        .output_type = hal::Gpio::OutputType::PushPull,
        .alternate_function = 0,
};

    auto spi_config = hal::SpiConfig{
        .device = hal::Spi::Device::Spi2,
        .cr1 = (0b111 << SPI_CR1_BR_Pos) |
            (1 << SPI_CR1_MSTR_Pos) |
                (1 << SPI_CR1_SSM_Pos) |
                    (1 << SPI_CR1_SSI_Pos),
        .cr2 = 0,
};
/*
    hal::GpioConfig mosi_test{
        .port = hal::Gpio::Port::C,
        .pin = 1,
        .mode = hal::Gpio::Mode::Output,
        .pull = hal::Gpio::PullMode::None,
        .speed = hal::Gpio::Speed::High,
        .output_type = hal::Gpio::OutputType::PushPull,
        .alternate_function = 0,
    };
    hal::Gpio mosi_gpio;
    err =  mosi_gpio.setup(mosi_test);
*/
    hal::Gpio ld2{};
    err = ld2.setup(led_config);

    for (uint16_t i = 0; i < 3; i++)
    {
        ld2.toggle();
        for (volatile uint32_t j = 0; j < 100000; j++);
    }

    hal::Gpio nss;
    err = nss.setup(nss_config);
    if (err != hal::Error::OK)
    {
        while (1);
    }
    nss.set();

    hal::Spi spi;
    err = spi.setup(spi_config);
    if (err != hal::Error::OK)
    {
        while (1);
    }
    uint8_t data[] = "Hallo\r\n";

    while (1)
    {
        ld2.set();
        for (volatile uint32_t j = 0; j < 1000000; j++);
        nss.reset();
        spi.send(0x28);//data, sizeof(data));
        nss.set();
        ld2.reset();
        for (volatile uint32_t j = 0; j < 1000000; j++);
    }

}
