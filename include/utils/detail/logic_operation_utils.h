#ifndef DIFRAME_LOGIC_OPERATION_H
#define DIFRAME_LOGIC_OPERATION_H

namespace DIFrame {
namespace utils {
    
        //********************
        //This part is designed to achieve the operations including logic_and, login_or, logic_sum

        //********************
        //The achievement of logic_and
        template <bool... bs>
        struct logic_and {
            static constexpr bool value = true;
        };

        template <bool b, bool... bs>
        struct logic_and <b, bs...> {
            static constexpr bool value = b && logic_and<bs...>::value;
        };

        template <bool... bs>
        struct logic_or {
            static constexpr bool value = !logic_and<!bs...>::value;
        }; 

        template <int... is>
        struct logic_sum {
            static constexpr int value = 0;
        };

        template <int i, int... is>
        struct logic_sum <i, is...> {
            static constexpr int value = i + logic_sum<is...>::value;
        };

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_LOGIC_OPERATION_H
