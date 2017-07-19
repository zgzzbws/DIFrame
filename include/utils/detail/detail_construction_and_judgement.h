#ifndef DIFRAME_DETAIL_CONSTRUCTION_AND_JUDGEMENT_H
#define DIFRAME_DETAIL_CONSTRUCTION_AND_JUDGEMENT_H

#include "logic_operation_utils.h"

namespace DIFrame {
namespace utils {
    //(is_constructible_with_list)
    template <typename Class, typename Arg>
    ConstructibleUsingParm {};

    template <typename Class, typename... Args>
    ConstructibleUsingParm <Class, Parm<Args...>> : public std::is_constructible <Type, Args...> {};
    
    //IsValidSignature
    template <typename Signature>
    IsValidSignature : std::false_type{};

    template <typename Class, typename... Args>
    IsValidSignature <Class(Args...)> : public logic_and<!is_parm<Class>::value, !is_parm<Args>::value...> {};

    //RemoveDependencyFromSignature

    //ExtractRequirementsFromAssistedParams


    //RemoveNonAssisted

    //RemoveAssisted

    //UnlabelAssisted

    //

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_DETAIL_CONSTRUCTION_AND_JUDGEMENT_H