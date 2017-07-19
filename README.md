# DIFrame框架

## 简介

DIFrame是一个使用C++编写的依赖注入（DI）容器，亦可称为控制反转（IoC）容器，该框架可以用来控制下层单元的实例化过程，以达到降低上层类和下层单元之间的耦合的目的。

##原理

要实现动态实例化的过程，最容易想到的就是Java中的反射机制，所以在这里我通过一个工厂方法类来模拟这种机制。其核心是一个散列表，在某一个类被实例化之前，散列表中存储了该类的构造和销毁方法，在其被调用时，通过构造方法对其实例化；在其生命周期结束时，通过销毁方法对内存进行回收。由此可见，这个框架的瓶颈取决于散列表的效率。

##特性

* 注册简单
    *  要将一个待调用类注册散列表，只需在该类的构造函数上使用`using Submit`标记即可
* 多种注册方式
    *  在调用第三方库的类时，可能无法直接标记构造函数，在此还可以通过类的实例或者签名式进行注册
* 实现构造函数注入
    *  到目前为止，以构造函数的注入方式为主。
* 编译期间的错误检测
    *  在编写过程中，我使用了C++11的一些特性，以及模板元编程，这决定了该框架可以在编译期间就对大部分错误进行检测。不过涉及到实例注入的话，就只能在运行时进行检测了。

##待完成

* 线程安全性
    *  关于线程安全性我还没有想好，希望可以找到一种不加锁的解决方案。
* 冲突
    *  如果多个类对应一个接口，可能需要额外的处理方式。

##示例
* 注册
```C++
//接口类
class Writer {
public:
  virtual void write(std::string s) = 0;
};

//实现类
class StdoutWriter : public Writer {
public:
  // Like "StdoutWriter() = default;" but also marks this constructor as the
  // one to use for injection.
  INJECT(StdoutWriter()) = default;
  
  virtual ~StdoutWriter() = default;
  
  virtual void write(std::string s) override {
    std::cout << s;
  }
};
```