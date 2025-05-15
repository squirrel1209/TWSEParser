// TWSEFormat1Parser.cpp
#include "format1/TWSEFormat1Parser.h"
#include <cstring>  // for std::memcpy

/// ========================================
/// getFormatName()
/// - 傳回格式字串 "Format1"
/// ========================================
std::string TWSEFormat1Parser::getFormatName() const {
    return "Format1";
}

/// ========================================
/// getParsedResult()
/// - 傳回儲存的解析結果物件
/// ========================================
std::shared_ptr<TWSEParsedResult> TWSEFormat1Parser::getParsedResult() const {
    return result;
}

/// ========================================
/// parse()
/// - 檢查長度、複製封包 → 執行封包完整性驗證
/// - 若任一驗證失敗則回傳 false
/// ========================================
bool TWSEFormat1Parser::parse(const uint8_t* data, size_t length) {
    if (length < sizeof(TWSEStockBasicInfoFormat1)) return false;

    result = std::make_shared<TWSEParsedFormat1Result>();
    std::memcpy(&result->packet, data, sizeof(TWSEStockBasicInfoFormat1));

    bool escOK, termOK, xorOK;
    return result->packet.verifyAll(escOK, termOK, xorOK);
}
