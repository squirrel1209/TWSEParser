#pragma once
#include "TWSEFormatParser.h"
#include <memory>
#include <cstdint>

/// =======================================
/// TWSEParserFactory
/// - 封包解析器工廠
/// - 根據 formatCode 回傳對應的 FormatParser 實例
/// - 使用 std::shared_ptr 建立動態多型解析器
/// =======================================
class TWSEParserFactory {
public:
    /// 工廠方法：根據格式碼建立對應格式的解析器
    /// @param formatCode TWSE 格式代碼（例：0x01, 0x06）
    /// @return 繼承自 TWSEFormatParser 的解析器實例（多型）
    static std::shared_ptr<TWSEFormatParser> createParser(uint8_t formatCode);
};
