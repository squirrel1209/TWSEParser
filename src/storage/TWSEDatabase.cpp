#include "storage/TWSEDatabase.h"
#include "common/EncodingUtils.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void TWSEDatabase::load(const std::vector<std::shared_ptr<TWSEParsedResult>>& results) {
    for (const auto& res : results) {
        if (auto f1 = std::dynamic_pointer_cast<TWSEParsedFormat1Result>(res)) {
            FixedString<6> code(f1->packet.stockInfo.stockCode.data());
            stockInfoMap[code] = f1->packet;
        }
        else if (auto f6 = std::dynamic_pointer_cast<TWSEParsedFormat6Result>(res)) {
            FixedString<6> code(f6->fixed.body.stockCode.data());
            for (const auto& tick : f6->dealPrices) {
                dealHistory[code].push_back(tick);
            }
        }
    }

    std::cout << "📁 匯入股票基本資料: " << stockInfoMap.size() << " 檔\n";
    std::cout << "📈 有成交紀錄的股票數量: " << dealHistory.size() << " 檔\n";
}

void TWSEDatabase::writeToFile(const std::string& filepath) const {
    std::ofstream out(filepath);
    if (!out) {
        std::cerr << "❌ 無法寫入檔案: " << filepath << "\n";
        return;
    }

    // 輸出 Format1 股票基本資料
    out << "=== 股票基本資料 ===\n";
    for (const auto& [code, info] : stockInfoMap) {
        out << "股票代號: " << info.stockInfo.stockCode.toString()
            << " | 名稱: " << EncodingUtils::big5ToUtf8(info.stockInfo.stockName.toString())
            << " | 參考價: " << info.stockInfo.refPrice.toDouble(2)
            << " | 漲停: " << info.stockInfo.upLimit.toDouble(2)
            << " | 跌停: " << info.stockInfo.downLimit.toDouble(2)
            << "\n";
    }

    // 輸出 Format6 成交紀錄
    out << "\n=== 成交歷史紀錄 ===\n";
    for (const auto& [code, history] : dealHistory) {
        out << "股票代號: " << code.toString() << "\n";
        for (const auto& tick : history) {
            out << "  成交價: " << tick.price.toDouble(2)
                << " | 成交量: " << tick.quantity.toInt()  // 根據 TWSEFormat6.h
                << "\n";
        }
    }

    std::cout << "📝 資料寫入完成: " << filepath << "\n";
}
