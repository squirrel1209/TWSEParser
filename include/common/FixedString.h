#pragma once
#include <string>
#include <cstring>
#include <algorithm>
#include <functional>
#include "FixedArray.h"

// ==============================================
// 固定長度字串類型（基於 FixedArray<char, N>）
// - 可作為 std::unordered_map 的 key
// - 支援 const char* / std::string 初始化
// - 支援比較、轉換成 std::string
// ==============================================
template <std::size_t N>
struct FixedString {
    FixedArray<char, N> data;  // 實際儲存資料的固定陣列

    /// 預設建構子（不初始化內容）
    FixedString() = default;

    /// 從 C 字串初始化（最多 N 個字元）
    FixedString(const char* s) {
        std::size_t len = std::min(N, std::strlen(s));
        for (std::size_t i = 0; i < len; ++i)
            data[i] = s[i];
        for (std::size_t i = len; i < N; ++i)
            data[i] = '\0';  // 補齊空字元
    }

    /// 從 std::string 初始化（最多 N 個字元）
    FixedString(const std::string& s) {
        std::size_t len = std::min(N, s.size());
        for (std::size_t i = 0; i < len; ++i)
            data[i] = s[i];
        for (std::size_t i = len; i < N; ++i)
            data[i] = '\0';
    }

    /// 將固定長度字串轉為 std::string（遇 \0 結束）
    std::string toString() const {
        return std::string(data.data(), strnlen(data.data(), N));
    }

    /// 等號比較運算子（必要條件：作為 map/unordered_map key）
    bool operator==(const FixedString& other) const {
        return std::memcmp(data.data(), other.data.data(), N) == 0;
    }

    /// 小於比較運算子（必要條件：作為 std::map key）
    bool operator<(const FixedString& other) const {
        return std::lexicographical_compare(data.data(), data.data() + N,
                                            other.data.data(), other.data.data() + N);
    }
};

// ==============================================
// ✅ 為了支援 unordered_map，需要實作 std::hash 特化
// ==============================================
namespace std {
    template <std::size_t N>
    struct hash<FixedString<N>> {
        std::size_t operator()(const FixedString<N>& s) const {
            return std::hash<std::string_view>()(
                std::string_view(s.data.data(), N)
            );
        }
    };
}
