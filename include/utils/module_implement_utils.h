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
        struct check_provided_type <Module, targetRequirements, true, C> : public Identify<Module> {};

        //if the typeis not provided or is not in the target requirements, then try to auto register it
        template <typename Module, typename targetRequirements, typename C>
        struct check_provided_type <Module, targetRequirements, false, C> : public auto_register<Module, targetRequirements, C> {};

        //********************
        template <typename Module, typename targetRequirements, typename Parms>
        struct check_provided_types : public Identify<Module> {
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

        

        













    } //namespace utils 
}  //namespace DIFrame



















#endif //DIFRAME_MODULE_IMPLEMENT_UTILS_H
