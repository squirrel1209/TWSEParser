// TWSEParsedFormat1Result.h
#pragma once
#include "TWSEParsedResult.h"
#include "TWSEFormat1.h"

// ========================================
// Format1 專用的解析結果封裝
// - 僅包含完整封包內容（114 Bytes）
// - 提供 TWSEStockBasicInfoFormat1 結構
// ========================================
class TWSEParsedFormat1Result : public TWSEParsedResult {
public:
    TWSEStockBasicInfoFormat1 packet;
};