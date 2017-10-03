# DIFrame框架

## 简介

DIFrame是一个使用C++编写的依赖注入`(Dependence Injection, DI)`容器，亦可称为控制反转`(Inversion of Control, IoC)`容器，该框架可以用来控制下层单元的实例化过程，以达到降低上层类和下层单元之间的耦合的目的。

## 原理

这一框架主要有三个组件构成，分别是Container，Reflections和Getter。其中Container用于对类之间的依赖关系进行管理，同时负责将类的构造等方法注册到Reflections。接下来，我使用工厂模式（即Reflections模块）实现了对象动态实例化的过程。其核心是一个散列表，在某一个类被实例化之前，散列表中存储了该类的构造以及析构方法，在其被调用时，通过构造方法对其实例化；在其生命周期结束时，通过销毁方法对内存进行回收。由此可见，这个框架的瓶颈取决于散列表的效率。最后，通过接口Getter返回相应的实例。

## 特性

* 注册简单
    *  要将一个待调用类注册到散列表，只需在该类的构造函数上使用`using Submit`标记即可；如果一个类需要一个或者多个依赖类，需要将其依赖关系通过链式编程的方式注册到对应的Container，Container负责最终进行组装。
* 多种注册方式
    *  在调用第三方库的类时，可能无法直接标记构造函数，在此还可以通过类的实例或者签名式进行注册
* 实现构造函数注入
    *  到目前为止，以构造函数的注入方式为主。
* 编译期间的错误检测
    *  在编写过程中，我使用了C++11的一些特性，以及模板元编程，这决定了该框架可以在编译期间就对大部分错误进行检测。不过涉及到实例注入的话，就只能在运行时进行检测了。

## 优化

* 为了提高容器的效率，优化工作主要从散列表入手。
    *  之前使用的散列表是std::unordered_map，这一散列表使用了链表来避免键与键冲突的问题，这个解决方案由于内存上的不连续容易出现cache miss。
    *  我分别在以下几个方面对其进行优化：1）在最底层采用开放寻址和线性探测的方法，由于其实现是连续的一块内存，且查找键的方式是线性的，这样迎合了CPU Prefetcher的工作方式，在减轻其压力的同时避免了cache miss。 2）在hash策略上，采用Robin Hood Hash， 传统的线性探测容易形成较长的键簇，这样探测长度过长容易造成cache miss，而Robin Hood Hash通过对元素进行交换，保证映射到同一个bucket的元素聚集到一起，避免了过长探测的问题。3) 即便如此，当插入键有一定的规律，探测长度还是会超出cache line的大小，在这种情况下，当探测长度超过一定的值，我就扩大数组，进行rehash。
    *  正在测试该算法当插入较大对象时的效率，稍后上传这一部分。

## 待完成

* 线程安全性
    *  关于线程安全性我还没有想好，希望可以找到一种不加锁的解决方案。
* 冲突
    *  如果多个类对应一个接口，可能需要额外的处理方式。

## 示例
* 注册
```C++
//实现类
class Pen : public WritingTools {
public:
    Pen(Color* color) : color(color) {

    }
    
    //使用Submit标识要注册的类
    using Submit = Pen(Color*);

    // 实现方法
    //...

private:
    Color* color;

};
```

* 构造
```C++
//实现类
//假设无法直接在类中标识
class Pen : public WritingTools {
public:
    Pen(Color* color) : color(color) {

    }

    // 实现方法
    // ...

private:
    Color* color;

};

DIFrame::Container<DIFrame::Dependency<Color>, WritingTools> ObtainContainer {
    //选择链式编程的方式
    //在这里使用setWithConstructor进行注册
    return DIFrame::createContainer()
                    .setWithConstructor<Pen(Color*)>()
                    .set<WritingTools, Pen>();
}
```
