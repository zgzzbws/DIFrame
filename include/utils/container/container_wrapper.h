#ifndef DIFRAME_CONTAINER_WRAPPER_H
#define DIFRAME_CONTAINER_WRAPPER_H

namespace DIFrame{
namespace utils{

    template <typename Contain>
    struct Identity {
        Contain operator()( Contain&& contain ) {
            return std::move(contain);
        };
    };

template <typename M, typename I, typename C>
struct Bind {
  using M1 = AddRequirement<M, C>;
  using M2 = AddProvide<M1, I, List<C>>;
  M2 operator()(M&& m) {
    FruitDelegateCheck(CheckClassType<I, GetClassForType<I>>);
    FruitDelegateCheck(CheckClassType<C, GetClassForType<C>>);
    FruitDelegateCheck(CheckBaseClass<I, C>);
    m.unsafeModule.template bind<I, C>();
    return M2(std::move(m.unsafeModule));
  };
};

    template <typename Contain, typename Interface, typename Implement >
    struct Set {
        using 
        using
        M2 operator()(Contain&& contain) {
            contain.reflections.template set<Interface, Implement>();
            return M2(std::move(contain.reflections));
        };
    };

}
}

#endif // DIFRAME_CONTAINER_WRAPPER_H