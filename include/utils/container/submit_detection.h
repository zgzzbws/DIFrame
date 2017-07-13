#ifndef DIFRAME_SUBMIT_DETECTION_H
#define DIFRAME_SUBMIT_DETECTION_H

#include <type_traits>

namespace DIFrame {
namespace utils{

    //using Substitution Failure Is Not An Error (SFINAE) tech to detect
    //whether the T is annotated with 'Submit'
    //'hasSubmitTypedef' is used later to register the class T
    //use this : hasSubmitTypedef<XXX>::value 
    template <typename... Ts> 
    using void_t = void;

    template <typename T, typename = void>
    struct hasSubmitTypedef : std::false_type {};

    template <typename T>
    struct hasSubmitTypedef<T, void_t<typename T::Submit>> : std::true_type {};

    //Fetch and deal with the Submit annotation
    template <typename T>
    struct getSubmitTypedef {
        using Submit = typename T::Submit;
        using Para = SignatureArgs<Submit>;
        //check

        static constexpr bool ok = true && IsValidSignature<Submit>::value
                                        && std::is_same<T, SignatureType<Submit>>::value
                                        && is_constructible_with_list<C, UnlabelAssisted<A>>::value;
   
        using Signature = typename std::enable_if<ok, Submit>::type;
        using Args = typename std::enable_if<ok, Para>::type;
    }



}
}

#endif // DIFRAME_SUBMIT_DETECTION_H