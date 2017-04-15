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

        template <typename C, typename Sigsets>
        struct remove_requirement_from_sigsets_impl {
            static_assert( false && sizeof(C*), "" );
        };

        template <typename C, typename... Sigsets>
        struct remove_requirement_from_sigsets_impl <C, parm<Sigsets...>> {
            using type = parm<remove_requirement_from_sigset<C, Sigsets>...>;
        };

        template <typename C, typename Sigsets>
        using remove_requirement_from_sigsets = typename remove_requirement_from_sigsets_impl<C, Sigsets>::type; 

        //********************
        //To construct the sigsets
        template <typename P, typename Rs>
        using construct_sigset = construct_signature<P*, remove_duplicate_element<add_pointer<Rs>>>;

        template <typename Rs, typename... P>
        using construct_sigsets = parm<construct_sigset<P, Rs>...>;

        //********************
        //In the given sigset, replace all the requirements for R with Rs
        template <typename Sigset, typename R, typename Rs>
        using replace_requirement_in_sigset = signatureType<Sigset>(replace_parm<R, Rs, signatureArgs<Sigset>>);

        //********************
        //To check the self loop
        template <typename Sigset>
        struct self_loop_check : public parm_in_list<signatureType<Sigset>, signatureArgs<Sigset>> {};

        //*********************
        //Now we want to replace the current sigset with another sigset in a constructor
        template <typename cS, typename nS>
        using replace_current_sigset = construct_signature<signatureType<cS>, replace_parm<signatureType<nS>, signatureArgs<nS>, signatureArgs<cS>>>;

        template <typename cSs, typename nS>
        struct replace_current_sigsets_with_ns {};

        template <typename... cSc, typename nS>
        struct replace_current_sigsets_with_ns <parm<cSc...>, nS> {
            using type = parm<replace_current_sigset<cSc, nS>...>;
        };

        template <typename cS, typename nSs>
        struct replace_current_sigset_with_nss {};

        template <typename cS>
        struct replace_current_sigset_with_nss <cS, parm<>> {
            using type = cS;
        };

        template <typename cS, typename S1, typename... nS> 
        struct replace_current_sigset_with_nss <cS, parm<S1, nS...>> {
            using temp_result = typename replace_current_sigset_with_nss<cS, parm<nS...>>::type;
            using type = replace_current_sigset<temp_result, S1>;
        };

        //********************
        //
        template <typename sigset, typename sigsets>
        struct add_to_sigset_impl {
            using replaced_sig = typename replace_current_sigset_with_nss<sigset, sigsets>::type;
            //check assert in here
            using type = add_to_parm<replaced_sig, typename replace_current_sigsets_with_ns<sigsets, replaced_sig>::type>;
        };
  
        template <typename sigset, typename sigsets>
        using add_to_sigset = typename add_to_sigset_impl<sigset, sigsets>::type;

        template <typename sigset, typename otherSigsets>
        struct add_to_sigsets_impl {};

        template <typename... otherSigsets>
        struct add_to_sigsets_impl <parm<>, parm<otherSigsets...>> {
           using type = parm<otherSigsets...>;
        };

        template <typename sigset, typename... sig1, typename... otherSigsets>
        struct add_to_sigsets_impl <parm<sigset, sig1...>, parm<otherSigsets...>> {
            using temp_result = typename add_to_sigsets_impl<parm<sig1...>, parm<otherSigsets...>>::type;
            using type = add_to_sigset<sigset, temp_result>;
        };

        template <typename sigset, typename otherSigsets>
        using add_to_sigsets = typename add_to_sigsets_impl<sigset, otherSigsets>::type;

        //********************
        //check the sigsets
        typename <typename s, typename sigsets>
        struct check_sigsets {
            static_assert( false && sizeof(s), "Bug" );
        };

        typename <typename s>
        struct check_sigsets<s, parm<>> {
            static_assert( false && sizeof(s), "" );
        };

        typename <typename sType, typename... sArgs, typename s1Type, typename... s1Args, typename... sigsets>
        struct check_sigsets<sType(sArgs...), parm<s1Type(s1Args...), sigsets...>> : public check_sigsets<sType(sArgs...), parm<sigsets...>> {};

        typename <typename sType, typename... sArgs, typename... s1Args, typename... sigsets>
        struct check_sigsets<sType(sArgs), parm<sType(s1Args), sigsets...>> {
            static_assert( empty_parm_list<figure_differences<parm<sArgs>, parm<s1Args>>>::value, "error" );
        };

        //********************
        //check the subset of sigsets
        template <typename subset, typename sigset>
        struct check_subset {
            static_assert( empty_parm_list<subset>::value, "" );
        };

        template <typename s1, typename... s, typename sigset>
        struct check_subset <parm<s1, s...>, sigset> : check_subset<parm<s...>, sigset> {
            //check assert in here
        };

        //*******************
        //check module
        template <typename module, typename neceModule>
        struct check_module {
            //check assert in here
        };

        //******************
        //expend injector in parm
        
    } // namespace utils
} //namespace DIFrame

#endif //DIFRAME_BASIC_CHECK_FOR_MODULE_UTILS
