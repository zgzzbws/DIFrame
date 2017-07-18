#ifndef DIFRAME_CORE_GET_INSTANCE_H
#define DIFRAME_CORE_GET_INSTANCE_H

namespace DIFrame{
namespace utils{

    template <typename T>
    struct GetInstance {
        T operator()( Reflections& reflections ) {
            return *( reflections.getInstanceHelper<T>() );
        }
    };

    template <typename T>
    struct GetInstance <const T> {
        const T operator()( Reflections& reflections ) {
            return *( reflections.getInstanceHelper<T>() );
        }
    };

    template <typename T>
    struct GetInstance <T&> {
        T& operator()( Reflections& reflections ) {
            return *( reflections.getInstanceHelper<T>() );
        }
    };

    template <typename T>
    struct GetInstance <const T&> {
        const T& operator()( Reflections& reflections ) {
            return *( reflections.getInstanceHelper<T>() );
        }
    };

    template <typename T>
    struct GetInstance <T*> {
        T* operator()( Reflections& reflections ) {
            return reflections.getInstanceHelper<T>();
        }
    };

    template <typename T>
    struct GetInstance <const T*> {
        const T* operator()( Reflections& reflections ) {
            return reflections.getInstanceHelper<T>();
        }
    };

    template <typename T>
    struct GetInstance <std::shared_ptr<T>> {
        std::shared_ptr<T> operator()( Reflections& reflections ) {
            return std::shared_ptr<T>(std::shared_ptr<char>(), reflections.getInstanceHelper<T>());
        }
    };

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_CORE_GET_INSTANCE_H