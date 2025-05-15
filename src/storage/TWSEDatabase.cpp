// TWSEDatabase.cpp
#include "storage/TWSEDatabase.h"
#include <iostream>

void TWSEDatabase::load(const std::vector<std::shared_ptr<TWSEParsedResult>>& results) {
    for (const auto& res : results) {
        // 🟡 Format1：基本股票資料
        if (auto f1 = std::dynamic_pointer_cast<TWSEParsedFormat1Result>(res)) {
            FixedString<6> code(f1->packet.stockInfo.stockCode.data());  // ✅ 修正
            stockInfoMap[code] = f1->packet;
        }

        // 🔴 Format6：成交價資料
        else if (auto f6 = std::dynamic_pointer_cast<TWSEParsedFormat6Result>(res)) {
            FixedString<6> code(f6->fixed.body.stockCode.data());  // ✅ 修正
            for (const auto& tick : f6->dealPrices) {
                dealHistory[code].push_back(tick);
            }
        }
    }

    std::cout << "📁 匯入股票基本資料: " << stockInfoMap.size() << " 檔\n";
    std::cout << "📈 有成交紀錄的股票數量: " << dealHistory.size() << " 檔\n";
}
