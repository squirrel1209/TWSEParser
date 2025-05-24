#include "controller/TWSEPacketParser.h"
#include "storage/TWSEDatabase.h"
#include "result/TWSEParsedFormat1Result.h"
#include "result/TWSEParsedFormat6Result.h"
#include "common/EncodingUtils.h"

#include <fstream>
#include <vector>
#include <memory>
#include <iostream>
#include <string>

int main() {
    // 讀取封包檔案
    std::ifstream file("../data/Tse.bin", std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "❌ 無法開啟檔案 ../data/Tse.bin\n";
        return 1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> raw(size);
    file.read(reinterpret_cast<char*>(raw.data()), size);
    std::cout << "✅ 封包讀取完成，大小：" << raw.size() << " bytes\n";

    // 封包解析
    TWSEPacketParser parser(raw);
    parser.parseAll();
    const auto& results = parser.getResults();
    std::cout << "📦 封包數量：" << results.size() << " 筆\n";

    // 載入資料庫
    TWSEDatabase db;
    db.load(results);

    // 輸入股票代號並補滿為 6 碼
    std::string inputCode;
    std::cout << "🔍 請輸入股票代號（如 2330）: ";
    std::cin >> inputCode;
    std::string paddedCode = EncodingUtils::padStockCode(inputCode);

    // 查詢 Format1 資料
    auto info = db.queryFormat1ByStockCode(paddedCode);
    if (info) {
        std::cout << "\n📄 股票基本資料 (Format1)\n";
        std::cout << "股票代號: " << info->stockInfo.stockCode.toString() << "\n";
        std::cout << "股票名稱: " << EncodingUtils::big5ToUtf8(info->stockInfo.stockName.toString()) << "\n";
        std::cout << "參考價: "   << info->stockInfo.refPrice.toDouble(2) << "\n";
        std::cout << "漲停價: "   << info->stockInfo.upLimit.toDouble(2) << "\n";
        std::cout << "跌停價: "   << info->stockInfo.downLimit.toDouble(2) << "\n";
    } else {
        std::cout << "❌ 找不到股票基本資料 (Format1)\n";
    }

    // 查詢 Format6 資料
    auto ticks = db.queryFormat6ByStockCode(paddedCode);
    if (!ticks.empty()) {
        std::cout << "\n📊 成交歷程 (Format6)，共 " << ticks.size() << " 筆\n";
        const auto& latest = ticks.back();
        std::cout << "，價格 " << latest->price.toDouble(2)
          << "，數量 " << latest->quantity.toInt() << "\n";
    } else {
        std::cout << "❌ 找不到成交資料 (Format6)\n";
    }

    // 輸出統計資訊
    auto stats = db.summarizeStockStats(paddedCode);
    std::cout << "\n📈 成交統計\n";
    std::cout << "最高價: " << stats.maxPrice
              << " | 最低價: " << stats.minPrice
              << " | 總成交量: " << stats.totalVolume << "\n";

    // 匯出成交歷程為 CSV（透過 TWSEDatabase 方法，內含 UTF-8 BOM）
    std::string outPath = paddedCode + "_history.csv";
    db.exportFormat6HistoryToCSV(paddedCode, outPath);

    return 0;
}
