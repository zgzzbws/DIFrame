#ifndef DIFRAME_DETAIL_TYPE_IDENTIFICATION_H
#define DIFRAME_DETAIL_TYPE_IDENTIFICATION_H

#include <memory>

namespace DIFrame {
namespace utils{

    template <typename T>
    struct getTypeHelper {
        using type = T;
    };

    template <typename T>
    struct getTypeHelper <const T> {
        using type = T;
    };

    template <typename T>
    struct getTypeHelper <T&> {
        using type = T;
    };

    template <typename T>
    struct getTypeHelper <const T&> {
        using type = T;
    };

    template <typename T>
    struct getTypeHelper <T*> {
        using type = T;
    };

    template <typename T>
    struct getTypeHelper <const T*> {
        using type = T;
    };

    template <typename T>
    struct getTypeHelper <std::shared_ptr<T>> {
        using type = T;
    };

    template <typename T>
    using getType = typename getTypeHelper<T>::type;

    template <typename T>
    struct NopDeleter {
        void operator()(T*){
        }
    };

    template <typename F, typename... Args>
    using resultOf = typename std::remove_reference<decltype(F()(std::declval<Args>()...))>::type;
                     
} // namespace utils
} //namespace DIFrame

#endif // DIFRAME_DETAIL_TYPE_IDENTIFICATION_H