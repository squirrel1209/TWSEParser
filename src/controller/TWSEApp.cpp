// TWSEApp.cpp
#include "controller/TWSEApp.h"
#include "controller/TWSEPacketParser.h"
#include "storage/TWSEDatabase.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>

/// =======================================
/// run()
/// - 讀取封包檔案 → 呼叫解析器 → 將結果載入資料庫
/// =======================================
void TWSEApp::run(const std::string& filename) {
    // 嘗試開啟二進位封包檔案
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "❌ 無法開啟檔案: " << filename << "\n";
        return;
    }

    // 預先抓取檔案大小，並重新定位到起點讀取資料
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> raw(size);
    file.read(reinterpret_cast<char*>(raw.data()), size);

    std::cout << "📦 載入封包資料: " << raw.size() << " bytes\n";

    // 解析封包資料
    TWSEPacketParser parser(raw);
    parser.parseAll();

    // 將解析後資料存入內部資料庫結構
    TWSEDatabase db;
    db.load(parser.getResults());

    std::cout << "✅ 資料處理完成\n";
}
