#ifndef STM32F4_HAL_ERROR_HPP
#define STM32F4_HAL_ERROR_HPP

namespace hal {
    enum class Error {
        OK,
        NullPointer,
        NotImplemented,
        NotSupported,
        InvalidParameter,
        Timeout,
        NotInitialized,
    };

}
#endif // STM32F4_HAL_ERROR_HPP
