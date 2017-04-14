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

        //********************
        //If the constructor is marked with inject
        template <typename C>
        struct has_inject_annotation {
            using yes[1] = char;
            using no[2] = char;

            template <typename C1>
            static yes& test( typename C1::Inject* );

            template <typename>
            static no& test( ... );

            static const bool value = ( sizeof( test<C>(0) ) == sizeof(yes) );
        };

        //********************
        //To judge whether the signature is a constructor
        template <typename T, typename signature>
        struct is_constructible_signature : public std::false_type {};

        template <typename C, typename... Ps>
        struct is_constructible_signature <C, C(Ps...)> : public std::true_type {};

        //********************
        //If the constructor is marked with inject, try to get it
        template <typename C>
        struct get_inject_annotation {
            using S = typename C::Inject;
            using A = signatureArgs<S>;

            static_assert( is_safe_signature<S>::value, "The needed Inject typedef is the form of C(Args...)" );
            static_assert( std::is_same<C, signatureType<S>>::value, "Something wrong when inheriting from base class" );
            static_assert( is_constructible_from_parm<C, flat_assisted_parm<A>>::value, "C cannot be constructed with specific type" );

            static constexpr bool valid = true
                                       && is_safe_signature<S>::value
                                       && std::is_same<C, signatureType<S>>::value
                                       && is_constructible_from_parm<C, flat_assisted_parm<A>>::value;

            using Signature = typename std::enable_if<valid, S>::type;
            using Args = typename std::enable_if<valid, A>::type;
        };

        //********************
        //
        template <typename Sigset>
        struct sig_set_requirements_impl {};

        template <typename Sigset>
        struct sig_set_requirements = typename sig_set_requirements_impl<Sigset>::type;

        //********************
        //remove requirement(s) from sigset
        template <typename C, typename Sigset>
        using remove_requirement_from_sigset = construct_signature<signatureType<Sigset>, remove_parm<C, signatureArgs<Sigset>>>; 



















    }
}

#endif //DIFRAME_BASIC_CHECK_FOR_MODULE_UTILS
