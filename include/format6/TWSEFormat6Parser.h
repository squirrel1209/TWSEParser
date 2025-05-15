#pragma once
#include "base/TWSEFormatParser.h"
#include "result/TWSEParsedFormat6Result.h"
#include <memory>
#include <string>

/// =======================================
/// TWSEFormat6Parser
/// - Format6 專用封包解析器
/// - 支援解析動態價格欄位（揭示旗標）
/// =======================================
class TWSEFormat6Parser : public TWSEFormatParser {
private:
    std::shared_ptr<TWSEParsedFormat6Result> result;

public:
    /// 傳回封包格式名稱（"Format6"）
    std::string getFormatName() const override;

    /// 傳回解析後的封裝結果（TWSEParsedFormat6Result）
    std::shared_ptr<TWSEParsedResult> getParsedResult() const override;

    /// 主解析流程
    /// - 驗證長度
    /// - 解析固定區（TWSERealtimeQuoteFormat6Fixed）
    /// - 拆解揭示旗標 → 推導價格筆數
    /// - 將價格資料轉換為 deal/bid/ask 三類 vector
    bool parse(const uint8_t* data, size_t length) override;
};
