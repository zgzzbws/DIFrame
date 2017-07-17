#ifndef DIFRAME_CONTAINER_H
#define DIFRAME_CONTAINER_H

#include "forward_declaration.h"
#include "utils/container_impl.h"

namespace DIFrame {

    //Dep  = Dependency;
    //Cons = Constructor;
    //Sig  = Signature;
    template <typename Dependency, typename Constructor, typename Signature>
    class ContainerImpl {
    private:
        Reflections reflections;

        ContainerImpl() = default;

        ContainerImpl( Reflections&& reflections );

        template <typename Dep_Other, typename Cons_Other, typename Sig_Other>
        ContainerImpl( const ContainerImpl<Dep_Other, Cons_Other, Sig_Other>& ContainerImpl_Other );

    private:
        template <typename... Types>
        friend class DIFrame::Get;
  
        template <typename... Types>
        friend class DIFrame::Container;
  
        template <typename OtherRs, typename OtherPs, typename OtherDeps>
        friend struct DIFrame::utils::ContainerImpl;
  
        template <typename M, typename ToRegister>
        friend struct DIFrame::utils::ModuleConversionHelper;
  
        template <typename M, typename TargetRequirements, bool is_already_provided, typename C>
        friend struct DIFrame::utils::EnsureProvidedTypeHelper;
    
        template <typename M>
        friend struct DIFrame::utils::Identity;
  
        template <typename M, typename I, typename C>
        friend struct DIFrame::utils::Bind;
  
        template <typename M, typename C>
        friend struct DIFrame::utils::RegisterInstance;
  
        template <typename M, typename Signature>
        friend struct DIFrame::utils::RegisterProvider;
  
        template <typename M, typename AnnotatedSignature>
        friend struct DIFrame::utils::RegisterFactory;
  
        template <typename M, typename Signature>
        friend struct RegisterConstructor;
  
        template <typename M, typename AnnotatedSignature>
        friend struct DIFrame::utils::RegisterConstructorAsFactory;
  
        template <typename M, typename OtherM>
        friend struct DIFrame::utils::InstallModule;

    public:
        using Dep  = Dependency;
        using Cons = Constructor;
        using Sig  = Signature;
        using This = ContainerImpl<Dep, Cons, Sig>;

    public:
        template <typename Interface, typename Implement>
        resultOf<Set<This, Interface, Implement>, This&&>
        set() && {
            return Set<This, Interface, Implement>()(std::move(*this));
        }

        template <typename >
        setExtraConstructor

        template <typename >
        setInstance

        //??
        template <typename >
        registerProvider

        template <typename >
        setFactory

        template <typename >
        appendContainer
    
    }

    template <typename... Types>
    class Container;

    template <typename... Types>
    class Container : public Container<Dependency<>, Types...> {
    private:
        Container() = default;
        friend Container<> createContainer();

        template <typename Dep_Other, typename Cons_Other, typename Sig_Other, typename Set_Other>
        ContainerImpl( const ContainerImpl<Dep_Other, Cons_Other, Sig_Other, Set_Other>& ContainerImpl_Other );

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

    inline Container<> createContainer() {
        return {};
    }

} // namespace DIFrame

#endif // DIFRAME_CONTAINER