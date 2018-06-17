#include <iostream>
#include <vector>

template <class ContainerT>
class Foo
{
	typename ContainerT::iterator it_;  //���Ͷ������������
	//������ const ContainerT ��ͨ��������ʹ������it_���壺
	//decltype(std::declval<ContainerT>().begin()) it_;
public:
	void func(ContainerT& container)
	{
		it_ = container.begin();
	}

	//...
};
//#1.ģ���ػ�
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
	//#2.�����ڸ����û�ָ�������������Ƶ�it_�����ͣ�const_iterator or iterator?
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
	typedef const std::vector<int> container_t;  //������const���ԣ�foo.func(arr)���뱨��
	container_t arr;

	Foo<container_t> foo;
	foo.func(arr);

	Foo2<container_t> foo2;
	foo2.func(arr);

	system("pause");
	return 0;
}