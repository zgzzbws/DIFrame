#ifndef DIFRAME_CONTAINER_H
#define DIFRAME_CONTAINER_H

#include "forward_declaration.h"
#include "utils/container_impl.h"

namespace DIFrame {

    //using Dep = Dependency;
    //using Cons = Constructor;
    //using Sig = Signature;
    template <typename Dependency, typename Constructor, typename Signature>
    class ContainerImpl {
    public:
        using Dep  = Dependency;
        using Cons = Constructor;
        using Sig  = Signature;
        using This = ContainerImpl<Dep, Cons, Sig>;
    }

    template <typename... Types>
    class Container;

    template <typename... Types>
    class Container : public Container<Dependency<>, Types...> {
    private:
        Container() = default;
        friend Container<> createContainer();

    public:
        template <typename Cont>
        Container( Cont&& c ) : Container<Dependency<>, Types...>(std::forward<Cont>(c)) {}
    };

    template <typename... Dep, typename... Par>
    class Container<Dependency<Dep...>, Par...>
        : public aaaComponent<DIFrame::utils::Parm<Dep...>,
                              DIFrame::utils::Parm<Par...>,
                              DIFrame::utils::Composition<DIFrame::utils::Parm<Dep...>, Par...>,
                              DIFrame::utils::Parm<>> {
        
    };

} // namespace DIFrame

#endif // DIFRAME_CONTAINER