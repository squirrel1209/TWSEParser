// TWSEFormat1Parser.h
#pragma once
#include "base/TWSEFormatParser.h"
#include "result/TWSEParsedFormat1Result.h"
#include <memory>
#include <string>

/// ========================================
/// TWSEFormat1Parser
/// - Format1 封包專用解析器（固定 114 bytes）
/// - 負責資料解析與完整性驗證
/// ========================================
class TWSEFormat1Parser : public TWSEFormatParser {
private:
    std::shared_ptr<TWSEParsedFormat1Result> result;

public:
    /// 傳回格式名稱（"Format1"）
    std::string getFormatName() const override;

    /// 傳回解析後結果（封裝於 TWSEParsedFormat1Result 中）
    std::shared_ptr<TWSEParsedResult> getParsedResult() const override;

    /// 執行封包解析流程
    /// - 使用 memcpy 將原始資料轉換為結構
    /// - 驗證 ESC / XOR / Terminal 是否通過
    bool parse(const uint8_t* data, size_t length) override;
};
