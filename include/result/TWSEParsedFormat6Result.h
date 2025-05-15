// TWSEParsedFormat6Result.h
#pragma once
#include "TWSEParsedResult.h"
#include "TWSEFormat6.h"
#include <vector>

// ================================
// Format6 專用的解析結果封裝
// ================================
class TWSEParsedFormat6Result : public TWSEParsedResult {
public:
    TWSERealtimeQuoteFormat6Fixed fixed;  // 固定區段（38 bytes）
    std::vector<TWSEFormat6PriceVolume> dealPrices;
    std::vector<TWSEFormat6PriceVolume> bidPrices;
    std::vector<TWSEFormat6PriceVolume> askPrices;
};