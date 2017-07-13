#ifndef DIFRAME_BASIC_UTILS
#define DIFRAME_BASIC_UTILS

namespace DIFrame{
namespace utils{

    template <typename... Types>
    struct Parm {};

    template <typename Type>
    struct is_parm : std::false_type {};

    template <typename... Types>
    struct is_parm <Parm<Types...>> : std::true_type {};

}
}

#endif