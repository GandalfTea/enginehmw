
#include <iostream>
#include <cstdint>
#include <vector>
#include <stdio.h>

template <typename T>
struct allocator {
	std::vector<std::vector<T>> matrix;
	std::vector<T> row;
	T type;
};

template <class T>
class Balls {
	public:
		T var;
};

int main() {
	uint32_t dick = 69;
	using TypePtr = std::remove_reference<decltype(dick)>::type;
	allocator<TypePtr> a;

	Balls<int> balls;
	balls.var = 5;

	std::cout << typeid(a.type).name() << std::endl;

	return 0;
}
