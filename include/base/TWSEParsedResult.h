#pragma once

// ========================================
// TWSEParsedResult 抽象基底類別
// ========================================
// 所有格式的解析結果都應繼承本類別
// 用於統一傳回解析結果類型（方便上層多型處理）
// - Format1 可傳回 TWSEParsedFormat1Result
// - Format6 可傳回 TWSEParsedFormat6Result
// - 未來支援更多格式只需繼承此類別
class TWSEParsedResult {
public:
    virtual ~TWSEParsedResult() = default;
};
