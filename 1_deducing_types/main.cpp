#include <cstdlib>
#include <cstdint>
#include <iostream>

// case 1: when the param is a reference
template <typename T>
void foo_lref(T &param) {}
template <typename T>
void foo_cref(const T &param) {}

// case 2: when the param is a universal reference
template <typename T>
void foo_uref(T &&param) {}

// case 3: when the param is a value
template <typename T>
void foo_val(T param) {}

//
template <typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
  return N;
}

int main()
{
  std::int32_t x = 1;
  const std::int32_t cx = x;
  const std::int32_t &rx = x;

  // case 1
  foo_lref(x);  // T is int, param type is int&
  foo_lref(cx); // T is const int as the constness of cx is preserved via T,
                // param type is const int&
  foo_lref(rx); // T is const int as the constness of rx is preserved via T,
                // param type is const int&

  foo_cref(x);  // T is int, param type is const int&
  foo_cref(cx); // T is int as the constness of cx is preseverd via the const
                // adornment in the paramater list, param type is const int&
  foo_cref(rx); // T is int as the constness of rx is preseverd via the const
                // adornment in the paramater list, param type is const int&

  // case 2
  foo_uref(x);  // T is int&. The
                // param type is int&. (this is the unintuitive part)
  foo_uref(cx); // T is const int&. The
                // param type is const int&
  foo_uref(rx); // T is const int&. The
                // param type is const int&
  foo_uref(1);  // T is int. The
                // param type is int&&

  // case 3
  foo_val(x);  // T is int
  foo_val(cx); // T is int, the constness is not carried over as the parans are
               // just copies
  foo_val(rx); // T is int, the constness is not carried over as the parans are
               // just copies

  const int y = 10;
  const int *const ptr = &y; // a constant pointer to a constant int
  // /|\       /|\
  //  |         |
  //  |         |
  // ptr to     const ptr
  // const

  foo_val(ptr); // when ptr is passed by value the paramater is just a copy and
                // hence not a constant pointer. but it is still a pointer to a
                // constant int. T is const int* and param type is const int*

  int arr[10] = {};
  foo_val(arr);  // T is int*, param type is int*
  foo_lref(arr); // T is int[10], param type is int(&)[10]
                 // In case the parameter is a lvalue reference the actual
                 // array type is deduced and not just the pointer

  std::cout << "Array size: " << arraySize(arr) << "\n";

  return EXIT_SUCCESS;
}