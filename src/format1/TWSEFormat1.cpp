// TWSEFormat1.cpp
#include "format1/TWSEFormat1.h"

/// =======================================
/// checkXor()
/// - 從 header.messageLength 到 lineFlag 的所有 byte 進行 XOR
/// - 結果應等於 xorCheck 欄位
/// =======================================
bool TWSEStockBasicInfoFormat1::checkXor() const {
    const uint8_t* begin = reinterpret_cast<const uint8_t*>(&header.messageLength);
    const uint8_t* end   = reinterpret_cast<const uint8_t*>(&lineFlag);
    uint8_t xorValue = 0;
    for (const uint8_t* p = begin; p <= end; ++p) {
        xorValue ^= *p;
    }
    return xorValue == xorCheck[0];
}

/// =======================================
/// verifyEsc()
/// - 確認封包起始是否為 ESC (0x1B)
/// =======================================
bool TWSEStockBasicInfoFormat1::verifyEsc() const {
    return escCode[0] == 0x1B;
}

/// =======================================
/// verifyTerminal()
/// - 確認封包結尾是否為 0x0D 0x0A
/// =======================================
bool TWSEStockBasicInfoFormat1::verifyTerminal() const {
    return terminalCode[0] == 0x0D && terminalCode[1] == 0x0A;
}

/// =======================================
/// verifyAll()
/// - 同時驗證 ESC、XOR、結尾是否正確
/// - 回傳三項結果與總驗證結果
/// =======================================
bool TWSEStockBasicInfoFormat1::verifyAll(bool& escOK, bool& termOK, bool& xorOK) const {
    escOK  = verifyEsc();
    termOK = verifyTerminal();
    xorOK  = checkXor();
    return escOK && termOK && xorOK;
}

/// =======================================
/// verifySize()
/// - 編譯期檢查整體結構大小是否為 114 bytes
/// - 提供測試或單元驗證使用
/// =======================================
bool TWSEStockBasicInfoFormat1::verifySize() {
    return sizeof(TWSEStockBasicInfoFormat1) == 114;
}
