#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "TWSEParsedResult.h"

// ========================================
// TWSEFormatParser 抽象解析器介面
// ========================================
// 所有格式（Format1、Format6⋯）都應實作本介面
// 每個 parser 負責：
// - 從原始封包位元組解析內容
// - 傳回解析結果物件（shared_ptr）
// - 提供格式名稱識別（例如 "Format6"）
// ========================================
class TWSEFormatParser {
public:
    virtual ~TWSEFormatParser() = default;

    /// 解析指定位元組資料，成功時回傳 true
    virtual bool parse(const uint8_t* data, size_t length) = 0;

    /// 回傳此解析器的格式名稱（如 "Format1", "Format6"）
    virtual std::string getFormatName() const = 0;

    /// 傳回解析結果物件（由 TWSEParsedResult 衍生）
    virtual std::shared_ptr<TWSEParsedResult> getParsedResult() const = 0;
};
