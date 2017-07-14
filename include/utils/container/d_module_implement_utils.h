#ifndef DIFRAME_MODULE_IMPLENENT_UTILS_H
#define DIFRAME_MODULE_IMPLEMENT_UTILS_H

namespace DIFrame {
    namespace utils {

        //********************
        //add requirement(s) to the module
        template <typename Module, bool is_presented, typename C>
        struct add_requirement_impl {};

        template <typename Module, typename C>
        struct add_requirement_impl <Module, true, C> {
            using type = Module;
        };

        template <typename Module, typename C>
        struct add_requirement_impl <Module, false, C> {
            using type = module_impl<attach_parm<C, typename Module::Rs>, typename Module::Ps, typename Module::Deps>;
        };

        template <typename Module, bool is_presented, typename C>
        using add_requirement = typename add_requirement_impl <Module, parm_in_list<C, typename Module::Ps>::value || parm_in_list<C, typename Module::Rs>::value ,C>::type;

        //*********************
        template <typename Module, typename L>
        struct add_requirements_impl {};

        template <typename Module, typename L>
        struct add_requirements_impl {
            //check assert in here

            using type = Module;
        };

        template <typename Module, typename R, typename... Rs>
        struct add_requirements_impl <Module, parm<R, Rs...>> {
             using temp_module = typename add_requirements_impl<Module, parm<Rs...>>::type;
             using type = add_requirement<temp_module, R>;
        };

        template <typename Module, typename L>
        using add_requirements = typename add_requirements_impl<Module, L>::type;

        //********************
        //Compare to the adding operation, we also need a removing operation
        template <typename Module, typename C>
        using remove_requirement = module_impl<remove_parm<C, typename Module::Rs>, typename M::Ps, remove_requirement_from_sigsets<C, typename M::Deps>>;

        //********************
        //It is time to add sth to provider
        template <typename Module, typename C, typename Parms>
        struct add_provide_impl {
            using temp_sigsets = add_to_sigset<construct_sigset<C, Parms>, typename Module::Deps>;
            static_assert( true || sizeof(temp_sigsets), "" );
            //check assert in here
            using temp_module = module_impl<typename M::Rs, attach_parm<C, typename Module::Ps>, temp_sigsets>;
            using type = remove_requirement<temp_module, C>
        };

        template <typename Module, typename C, typename Parms>
        using add_provide = typename add_provide_impl<Module, C, Parms>::type;

        //********************
        //register sth
        template <typename Module, typename targetRequirements, typename C>
        struct auto_register {};

        //********************
        //Now check provided type
        //flag here is used to judge whether the type is provided before or in the target requirements 
        template <typename Module, typename targetRequirements, bool flag, typename C>
        struct check_provided_type {};

        //if the type is provided before or is in the target requierments
        templage <typename Module, typename targetRequirements, typename C>
        struct check_provided_type <Module, targetRequirements, true, C> : public Identity<Module> {};

        //if the typeis not provided or is not in the target requirements, then try to auto register it
        template <typename Module, typename targetRequirements, typename C>
        struct check_provided_type <Module, targetRequirements, false, C> : public auto_register<Module, targetRequirements, C> {};

        //********************
        template <typename Module, typename targetRequirements, typename Parms>
        struct check_provided_types : public Identity<Module> {
            //check assert in here
        };

        template <typename Module, typename targetRequirements, typename P, typename... Parms>
        struct check_provided_types <Module, targetRequirements, parm<P, Parms...>> {
            using temp_P = check_provided_type <Module, 
                                                targetRequirements, 
                                                parm_in_list<getType<P>, typename Module::Ps>::value
                                             || parm_in_list<getType<P>, targetRequirements>::value,
                                                getType<P>>;
            using temp_module1 = functor_result<temp_P, Module&&>;
            using temp_Ps = check_provided_types<temp_module1, targetRequirements, parm<Parms...>>;
            using temp_module2 = functor_result<temp_Ps, temp_module1&&>;

            temp_module2()(Module&& m) {
                return temp_Ps()(temp_P()(std::move(m)));
            }
        };

        //********************
        //
        template <typename Module, typename targetRequirements, bool has_inject_annotation ,typename C>
        struct auto_register_impl {};

        template <typename Module, typename targetRequirements, typename C>
        struct auto_register_impl <Module, targetRequirements, true, C> {
            using temp_register = ///registerConstructor<Module, get_inject_annotation<C>::Signature>;
            using temp_module1 = functor_result<temp_register, Module&&>;
            using register_args = check_provided_types<temp_module1, targetRequirements, ///expandInjectorsInParms<typename get_inject_annotation<C>::Args>>;
            using temp_module2 = functor_result<register_args, temp_module1>;

            temp_module2 operator()(Module&& m){
                return register_args()(temp_register()(std::move(m)));
            }
        };

        template <typename Module, typename targetRequirements, typename C>
        struct auto_register_impl <Module, targetRequirements, false, C> {
            //check assert in here
        };

        //********************
        //
        template <typename Module, typename targetRequirements, bool has_inject_annotation, typename C, typename... args>
        struct auto_register_factory_impl {};

        template <typename Module, typename targetRequirements, typename C, typename... args>
        struct auto_register_factory_impl <Module, targetRequirements, false, C, args...> {
            //check assert in here
        };        
    
        template <typename Module, typename targetRequirements, typename C, typename... arg>
        struct auto_register_factory_impl <Module, targetRequirements, true, C, arg...> {
            using thisSignature = typename get_inject_annotation<C>::Signature;
            //check assert in here
            using removedAssistedAsgs = get_non_assisted_parm<signatureArgs<thisSignature>>;
            using temp_register = //registerConstructorAsFactory<Module, thisSignature>;
            using temp_module1 = functor_result<temp_register, Module&&>;
            using auto_register_args = check_provided_types<temp_module1, targetRequirements, //expendInjectorInParms<thisSignature>>;
            using temp_module2 = functor_result<auto_register_args, temp_module1&&>;

            temp_module2 operator()(Module&& m){
                return auto_register_args()(temp_register()(std::move(m)));
            }
        };

        //********************
        //looking for a typedef called Inject inside C to register C
        template <typename Module, typename targetRequirements, typename C>
        struct auto_register : public auto_register_impl <Module, targetRequirements, has_inject_annotation<C>::value , C> {};

        template <typename Module, typename targetRequirements, typename C, typename... args>
        struct auto_register <Module, targetRequirements, std::function<C(args...)>> : public auto_register_factory_impl<Module, 
                                                                                                                         targetRequirements, 
                                                                                                                         has_inject_annotation<C>::value, 
                                                                                                                         C, 
                                                                                                                         args...> {};

        //*******************
        //
        template <typename Module>
        struct Indetity {
            Module operator()(Module&& m) {
                return std::move(m);
            }
        };
   
        //*******************
        template <typename Module, typename I, typename C>
        struct Bind {
            using temp_module1 = add_requirement<Module, C>;
            using temp_module2 = add_provide<temp_module1, I, parm<C>>;

            temp_module2 operator()(Module&& m){
                //check assert in here
                //check assert in here
                //check assert in here
                m.unsafeModule.template bind<I, C>();
                return temp_module2(std::move(m.unsafeModule));
            };
        };

        //********************
        //
        template <typename Module, typename Signature>
        struct register_provider {};

        template <typename Module, typename T, typename... args>
        struct register_provider <Module, T(args...)>{
            using Signature = T(args...);
            using signature_requirements = //expendInjectorsInParms<parm<getType<args>...>>;
            using temp_module1 = add_requirements<Module, signature_requirements>;
            using temp_module2 = add_provide<temp_module1, getType<T>, signature_requirements>;

            temp_module2 operator()(Module&& m, Signature* provider){
                m.unsafeModule.registerProvider(provider);
                return std::move(m.unsafeModule);
            }
        };

        //*******************
        //
        template <typename Module, typename annotatedSignature>
        struct register_factory {
            using temp_type = function_injection_with_assisted<annotatedSignature>;
            using required_signature = necessary_signature_construction<annotatedSignature>;
            using temp_requirement = //expendInjectorInParms<obtain_necessary_parm_avoid_assisted_mark<signatureArgs<annotatedSignature>>>;
            using temp_module1 = add_requirements<Module, temp_requirement>;
            using temp_module2 = add_provide<temp_module1, std::function<temp_type>, temp_requirement>;
            
            temp_module2 operator()(Module&& m, required_signature* factory) {
                m.unsafeModule.template registerFactory<annotatedSignature>(factory);
                return std::move(m.unsafeModule);
            }
        };

        //********************
        //
        template <typename Module, typename Signature>
        struct register_constructor {
            using provider = decltype(provide_construct<Signature>::f);
            using provide_registration = register_provider<Module, provider>;
            using temp_module1 = functor_result<provide_registration, Module&&, provider*>;

            temp_module1 operator()(Module&& m){
                return provide_registration()(std::move(m), provide_construct<Signature>::f);
            }
        };

        template <typename Module, typename C>
        struct register_instance {
            using temp_module1 = add_provide<Module, C, parm<>>;

            temp_module1 operator()(Module&& m, C* instance) {
                m.unsafeModule.bindInstance(instance);
                return std::move(m.unsafeModule);
            }
        };

        template <typename Module, typename annotatedSignature>
        struct register_constructor_as_factory {
            using required_signature = necessary_signature_construction<annotatedSignature>;
            using provider = decltype(provide_factory_construct<required_signature>::f);
            using factory_registration = register_factory<Module, annotatedSignature>;
            using temp_module1 = functor_result<factory_registration, M&&, provider*>;

            temp_module1  operator()(Module** m){
                return factory_registration()(std::move(m), provide_factory_construct<required_signature>::f);
            }
        };

        template <typename Module, typename otherModule>
        struct register_module {
            using otherModule_Rs = typename otherModule::Rs;
            using otherModule_Ps = typename otherModule::Ps;
            using otherModule_Deps = typename otherModule::Deps;

            //assert is here
            using compose_Ps = join_parm<typename Module::Ps, otherModule_Ps>;
            using compose_Rs = figure_differences<combine_parm_lists<typename Module::Rs, otherModule_Rs>, compose_Ps>;
            using compose_Deps = add_to_sigsets<typename Module::Deps, otherModule_Deps>;

            using temp_module1 = module_impl<compose_Rs, compose_Ps, compose_Deps>;

            temp_module1 operator()(Module&& m, const otherModule& otherM) {
                m.unsafeModule.install(otherM.unsafeModule);
                return std::move(m.unsafeModule);
            }
        };

        //**********************
        //
        template <typename XXX>
        ModuleImpl






    } //namespace utils 
}  //namespace DIFrame



















#endif //DIFRAME_MODULE_IMPLEMENT_UTILS_H
