#include <iostream>
using namespace std;
void func(auto a = 1);  //error:auto�������ں�������

struct Foo
{
	auto var1_ = 0;  //error:auto�������ڷǾ�̬��Ա����
	static const auto var2_ = 0;
};

template <typename T>
struct Bar {};

int main(void)
{
	int arr[10] = {0};
	auto aa = arr;  //OK: aa -> int *
	auto rr[10] = arr;  //error:auto�޷���������
	Bar<int> bar;
	Bar<auto> bb = bar;  //error:auto�޷��Ƶ���ģ�����

	system("pause");
	return 0;
}