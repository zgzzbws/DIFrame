#ifndef DIFRAME_BASIC_CHECK_FOR_MODULE_UTILS
#define DIFRAME_BASIC_CHECK_FOR_MODULE_UTILS

//module_utils.h

#include "a.h"
#include "b.h"

namespace DIFrame {
    namespace utils {
        
        //********************
        //To judge whether the signature is safe and valid 
        template <typename signature>
        struct is_safe_signature : std::false_type {};

        template <typename C, typename... Ps>
        struct is_safe_signature <C(Ps...)> : public logic_and<!has_multi_parameters<C>::value, !has_multi_parameters<Ps>::value...> {};

        //********************
        //To judge whether the signature set provided is safe and valid
        template <typename Sigset>
        struct is_safe_sig_set : public std::false_type {};

        template <typename... Sigsets>
        struct is_safe_sig_set <parm<Sigsets...>> : public logic_and<is_safe_signature<Sigsets>::value...> {};
    }
}

#endif //DIFRAME_BASIC_CHECK_FOR_MODULE_UTILS
