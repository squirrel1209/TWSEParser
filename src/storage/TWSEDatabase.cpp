// ==========================
// TWSEDatabase.cpp 補正錯誤版本
// ==========================
#include "storage/TWSEDatabase.h"
#include "common/EncodingUtils.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cfloat>

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


std::shared_ptr<TWSEStockBasicInfoFormat1> TWSEDatabase::queryFormat1ByStockCode(const std::string& code) const {
    FixedString<6> key(code.c_str());
    auto it = stockInfoMap.find(key);
    if (it != stockInfoMap.end()) {
        return std::make_shared<TWSEStockBasicInfoFormat1>(it->second);
    }
    return nullptr;
}

std::vector<std::shared_ptr<TWSEFormat6PriceVolume>> TWSEDatabase::queryFormat6ByStockCode(const std::string& code) const {
    FixedString<6> key(code.c_str());
    std::vector<std::shared_ptr<TWSEFormat6PriceVolume>> result;
    auto it = dealHistory.find(key);
    if (it != dealHistory.end()) {
        for (const auto& item : it->second) {
            result.push_back(std::make_shared<TWSEFormat6PriceVolume>(item));
        }
    }
    return result;
}

TWSEStockStats TWSEDatabase::summarizeStockStats(const std::string& code) const {
    FixedString<6> key(code.c_str());
    TWSEStockStats stats = {0.0, DBL_MAX, 0};
    auto it = dealHistory.find(key);
    if (it != dealHistory.end()) {
        for (const auto& d : it->second) {
            double p = d.price.toDouble(2);
            stats.maxPrice = std::max(stats.maxPrice, p);
            stats.minPrice = std::min(stats.minPrice, p);
            stats.totalVolume += d.quantity.toInt();
        }
    }
    if (stats.minPrice == DBL_MAX) stats.minPrice = 0.0;
    return stats;
}

void TWSEDatabase::writeToFile(const std::string& filepath) const {
    std::ofstream out(filepath);
    if (!out) {
        std::cerr << "❌ 無法寫入檔案: " << filepath << "\n";
        return;
    }
    out << "=== 股票基本資料 ===\n";
    for (const auto& [code, info] : stockInfoMap) {
        out << "股票代號: " << info.stockInfo.stockCode.toString()
            << " | 名稱: " << EncodingUtils::big5ToUtf8(info.stockInfo.stockName.toString())
            << " | 參考價: " << info.stockInfo.refPrice.toDouble(2)
            << " | 漲停: " << info.stockInfo.upLimit.toDouble(2)
            << " | 跌停: " << info.stockInfo.downLimit.toDouble(2)
            << "\n";
    }
    out << "\n=== 成交歷史紀錄 ===\n";
    for (const auto& [code, history] : dealHistory) {
        out << "股票代號: " << code.toString() << "\n";
        for (const auto& tick : history) {
            out << "  成交價: " << tick.price.toDouble(2)
                << " | 成交量: " << tick.quantity.toInt() << "\n";
        }
    }
    std::cout << "📝 資料寫入完成: " << filepath << "\n";
}

void TWSEDatabase::exportFormat6HistoryToCSV(const std::string& code, const std::string& filename) const {
    FixedString<6> key(code.c_str());
    auto it = dealHistory.find(key);
    if (it == dealHistory.end()) {
        std::cerr << "❌ 找不到股票代號: " << code << " 的成交資料\n";
        return;
    }

    // 移除代號右側空白，避免用於檔名產生問題
    std::string cleanCode = code;
    cleanCode.erase(cleanCode.find_last_not_of(' ') + 1);

    std::ofstream out(cleanCode + "_history.csv", std::ios::binary);
    if (!out) {
        std::cerr << "❌ 無法寫入檔案: " << cleanCode << "_history.csv\n";
        return;
    }
    out << "\xEF\xBB\xBF"; // UTF-8 BOM

    // 標題列
    out << "股票代號,股票名稱,價格,張數\n";

    // 取得股票名稱（若有）
    std::string stockNameUtf8 = "";
    auto info = queryFormat1ByStockCode(code);
    if (info) {
        stockNameUtf8 = EncodingUtils::big5ToUtf8(info->stockInfo.stockName.toString());
    }

    // 寫入每筆成交紀錄
    for (const auto& tick : it->second) {
        out << cleanCode << ","
            << stockNameUtf8 << ","
            << std::fixed << std::setprecision(2) << tick.price.toDouble(2) << ","
            << tick.quantity.toInt() << "\n";
    }

    std::cout << "📄 已輸出 CSV: " << cleanCode << "_history.csv\n";
}
