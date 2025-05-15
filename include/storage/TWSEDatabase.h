// TWSEDatabase.h
#pragma once
#include "result/TWSEParsedFormat1Result.h"
#include "result/TWSEParsedFormat6Result.h"
#include <unordered_map>
#include <vector>
#include <memory>

// ==========================================
// TWSEDatabase
// - 儲存所有成功解析的封包資料
// - 可查詢股票基本資料與成交紀錄
// ==========================================
class TWSEDatabase {
public:
    // 股票基本資料（格式一）
    std::unordered_map<FixedString<6>, TWSEStockBasicInfoFormat1> stockInfoMap;

    // 成交歷史紀錄（格式六）
    std::unordered_map<FixedString<6>, std::vector<TWSEFormat6PriceVolume>> dealHistory;

    /// 從解析結果載入所有資料
    void load(const std::vector<std::shared_ptr<TWSEParsedResult>>& results);
};