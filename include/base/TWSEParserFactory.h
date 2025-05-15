#pragma once
#include "TWSEFormatParser.h"
#include <memory>
#include <cstdint>

// Forward declarations（可選）
class TWSEFormat1Parser;
class TWSEFormat6Parser;

/// =======================================
/// TWSEParserFactory
/// - 工廠：依 formatCode 建立對應的 parser
/// =======================================
class TWSEParserFactory {
public:
    static std::shared_ptr<TWSEFormatParser> createParser(uint8_t formatCode);
};
