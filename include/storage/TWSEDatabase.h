// ==========================
// TWSEDatabase.h 補完版本（含完整註解）
// ==========================
#pragma once
#include "result/TWSEParsedFormat1Result.h"
#include "result/TWSEParsedFormat6Result.h"
#include "common/FixedString.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

// 股票統計資訊結構：包含最高價、最低價、總成交量
struct TWSEStockStats {
    double maxPrice;       // 該股票當日最高成交價
    double minPrice;       // 該股票當日最低成交價
    uint64_t totalVolume;  // 該股票總成交量（單位：股）
};

// TWSEDatabase：集中管理 Format1 與 Format6 解析結果
class TWSEDatabase {
public:
    std::unordered_map<FixedString<6>, TWSEStockBasicInfoFormat1> stockInfoMap;
    std::unordered_map<FixedString<6>, std::vector<TWSEFormat6PriceVolume>> dealHistory;

    void load(const std::vector<std::shared_ptr<TWSEParsedResult>>& results);
    void writeToFile(const std::string& filepath) const;

    std::shared_ptr<TWSEStockBasicInfoFormat1> queryFormat1ByStockCode(const std::string& code) const;
    std::vector<std::shared_ptr<TWSEFormat6PriceVolume>> queryFormat6ByStockCode(const std::string& code) const;
    TWSEStockStats summarizeStockStats(const std::string& code) const;
    void exportFormat6HistoryToCSV(const std::string& code, const std::string& filename) const;
};