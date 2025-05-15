#ifndef TWSEFORMAT1_H
#define TWSEFORMAT1_H

#include <cstdint>
#include "common/TBCD.h"
#include "base/TWSEPacketBase.h"
#include "common/FixedArray.h"

// =======================================
// 固定長度欄位型別（ASCII）
// 使用 FixedArray 封裝 C-style 陣列字串
// =======================================
typedef FixedArray<char, 1>   Ascii1;
typedef FixedArray<char, 2>   Ascii2;
typedef FixedArray<char, 3>   Ascii3;
typedef FixedArray<char, 6>   Ascii6;
typedef FixedArray<char, 16>  Ascii16;

#pragma pack(push, 1) // 關閉 padding，自動對齊 byte
// ===============================
// Header 區段（Byte 2–10）
// ===============================
struct TWSEFormat1Header {
    BCD2 messageLength;    // 訊息總長度（預期為 114 bytes）
    BCD1 businessCode;     // 業務別（固定為 "01"）
    BCD1 formatCode;       // 傳輸格式（固定為 "01"）
    BCD1 versionCode;      // 版號（通常為 "09"）
    BCD4 sequenceNumber;   // 傳輸序號（可供排序）
};
#pragma pack(pop)

#pragma pack(push, 1) // 關閉 padding，自動對齊 byte
// ===============================
// 股票基本資料區（Byte 11–64）
// ===============================
struct TWSEFormat1StockInfo {
    Ascii6  stockCode;               // 股票代號
    Ascii16 stockName;               // 股票名稱
    Ascii2  industryCode;            // 產業別代碼
    Ascii2  securityCategory;        // 有價證券類別
    Ascii2  lotNote;                 // 單位註記
    BCD1    abnormalCode;            // 異常代碼
    Ascii1  boardType;               // 上市/上櫃別
    BCD5    refPrice;                // 參考價
    BCD5    upLimit;                 // 漲停價
    BCD5    downLimit;               // 跌停價
    Ascii1  nonParFlag;              // 非面額股票註記
    Ascii1  specialPromotionFlag;    // 特別處置註記
    Ascii1  abnormalSecurityFlag;    // 異常證券註記
    Ascii1  dayTradingFlag;          // 現股當沖註記
    Ascii1  shortSellExemptFlag;     // 無需借券註記
    Ascii1  borrowSellExemptFlag;    // 無需借券賣出註記
    BCD3    matchInterval;           // 撮合間隔秒數
};
#pragma pack(pop)

#pragma pack(push, 1) // 關閉 padding，自動對齊 byte
// ===============================
// 權證資訊區（Byte 65–103）
// ===============================
struct TWSEFormat1WarrantInfo {
    Ascii1 warrantId;               // 權證代號
    BCD5   strikePrice;             // 履約價
    BCD5   prevDayExerciseQty;      // 前日行使量
    BCD5   prevDayCancelQty;        // 前日註銷量
    BCD5   outstandingQty;          // 發行餘額
    BCD4   exerciseRatio;           // 行使比例
    BCD5   upperPrice;              // 履約上限價
    BCD5   lowerPrice;              // 履約下限價
    BCD4   expirationDate;          // 到期日（民國年）
};
#pragma pack(pop)

#pragma pack(push, 1) // 關閉 padding，自動對齊 byte
// ===============================
// 其他補充資訊（Byte 104–110）
// ===============================
struct TWSEFormat1OtherInfo {
    Ascii1 foreignStockFlag;       // 是否為外國股票
    BCD3   tradingUnit;            // 每單位股數
    Ascii3 currencyCode;           // 幣別（TWD, USD）
};
#pragma pack(pop)

#pragma pack(push, 1) // 關閉 padding，自動對齊 byte
// ===============================
// 封包主體（完整 114 Bytes）
// ===============================
struct TWSEStockBasicInfoFormat1 : public TWSEPacketBase {
    BCD1 escCode;                         // Byte 1: 起始碼（ESC = 0x1B）
    TWSEFormat1Header header;            // Byte 2–10
    TWSEFormat1StockInfo stockInfo;      // Byte 11–64
    TWSEFormat1WarrantInfo warrantInfo;  // Byte 65–103
    TWSEFormat1OtherInfo otherInfo;      // Byte 104–110
    BCD1 lineFlag;                        // Byte 111: 線路註記
    BCD1 xorCheck;                        // Byte 112: XOR 驗證位元
    BCD2 terminalCode;                    // Byte 113–114: 結尾（0x0D 0x0A）

    // ========= 驗證函式（已在 .cpp 實作） =========

    /// XOR 檢查：驗證 messageLength~lineFlag 是否符合 xorCheck
    bool checkXor() const override;

    /// 起始碼是否為 ESC (0x1B)
    bool verifyEsc() const override;

    /// 結尾是否為 0x0D 0x0A
    bool verifyTerminal() const override;

    /// 同時驗證 ESC、XOR、終止碼
    bool verifyAll(bool& escOK, bool& termOK, bool& xorOK) const override;

    /// 封包大小是否為 114 bytes（for runtime 檢查）
    static bool verifySize();
};

#pragma pack(pop)

// ✅ 編譯期檢查封包大小是否正確
static_assert(sizeof(TWSEStockBasicInfoFormat1) == 114, "TWSE format error: struct must be 114 bytes");

#endif // TWSEFORMAT1_H
