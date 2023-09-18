#pragma once

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>
#include <type_traits>

#include "iterator.hpp"
#include "type_traits.hpp"
#include "util.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#endif

namespace mystl {

// construct 构造对象

template <typename Ty>
void construct(Ty* ptr) {
    ::new ((void)*ptr) Ty{};
}

template <typename Ty1, typename Ty2>
void construct(Ty1* ptr, const Ty2& value) {
    ::new ((void*)ptr) Ty1{value};
}

template <typename Ty, typename... Args>
void construct(Ty* ptr, Args&&... args) {
    ::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
}

// destroy 将对象析构

template <typename Ty>
void destroy_one(Ty*, std::true_type) {}

template <typename Ty>
void destory_one(Ty* pointer, std::false_type) {
    if (pointer != nullptr) {
        pointer->~Ty();
    }
}

template <typename ForwardIter>
void destory_cat(ForwardIter, ForwardIter, std::true_type){};

template <typename ForwardIter>
void destory_cat(ForwardIter first, ForwardIter last, std::false_type) {
    for (; first != last; ++first) {
        destory(&*first);
    }
}

template <typename Ty>
void destory(Ty* pointer) {
    destory_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <typename ForwardIter>
void destory(ForwardIter first, ForwardIter last) {
    destory_cat(first, last,
                std::is_trivially_destructible<
                    typename iterator_traits<ForwardIter>::value_type>{});
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

}  // namespace mystl