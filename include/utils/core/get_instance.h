#ifndef DIFRAME_GET_INSTANCE_H
#define DIFRAME_GET_INSTANCE_H

namespace DIFrame{
namespace utils{

    template <typename T>
    struct GetInstance {
        T operator()( Build& build ) {
            return *( build.getInstanceHelper<T>() );
        }
    };

    template <typename T>
    struct GetInstance <const T> {
        const T operator()( Build& build ) {
            return *( build.getInstanceHelper<T>() );
        }
    };

    template <typename T>
    struct GetInstance <T&> {
        T& operator()( Build& build ) {
            return *( build.getInstanceHelper<T>() );
        }
    };

    template <typename T>
    struct GetInstance <const T&> {
        const T& operator()( Build& build ) {
            return *( build.getInstanceHelper<T>() );
        }
    };

    template <typename T>
    struct GetInstance <T*> {
        T* operator()( Build& build ) {
            return build.getInstanceHelper<T>();
        }
    };

    template <typename T>
    struct GetInstance <const T*> {
        const T* operator()( Build& build ) {
            return build.getInstanceHelper<T>();
        }
    };

    template <typename T>
    struct GetInstance <std::shared_ptr<T>> {
        std::shared_ptr<T> operator()( Build& build ) {
            return std::shared_ptr<T>(std::shared_ptr<char>(), build.getInstanceHelper<T>());
        }
    };

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_GET_INSTANCE_H