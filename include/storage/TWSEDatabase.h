#pragma once
#include "result/TWSEParsedFormat1Result.h"
#include "result/TWSEParsedFormat6Result.h"
#include "common/FixedString.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

// ===============================
// TWSEDatabase：集中儲存所有封包解析結果
// - 包含 Format1 的股票基本資料
// - 包含 Format6 的成交歷史紀錄
// ===============================
class TWSEDatabase {
public:
    // 股票基本資料（Format1 封包對應）
    std::unordered_map<FixedString<6>, TWSEStockBasicInfoFormat1> stockInfoMap;

    // 成交歷史紀錄（Format6 封包對應）
    std::unordered_map<FixedString<6>, std::vector<TWSEFormat6PriceVolume>> dealHistory;

    /// 從解析結果中載入資料（Format1 + Format6）
    void load(const std::vector<std::shared_ptr<TWSEParsedResult>>& results);

    /// 將儲存的資料輸出成文字檔
    void writeToFile(const std::string& filepath) const;
};
