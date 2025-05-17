#include "controller/TWSEPacketParser.h"
#include "result/TWSEParsedFormat1Result.h"
#include "result/TWSEParsedFormat6Result.h"
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

int main() {
    // ✅ 讀取測試封包檔
    std::ifstream file("../data/tse_06.bin", std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "❌ 無法開啟檔案 data/Tse.bin\n";
        return 1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> raw(size);
    file.read(reinterpret_cast<char*>(raw.data()), size);

    std::cout << "✅ 讀取封包資料：" << raw.size() << " bytes\n";

    // ✅ 封包解析（支援 Format1、6）
    TWSEPacketParser parser(raw);
    parser.parseAll();

    const auto& results = parser.getResults();
    std::cout << "📦 封包筆數：" << results.size() << "\n";

    // ✅ 遍歷封包結果
    for (const auto& result : results) {
        if (auto f1 = std::dynamic_pointer_cast<TWSEParsedFormat1Result>(result)) {
            std::cout << "[Format1] 股票代號: " << f1->packet.stockInfo.stockCode.toString() << "\n";
        }
        else if (auto f6 = std::dynamic_pointer_cast<TWSEParsedFormat6Result>(result)) {
            std::cout << "[Format6] 成交: " << f6->dealPrices.size()
              << "，買進: " << f6->bidPrices.size()
              << "，賣出: " << f6->askPrices.size() << "\n";

        }
        else {
            std::cout << "[Unknown Format]\n";
        }
    }

    return 0;
}
