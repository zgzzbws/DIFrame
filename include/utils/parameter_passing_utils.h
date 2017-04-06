#ifndef DIFRAME_PARAMETER_PASSING_H
#define DIFRAME_PARAMETER_PASSING_H

#include <functional>

namespace DIFrame {
    namespace utils {
        
        //The struct to pass the parameters
        template <typename... parameters>
        struct parm { };
        
        //To judge whether the parameter list has one or multi-parameters
        //If there is just one parameter in the parameter list
        template <typename T>
        struct has_multi_parameters : public std::false_type {};

        //If there is more than one parameter in the list
        template <typename... Ts>
        struct has_multi_parameters <parm<Ts...>> : public std::true_type {};

        

    }
}

#endif
