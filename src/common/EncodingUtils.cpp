#include "common/EncodingUtils.h"
#include <windows.h>
#include <string>

/// =======================================
/// big5ToUtf8()
/// - 將 BIG5 字串轉換為 UTF-8 編碼字串（使用 Windows API）
/// =======================================
std::string EncodingUtils::big5ToUtf8(const std::string& big5str) {
    // Step 1: BIG5 → UCS-2 (寬字元)
    int wideLen = MultiByteToWideChar(950 /* BIG5 */, 0, big5str.c_str(), -1, nullptr, 0);
    if (wideLen <= 0) return "";  // 錯誤處理

    std::wstring wideStr(wideLen, L'\0');
    MultiByteToWideChar(950, 0, big5str.c_str(), -1, &wideStr[0], wideLen);

    // Step 2: UCS-2 → UTF-8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Len <= 0) return "";  // 錯誤處理

    std::string utf8Str(utf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Len, nullptr, nullptr);

    // ⚠ 移除多餘的 null terminator（std::string 包含 '\0'）
    if (!utf8Str.empty() && utf8Str.back() == '\0') {
        utf8Str.pop_back();
    }

    return utf8Str;
}

/// =======================================
/// padStockCode()
/// - 將股票代碼補齊成 6 字元（右補空格）
/// =======================================
std::string EncodingUtils::padStockCode(const std::string& input) {
    std::string padded = input;
    padded.resize(6, ' ');  // 若不足 6 字元，右補空格
    return padded;
}

/// =======================================
/// toPrice()
/// - 將原始整數（如 123456）轉換為價格（例：12.3456）
/// - 用於解析 BCD 編碼價格欄位
/// =======================================
double EncodingUtils::toPrice(int raw) {
    return raw / 10000.0;  // 假設為 4 位小數（ex: 123456 → 12.3456）
}
