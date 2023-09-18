#pragma once

// 这个头文件包含一个模板类
// allocator，用于管理内存的分配、释放，对象的构造、析构

#include <new>

#include "construct.hpp"
#include "util.hpp"

namespace mystl {

// 模板类：allocator
// 模板函数代表数据类型
template <typename T>
class allocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

public:
    static T* allocate() { return static_cast<T*>(::operator new(sizeof(T))); }
    static T* allocate(size_type n) {
        if (n == 0) {
            return nullptr;
        }

        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    static void deallocate(T* ptr) {
        if (ptr == nullptr) {
            return;
        }

        ::operator delete(ptr);
    }
    static void deallocate(T* ptr, size_type /*size*/) {
        if (ptr == nullptr) {
            return;
        }

        ::operator delete(ptr);
    }

    static void construct(T* ptr) {
        mystl::construct(ptr);  // placement new
    }
    static void construct(T* ptr, const T& value) {
        mystl::construct(ptr, value);
    }
    static void construct(T* ptr, T&& value) {
        mystl::construct(ptr, mystl::move(value));
    }

    template <typename... Args>
    static void construct(T* ptr, Args&&... args) {
        mystl::construct(ptr, mystl::forward<Args>(args)...);
    }

    static void destory(T* ptr) { mystl::destory(ptr); }
    static void destory(T* first, T* last) { mystl::destory(first, last); }
};

}  // namespace mystl