// TWSEPacketParser.cpp
#include "controller/TWSEPacketParser.h"
#include "base/TWSEParserFactory.h"
#include <iomanip>
#include <iostream>


void TWSEPacketParser::parseAll() {
    size_t offset = 0;
    int index = 0;

    while (offset + 3 <= rawData.size()) {
        // 搜尋封包起始碼 (0x1B)
        if (rawData[offset] != 0x1B) {
            ++offset;
            continue;
        }

        // 抓取封包長度（第 1~2 bytes 為 BCD 格式）
        if (offset + 3 > rawData.size()) break;
        uint8_t high = rawData[offset + 1];
        uint8_t low  = rawData[offset + 2];
        int len = ((high >> 4) & 0xF) * 1000 + (high & 0xF) * 100
                + ((low >> 4) & 0xF) * 10 + (low & 0xF);

        if (offset + len > rawData.size()) break;

        // 抓 formatCode
        uint8_t formatCode = rawData[offset + 4];
        auto parser = TWSEParserFactory::createParser(formatCode);

        if (parser && parser->parse(rawData.data() + offset, len)) {
            results.push_back(parser->getParsedResult());
            std::cout << "✅ Format " << parser->getFormatName()
                      << " 解析成功, Offset: " << offset
                      << ", 長度: " << len << "\n";
        } else {
            std::cout << "❌ 解析失敗, Offset: " << offset
                      << ", FormatCode: 0x" << std::hex << std::setw(2)
                      << std::setfill('0') << static_cast<int>(formatCode) << std::dec << "\n";
        }

        offset += len;
    }
}

const std::vector<std::shared_ptr<TWSEParsedResult>>& TWSEPacketParser::getResults() const {
    return results;
}
