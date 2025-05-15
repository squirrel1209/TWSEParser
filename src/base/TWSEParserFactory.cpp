// TWSEParserFactory.cpp
#include "base/TWSEParserFactory.h"
#include "base/TWSEFormatParser.h"

// ✅ 各格式對應解析器（新增格式時請於此處註冊）
#include "format1/TWSEFormat1Parser.h"
#include "format6/TWSEFormat6Parser.h"

/// =======================================
/// TWSEParserFactory::createParser()
/// - 根據 formatCode 建立對應格式解析器
/// =======================================
std::shared_ptr<TWSEFormatParser> TWSEParserFactory::createParser(uint8_t formatCode) {
    switch (formatCode) {
        case 0x01:
            return std::make_shared<TWSEFormat1Parser>();
        case 0x06:
            return std::make_shared<TWSEFormat6Parser>();
        // 🆕 TODO: 若有 Format2 / Format4，請在此擴充
        default:
            return nullptr;
    }
}
