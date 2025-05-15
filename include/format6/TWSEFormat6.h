#ifndef TWSEFORMAT6_H
#define TWSEFORMAT6_H

#include <cstdint>
#include "common/TBCD.h"
#include "base/TWSEPacketBase.h"
#include "common/FixedArray.h"

/// 固定長度 ASCII 欄位型別
typedef FixedArray<char, 6> Ascii6;

#pragma pack(push, 1)  // 關閉自動對齊，確保 byte-level 對齊

// =======================================
// 格式六：標頭區段（總長度：9 bytes）
// =======================================
struct TWSEFormat6Header {
    BCD2 messageLength;     // 2 bytes：封包長度（含 XOR 與終止碼）
    BCD1 businessCode;      // 1 byte：業務別（固定）
    BCD1 formatCode;        // 1 byte：格式代號（"06"）
    BCD1 versionCode;       // 1 byte：版本號（通常為 "09"）
    BCD4 sequenceNumber;    // 4 bytes：流水編號
};

// =======================================
// 格式六：成交主體資訊（總長度：28 bytes）
// =======================================
struct TWSEFormat6Body {
    Ascii6 stockCode;       // 6 bytes：股票代號
    BCD6   matchTime;       // 6 bytes：撮合時間（HHMMSS）
    BCD1   revealFlag;      // 1 byte：揭示旗標（含成交/買/賣筆數資訊）
    BCD1   limitFlag;       // 1 byte：漲跌停註記
    BCD1   statusFlag;      // 1 byte：交易狀態（開盤、暫停、收盤等）
    BCD4   cumulativeVolume;// 4 bytes：累積成交量（單位：張）
};

// =======================================
// 格式六：價格/張數對組（動態 N 筆）
// =======================================
struct TWSEFormat6PriceVolume {
    BCD5 price;             // 5 bytes：價格（含兩位小數）
    BCD4 quantity;          // 4 bytes：張數
};

// =======================================
// 格式六：固定封包區（ESC ~ Volume 為止，共 38 bytes）
// - 後續動態區域為 PriceVolume + XOR + Terminal
// =======================================
struct TWSERealtimeQuoteFormat6Fixed : public TWSEPacketBase {
    BCD1 escCode;                  // 1 byte：封包起始符（應為 0x1B）
    TWSEFormat6Header header;     // 9 bytes：封包標頭
    TWSEFormat6Body body;         // 28 bytes：封包主體資訊

    // ✅ 封包驗證函式（實作請見 TWSEFormat6.cpp）
    
    /// 驗證 XOR 是否正確（從 messageLength ~ cumulativeVolume 為止）
    bool checkXor() const override;

    /// 驗證 ESC 碼是否為 0x1B（封包開頭）
    bool verifyEsc() const override;

    /// 驗證封包終止碼（解析 revealFlag 決定價格筆數，推導出終止位置）
    bool verifyTerminal() const override;

    /// 綜合驗證封包（ESC + XOR + Terminal）
    bool verifyAll(bool& escOK, bool& termOK, bool& xorOK) const override;

    /// 驗證固定區段大小是否正確（應為 38 bytes）
    static bool verifySize();
};

#pragma pack(pop)

// ✅ 編譯期大小驗證：固定區段必須為 38 bytes
static_assert(sizeof(TWSERealtimeQuoteFormat6Fixed) == 38, "Format6 fixed section must be 38 bytes");

#endif // TWSEFORMAT6_H
