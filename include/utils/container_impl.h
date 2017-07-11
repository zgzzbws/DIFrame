#ifndef DIFRAME_CONTAINER_IMPL_H
#define DIFRAME_CONTAINER_IMPL_H

namespace DIFrame {
namespace utils {
    
    template <typename Dependency, typename Constructor, typename Signature, typename Settings>
    class ContainerImpl {
    private:
        Build build;

    public: 
        ContainerImpl() = default;
        ContainerImpl( ContainerImpl&& containerImpl );
        
        template <typename Dependency_Other, typename Constructor_Other, typename Signature_Other, typename Settings_Other>
        ContainerImpl( const ContainerImpl<Dependency_Other, Constructor_Other, Signature_Other, Settings_Other>& ContainerImpl_Other );

    public:
        using Dep = Dependency;
        using Cons = Constructor;
        using Sig = Signature;
        using Set = Settings;
        using This = PartialComponent<Dep, Cons, Sig, Set>;

    public:
        //The friend class or structs to this container
        template <typename... Types>
        friend class DIFrame::Get;

        template <typename... Types>
        friend class DIFrame::Container;

        




        
    };

} // namespace utils
} //namespace DIFrame

#endif // DIFRAME_CONTAINER_IMPL_H