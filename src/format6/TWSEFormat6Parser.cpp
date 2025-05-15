#include "format6/TWSEFormat6Parser.h"
#include <cstring> // for std::memcpy

/// =======================================
/// getFormatName()
/// - 傳回 Format 名稱字串
/// =======================================
std::string TWSEFormat6Parser::getFormatName() const {
    return "Format6";
}

/// =======================================
/// getParsedResult()
/// - 傳回目前解析後的封裝結果
/// =======================================
std::shared_ptr<TWSEParsedResult> TWSEFormat6Parser::getParsedResult() const {
    return result;
}

/// =======================================
/// parse()
/// - 驗證資料長度
/// - 複製固定區段（38 bytes）
/// - 根據揭示旗標解出價格欄位筆數
/// - 將價格資料依序歸類到 deal / bid / ask
/// =======================================
bool TWSEFormat6Parser::parse(const uint8_t* data, size_t length) {
    if (length < sizeof(TWSERealtimeQuoteFormat6Fixed)) return false;

    result = std::make_shared<TWSEParsedFormat6Result>();
    std::memcpy(&result->fixed, data, sizeof(TWSERealtimeQuoteFormat6Fixed));

    // 🧠 揭示旗標位元解析
    uint8_t flag = result->fixed.body.revealFlag[0];
    int deal = (flag & 0b10000000) ? 1 : 0;        // bit 7
    int bid  = (flag >> 4) & 0b111;                // bits 4~6
    int ask  = (flag >> 1) & 0b111;                // bits 1~3
    int total = deal + bid + ask;

    // 🧮 計算預期封包大小（固定區 + N 筆價格 + XOR + Terminal）
    size_t offset = sizeof(TWSERealtimeQuoteFormat6Fixed);
    size_t expectedSize = offset + total * sizeof(TWSEFormat6PriceVolume) + 3;
    if (length < expectedSize) return false;

    const TWSEFormat6PriceVolume* prices =
        reinterpret_cast<const TWSEFormat6PriceVolume*>(data + offset);

    // 📤 拆出價格欄位
    int idx = 0;
    if (deal == 1) result->dealPrices.push_back(prices[idx++]);
    for (int i = 0; i < bid; ++i) result->bidPrices.push_back(prices[idx++]);
    for (int i = 0; i < ask; ++i) result->askPrices.push_back(prices[idx++]);

    return true;
}
