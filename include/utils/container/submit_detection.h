#ifndef DIFRAME_SUBMIT_DETECTION_H
#define DIFRAME_SUBMIT_DETECTION_H

#include <type_traits>

namespace DIFrame {
namespace utils{

    //using Substitution failure is not an error (SFINAE) tech to detect
    //whether the T is annotated with 'Submit'
    //'hasSubmitTypedef' is used later to register the class T
    template <typename... Ts> 
    using void_t = void;

    template <typename T, typename = void>
    struct hasSubmitTypedef : std::false_type {};

    template <typename T>
    struct hasSubmitTypedef<T, void_t<typename T::Submit>> : std::true_type {};


}
}

#endif // DIFRAME_SUBMIT_DETECTION_H