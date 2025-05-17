#include <gtest/gtest.h>
#include "controller/TWSEPacketParser.h"
#include "result/TWSEParsedFormat1Result.h"
#include <fstream>
#include <vector>
#include <memory>
#include <filesystem>

TEST(Format1ParserTest, ParseFirstFormat1FromStream) {
    // ✅ 載入整個封包串流
    std::ifstream file("../data/Tse.bin", std::ios::binary | std::ios::ate);
    ASSERT_TRUE(file.is_open()) << "❌ 無法開啟檔案";

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> raw(size);
    file.read(reinterpret_cast<char*>(raw.data()), size);

    // ✅ 使用 TWSEPacketParser 切封包（支援 format1/format6）
    TWSEPacketParser packetParser(raw);
    packetParser.parseAll();

    const auto& packets = packetParser.getResults();

    ASSERT_FALSE(packets.empty()) << "❌ 沒有解析出任何封包";

    // ✅ 抓出第一個 Format1 封包
    for (const auto& result : packets) {
        auto f1 = std::dynamic_pointer_cast<TWSEParsedFormat1Result>(result);
        if (f1) {
            // ✅ 確認股票代號是你預期的（例如 1101）
            EXPECT_EQ(f1->packet.stockInfo.stockCode.toString(), "2330");

            // ✅ 寫出結果方便 debug
            std::filesystem::create_directories("output");
            std::ofstream out("output/format1_result.txt");
            out << "股票代號: " << f1->packet.stockInfo.stockCode.toString() << "\n";
            out << "參考價  : " << f1->packet.stockInfo.refPrice.toDouble(2) << "\n";
            out << "漲停價  : " << f1->packet.stockInfo.upLimit.toDouble(2) << "\n";
            out << "跌停價  : " << f1->packet.stockInfo.downLimit.toDouble(2) << "\n";
            return;
        }
    }

    FAIL() << "❌ 找不到 Format1 封包";
}
