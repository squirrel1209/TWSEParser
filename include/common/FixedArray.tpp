#pragma once

// ✅ 預設建構子：初始化所有元素為 T{}
template <typename T, std::size_t N>
FixedArray<T, N>::FixedArray() {
    for (std::size_t i = 0; i < N; ++i)
        arr[i] = T{};
}

// ✅ 初始化列表建構子
template <typename T, std::size_t N>
FixedArray<T, N>::FixedArray(std::initializer_list<T> init) {
    std::size_t i = 0;
    for (T val : init) {
        if (i < N)
            arr[i++] = val;
        else
            break;
    }
    for (; i < N; ++i)
        arr[i] = T{};
}

// ✅ operator[] 加入邊界檢查（可切換）
template <typename T, std::size_t N>
T& FixedArray<T, N>::operator[](std::size_t index) {
#ifdef FIXEDARRAY_BOUNDS_CHECK
    if (index >= N)
        throw std::out_of_range("FixedArray: index out of range");
#endif
    return arr[index];
}

template <typename T, std::size_t N>
const T& FixedArray<T, N>::operator[](std::size_t index) const {
#ifdef FIXEDARRAY_BOUNDS_CHECK
    if (index >= N)
        throw std::out_of_range("FixedArray: index out of range");
#endif
    return arr[index];
}

// ✅ 回傳固定大小
template <typename T, std::size_t N>
std::size_t FixedArray<T, N>::size() const {
    return N;
}

// ✅ 指標存取
template <typename T, std::size_t N>
T* FixedArray<T, N>::data() {
    return arr;
}

template <typename T, std::size_t N>
const T* FixedArray<T, N>::data() const {
    return arr;
}
