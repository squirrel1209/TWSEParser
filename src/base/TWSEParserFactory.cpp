#include "base/TWSEParserFactory.h"
#include "format1/TWSEFormat1Parser.h"
#include "format6/TWSEFormat6Parser.h"

/// =======================================
/// createParser()
/// - 傳入 formatCode（如 0x01, 0x06）
/// - 回傳對應的格式解析器（shared_ptr）
/// =======================================
std::shared_ptr<TWSEFormatParser> TWSEParserFactory::createParser(uint8_t formatCode) {
    switch (formatCode) {
        case 0x01:
            return std::make_shared<TWSEFormat1Parser>();  // ✅ Format1 股票基本資料
        case 0x06:
            return std::make_shared<TWSEFormat6Parser>();  // ✅ Format6 成交價資訊
        default:
            return nullptr;  // ❌ 未支援的格式代碼
    }
}
