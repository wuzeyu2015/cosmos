#include <iostream>
#include "assert.h"
class Foo{};

//函数调用的类型推导：
int& func_int_r(void);    //左值（lvalue，可简单理解为可寻址值）
int&& func_int_rr(void);  //x值（xvalue，右值引用本身是一个xvalue）
int func_int(void);       //纯右值（pvalue）

const int& func_cint_r(void);         //左值
const int&& func_cint_rr(void);       //x值
const int func_cint(void);            //#1.纯右值的内置类型exp无法携带cv限定符
const Foo func_cfoo(void);  		  //#2.纯右值的类类型exp可以携带cv限定符

int main(void)
{
	int x = 0;

	decltype(func_int_r()) a1 = x;    //a1 -> int&
	decltype(func_int_rr()) b1 = 0;   //b1 -> int&&
	decltype(func_int()) c1 = 0;      //c1 -> int

	decltype(func_cint_r()) a2 = x;    //a2 -> const int&
	decltype(func_cint_rr()) b2 = 0;   //b2 -> const int&&
	decltype(func_cint()) c2 = 0;      //c2 -> int, const限定符号被忽略
	c2 = 5;
	decltype(func_cfoo()) ff = Foo();  //ff -> const Foo

	//其他情况（括号、运算符等）下的推导规则：
	struct Foo{int x;};
	const Foo foo = Foo();
	int m = 1;
	int n = 2;
	decltype(m + n) o = 0;   //exp为右值, o -> int
    decltype(m += n) p = o;  //exp为左值, p -> int&
	decltype((m + n)) q = 0; //exp为右值, q -> int
	decltype(foo.x) r = o;   //不属于其他情况，是一般情况，r -> int
	r = 4;
	decltype((foo.x)) s = o; //exp为左值, s -> const int&
	system("pause");
	return 0;
}