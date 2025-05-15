#pragma once
#include <string>

/// =======================================
/// EncodingUtils
/// - 處理字串轉碼、補齊、數值轉換等共用工具
/// =======================================
class EncodingUtils {
public:
    /// 將 BIG5 編碼轉換為 UTF-8 字串
    /// @param big5str 原始 BIG5 編碼字串
    /// @return 對應的 UTF-8 字串
    static std::string big5ToUtf8(const std::string& big5str);

    /// 將股票代碼補齊為固定 6 碼（不足前補 0 或空格）
    /// @param input 原始代碼字串
    /// @return 補齊後的 6 字元代碼
    static std::string padStockCode(const std::string& input);

    /// 將 BCD 原始整數轉換為價格（含兩位小數）
    /// @param raw 原始整數（如 12345 表示 123.45）
    /// @return 轉換後的 double 價格
    static double toPrice(int raw);  // ✅ 新增轉換價格函式
};
