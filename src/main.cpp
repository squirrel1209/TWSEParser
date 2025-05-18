#include "controller/TWSEPacketParser.h"
#include "result/TWSEParsedFormat1Result.h"
#include "result/TWSEParsedFormat6Result.h"
#include "storage/TWSEDatabase.h"
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

int main() {
    // ✅ 開啟封包檔案（二進位讀取 + 獲取檔案大小）
    std::ifstream file("../data/Tse.bin", std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "❌ 無法開啟檔案 data/tse_06.bin\n";
        return 1;
    }

    // 讀取封包資料至 raw vector
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> raw(size);
    file.read(reinterpret_cast<char*>(raw.data()), size);

    std::cout << "✅ 讀取封包資料：" << raw.size() << " bytes\n";

    // ✅ 建立解析器並解析全部封包
    TWSEPacketParser parser(raw);
    parser.parseAll();

    // ✅ 取得解析結果（但不直接輸出）
    const auto& results = parser.getResults();
    std::cout << "📦 封包筆數：" << results.size() << "\n";

    // ✅ 匯入資料庫並輸出至文字檔
    TWSEDatabase db;
    db.load(results);
    db.writeToFile("output.txt");

    return 0;
}
