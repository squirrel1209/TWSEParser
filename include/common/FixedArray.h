#ifndef FIXEDARRAY_H
#define FIXEDARRAY_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

// ===============================
// 固定長度陣列：封裝靜態大小的陣列並提供安全存取接口
// 適用於封包解析中具有固定長度欄位（如 ASCII6, BCD5 等）
// ===============================
#pragma pack(push, 1)
template <typename T, std::size_t N>
class FixedArray {
private:
    T arr[N];  // 內部資料儲存

public:
    /// 預設建構子：值未初始化（由 .tpp 決定是否清空）
    FixedArray();

    /// 初始化建構子（支援大括號初始化）
    FixedArray(std::initializer_list<T> init);

    /// 取得指定索引的元素參考（無範圍檢查）
    T& operator[](std::size_t index);

    /// 取得指定索引的 const 元素參考
    const T& operator[](std::size_t index) const;

    /// 取得陣列大小（固定為 N）
    std::size_t size() const;

    /// 回傳可修改的資料指標
    T* data();

    /// 回傳唯讀的資料指標
    const T* data() const;
};
#pragma pack(pop)

#include "FixedArray.tpp"

#endif // FIXEDARRAY_H
