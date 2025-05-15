// TWSEDatabase.cpp
#include "storage/TWSEDatabase.h"
#include <iostream>

/// =======================================
/// TWSEDatabase::load()
/// - 將解析後的 TWSEParsedResult 資料分類入庫
/// - 支援 Format1（股票基本資料）
/// - 支援 Format6（成交價格記錄）
/// =======================================
void TWSEDatabase::load(const std::vector<std::shared_ptr<TWSEParsedResult>>& results) {
    for (const auto& res : results) {
        // 🟡 Format1：基本股票資料
        if (auto f1 = std::dynamic_pointer_cast<TWSEParsedFormat1Result>(res)) {
            const auto& code = f1->packet.stockInfo.stockCode;
            stockInfoMap[code] = f1->packet;
        }

        // 🔴 Format6：成交價資料（僅記錄 deal）
        else if (auto f6 = std::dynamic_pointer_cast<TWSEParsedFormat6Result>(res)) {
            const auto& code = f6->fixed.body.stockCode;
            for (const auto& tick : f6->dealPrices) {
                dealHistory[code].push_back(tick);
            }
        }
    }

    // ✅ 匯入統計
    std::cout << "📁 匯入股票基本資料: " << stockInfoMap.size() << " 檔\n";
    std::cout << "📈 有成交紀錄的股票數量: " << dealHistory.size() << " 檔\n";
}
