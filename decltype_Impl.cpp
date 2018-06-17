#include <iostream>
#include <vector>

template <class ContainerT>
class Foo
{
	typename ContainerT::iterator it_;  //类型定义可能有问题
	//若想在 const ContainerT 下通过编译请使用下面it_定义：
	//decltype(std::declval<ContainerT>().begin()) it_;
public:
	void func(ContainerT& container)
	{
		it_ = container.begin();
	}

	//...
};
//#1.模板特化
template <class ContainerT>
class Foo<const ContainerT>
{
	typename ContainerT::const_iterator it_; 
public:
	void func(const ContainerT& container)
	{
		it_ = container.begin();
	}

	//...
};

template <class ContainerT>
class Foo2
{
	//#2.编译期根据用户指定类型类型来推导it_的类型：const_iterator or iterator?
	decltype(ContainerT().begin()) it_;
public:
	void func(ContainerT& container)
	{
		it_ = container.begin();
	}

	//...
};
int main(void)
{
	typedef const std::vector<int> container_t;  //这里有const属性，foo.func(arr)编译报错
	container_t arr;

	Foo<container_t> foo;
	foo.func(arr);

	Foo2<container_t> foo2;
	foo2.func(arr);

	system("pause");
	return 0;
}