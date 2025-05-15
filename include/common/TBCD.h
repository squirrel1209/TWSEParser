#pragma once
#include "FixedArray.h"
#include <cstdint>
#include <cstddef>
#include <string>
#include <sstream>
#include <iomanip>

// ============================================
// TBCD<N>：泛型 BCD 欄位封裝（Binary-Coded Decimal）
// - 適用於證交所封包中常見的數值欄位（價格、成交量等）
// - 每個 byte 儲存兩位十進位數（例如 0x45 表示數字 45）
// ============================================
template <std::size_t N>
class TBCD {
private:
    FixedArray<uint8_t, N> raw;

public:
    /// 預設建構子
    TBCD() = default;

    /// 由 FixedArray 原始資料建構
    explicit TBCD(const FixedArray<uint8_t, N>& data) : raw(data) {}

    /// 取得唯讀資料陣列
    const FixedArray<uint8_t, N>& data() const { return raw; }

    /// 取得可修改資料陣列
    FixedArray<uint8_t, N>& data() { return raw; }

    /// 擷取指定 byte
    uint8_t operator[](std::size_t i) const { return raw[i]; }
    uint8_t& operator[](std::size_t i) { return raw[i]; }

    /// 將 BCD 資料轉為整數（例如 0x12 0x34 → 1234）
    int toInt() const {
        int result = 0;
        for (std::size_t i = 0; i < N; ++i)
            result = result * 100 + ((raw[i] >> 4) & 0xF) * 10 + (raw[i] & 0xF);
        return result;
    }

    /// 將 BCD 資料轉為十進位字串（例如顯示用途）
    std::string toString() const {
        std::ostringstream oss;
        for (std::size_t i = 0; i < N; ++i) {
            oss << ((raw[i] >> 4) & 0xF);
            oss << ((raw[i]     ) & 0xF);
        }
        return oss.str();
    }

    // ✅ 可擴充：toDecimal(int decimalPlaces)
};

// ✅ 類型別名
typedef TBCD<1> BCD1;
typedef TBCD<2> BCD2;
typedef TBCD<3> BCD3;
typedef TBCD<4> BCD4;
typedef TBCD<5> BCD5;
typedef TBCD<6> BCD6;
