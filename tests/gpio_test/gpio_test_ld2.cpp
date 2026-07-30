#include "hal/clock.hpp"
#include "hal/gpio.hpp"

int main() {
    hal::Clock clock{};
    clock.init();

    hal::GpioConfig config{
            .port = hal::Gpio::Port::A,
            .pin = 5,
            .mode = hal::Gpio::Mode::Output,
            .pull = hal::Gpio::PullMode::None,
            .speed = hal::Gpio::Speed::Low,
            .output_type = hal::Gpio::OutputType::PushPull,
            .alternate_function = 0,
    };

    hal::Gpio ld2{};
    auto error = ld2.setup(config);

    for (volatile int i = 0; i < 100000; i++)
        ;


    while (1) {
        for (volatile int i = 0; i < 100000; i++)
            ;
        ld2.toggle();
    }
}
