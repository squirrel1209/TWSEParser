// TWSEFormat6.cpp
#include "format6/TWSEFormat6.h"

/// =======================================
/// checkXor()
/// - 計算 XOR：從 messageLength 到 cumulativeVolume（含）
/// - 結果應與 XOR 欄位相同（緊接在 cumulativeVolume 後）
/// =======================================
bool TWSERealtimeQuoteFormat6Fixed::checkXor() const {
    const uint8_t* begin = reinterpret_cast<const uint8_t*>(&header.messageLength);
    const uint8_t* end   = reinterpret_cast<const uint8_t*>(&body.cumulativeVolume) + sizeof(BCD4);

    uint8_t xorValue = 0;
    for (const uint8_t* p = begin; p < end; ++p) {
        xorValue ^= *p;
    }

    const uint8_t* xorPtr = end;
    return xorValue == xorPtr[0];
}

/// =======================================
/// verifyEsc()
/// - 檢查封包是否以 0x1B 為起始
/// =======================================
bool TWSERealtimeQuoteFormat6Fixed::verifyEsc() const {
    return escCode[0] == 0x1B;
}

/// =======================================
/// verifyTerminal()
/// - 根據揭示旗標（revealFlag）計算價格筆數
/// - 進一步定位終止碼（0x0D 0x0A）是否正確
/// =======================================
bool TWSERealtimeQuoteFormat6Fixed::verifyTerminal() const {
    const uint8_t* termPtr = reinterpret_cast<const uint8_t*>(&body.cumulativeVolume) + sizeof(BCD4);

    // 👉 揭示旗標解析：成交筆數（bit 7），買/賣筆數為 3 位元
    uint8_t flag = body.revealFlag[0];
    int deal = (flag & 0b10000000) ? 1 : 0;        // bit 7
    int bid  = (flag >> 4) & 0b00000111;           // bits 4~6
    int ask  = (flag >> 1) & 0b00000111;           // bits 1~3
    int total = deal + bid + ask;                 // 價格筆數總和

    // 👉 價格欄位後面接著 XOR（1 byte）+ 終止符（0x0D 0x0A）
    const uint8_t* terminal = termPtr + sizeof(TWSEFormat6PriceVolume) * total + 1;
    return terminal[0] == 0x0D && terminal[1] == 0x0A;
}

/// =======================================
/// verifyAll()
/// - 同時檢查 ESC / XOR / Terminal 是否都正確
/// =======================================
bool TWSERealtimeQuoteFormat6Fixed::verifyAll(bool& escOK, bool& termOK, bool& xorOK) const {
    escOK  = verifyEsc();
    termOK = verifyTerminal();
    xorOK  = checkXor();
    return escOK && termOK && xorOK;
}

/// =======================================
/// verifySize()
/// - 檢查固定區段（不含價格）的大小是否為 38 bytes
/// =======================================
bool TWSERealtimeQuoteFormat6Fixed::verifySize() {
    return sizeof(TWSERealtimeQuoteFormat6Fixed) == 38;
}
