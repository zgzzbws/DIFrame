#ifndef DIFRAME_SUBMIT_DETECTION_H
#define DIFRAME_SUBMIT_DETECTION_H

#include <type_traits>

namespace DIFrame {
namespace utils{

    //If the parameter conform to the general case, give a error message.
    template <typename Type>
    FetchSignature {
        static_assert( false && sizeof(Type) , 
                       "Error Message : The input is not a valid signature" );
    };

    template <typename Type, typename... Args>
    FetchSignature <Type(Args...)> {
        using T = Type;
        using A = Parm<Args...>;
    };

    template <typename Signature>
    using SignatureType = typename FetchSignature<Signature>::T;

    template <typename Signature>
    using SignatureArgs = typename FetchSignature<Signature>::A;

    //using Substitution Failure Is Not An Error (SFINAE) tech to detect
    //whether the T is annotated with 'Submit'
    //'hasSubmitTypedef' is used later to register the class T
    //using like this : hasSubmitTypedef<Class>::value 
    template <typename... Ts> 
    using void_t = void;

    template <typename T, typename = void>
    struct HasSubmitTypedef : std::false_type {};

    template <typename T>
    struct HasSubmitTypedef<T, void_t<typename T::Submit>> : std::true_type {};

    //Fetch and deal with the Submit annotation
    template <typename T>
    struct GetSubmitTypedef {
        using Submit = typename T::Submit;
        using Para = SignatureArgs<Submit>;
        //check

        static constexpr bool judge = true && IsValidSignature<Submit>::value
                                           && std::is_same<T, SignatureType<Submit>>::value
                                           && ConstructibleUsingParm<T, UnlabelAssisted<Para>>::value;
   
        using Signature = typename std::enable_if<judge, Submit>::type;
        using Args = typename std::enable_if<judge, Para>::type;
    };

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_SUBMIT_DETECTION_H