// TWSEPacketParser.cpp
#include "controller/TWSEPacketParser.h"
#include "base/TWSEParserFactory.h"
#include <iomanip>
#include <iostream>


void TWSEPacketParser::parseAll() {
    size_t offset = 0;
    int index = 1;

    std::cout << "📦 [解析開始] 原始資料大小：" << rawData.size() << " bytes\n";

    while (offset + 3 <= rawData.size()) {
        // Step 1: 找起始碼
        if (rawData[offset] != 0x1B) {
            ++offset;
            continue;
        }

        // Step 2: 抓 BCD 封包長度
        uint8_t high = rawData[offset + 1];
        uint8_t low  = rawData[offset + 2];
        int len = ((high >> 4) & 0xF) * 1000 + (high & 0xF) * 100
                + ((low >> 4) & 0xF) * 10 + (low & 0xF);

        if (offset + len > rawData.size()) {
            std::cout << "⚠️ 封包超出資料尾端，Offset: " << offset << ", Len: " << len << "\n";
            break;
        }

        // Step 3: 格式碼 FormatCode
        uint8_t formatCode = rawData[offset + 4];

        std::cout << "\n🟡 嘗試解析封包 #" << index
                  << " | Offset: " << offset
                  << " | Length: " << len
                  << " | FormatCode: 0x" << std::hex << std::setw(2)
                  << std::setfill('0') << static_cast<int>(formatCode) << std::dec << "\n";

        // Step 4: 結尾符檢查
        if (rawData[offset + len - 2] != 0x0D || rawData[offset + len - 1] != 0x0A) {
            std::cout << "❌ 封包結尾不合法（預期 0x0D 0x0A）\n";
            ++offset;
            continue;
        }

        // Step 5: 建立解析器
        auto parser = TWSEParserFactory::createParser(formatCode);
        if (!parser) {
            std::cout << "❌ 無法建立對應 Parser（格式不支援或長度錯）\n";
            ++offset;
            continue;
        }

        // Step 6: 執行解析
        if (parser->parse(rawData.data() + offset, len)) {
            results.push_back(parser->getParsedResult());
            std::cout << "✅ Format " << parser->getFormatName()
                      << " 解析成功, Offset: " << offset
                      << ", 長度: " << len << "\n";
            ++index;
            offset += len;
        } 
        
        else {
            std::cout << "❌ 解析失敗（Parser 拒絕）\n";

            // 額外印出封包開頭 bytes 方便 debug
            std::cout << "🔍 封包前 8 bytes: ";
            for (int i = 0; i < 8 && offset + i < rawData.size(); ++i)
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << static_cast<int>(rawData[offset + i]) << " ";
            std::cout << std::dec << "\n";

            ++offset; // 小步前進，避免錯位連錯
        }
    }

    if (offset < rawData.size()) {
        std::cout << "\n[解析結束] 剩餘未處理資料：" << (rawData.size() - offset) << " bytes\n";
    } else {
        std::cout << "\n[解析結束] 所有封包解析完畢\n";
    }
}

const std::vector<std::shared_ptr<TWSEParsedResult>>& TWSEPacketParser::getResults() const {
    return results;
}
