#ifndef TWSEFORMAT1_H
#define TWSEFORMAT1_H

#include <cstdint>
#include "common/TBCD.h"
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

#pragma pack(push, 1)
// ===============================
// Header 區段（Byte 2–10）
// ===============================
struct TWSEFormat1Header {
    BCD2 messageLength;     // 封包長度（預期為 114 bytes）
    BCD1 businessCode;      // 業務別（固定為 "01"）
    BCD1 formatCode;        // 格式代碼（固定為 "01"）
    BCD1 versionCode;       // 格式版本（如 "09"）
    BCD4 sequenceNumber;    // 傳輸序號
};
#pragma pack(pop)

#pragma pack(push, 1)
// ===============================
// 股票基本資料區（Byte 11–64）
// ===============================
struct TWSEFormat1StockInfo {
    Ascii6  stockCode;              // 股票代號
    Ascii16 stockName;              // 股票名稱
    Ascii2  industryCode;           // 產業別代碼
    Ascii2  securityCategory;       // 有價證券類別
    Ascii2  lotNote;                // 單位註記
    BCD1    abnormalCode;           // 異常代碼
    Ascii1  boardType;              // 上市/上櫃別
    BCD5    refPrice;               // 參考價
    BCD5    upLimit;                // 漲停價
    BCD5    downLimit;              // 跌停價
    Ascii1  nonParFlag;             // 非面額股票註記
    Ascii1  specialPromotionFlag;   // 特別處置註記
    Ascii1  abnormalSecurityFlag;   // 異常證券註記
    Ascii1  dayTradingFlag;         // 現股當沖註記
    Ascii1  shortSellExemptFlag;    // 無需借券註記
    Ascii1  borrowSellExemptFlag;   // 無需借券賣出註記
    BCD3    matchInterval;          // 撮合間隔（秒）
};
#pragma pack(pop)

#pragma pack(push, 1)
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

#pragma pack(push, 1)
// ===============================
// 補充資訊區（Byte 104–110）
// ===============================
struct TWSEFormat1OtherInfo {
    Ascii1 foreignStockFlag;        // 是否為外國股票
    BCD3   tradingUnit;             // 每單位股數
    Ascii3 currencyCode;            // 幣別（TWD、USD 等）
};
#pragma pack(pop)

#pragma pack(push, 1)
// ===============================
// 封包主體（完整封包：114 bytes）
// ===============================
struct TWSEStockBasicInfoFormat1 {
    BCD1 escCode;                          // Byte 1：封包起始碼（0x1B）
    TWSEFormat1Header header;             // Byte 2–10
    TWSEFormat1StockInfo stockInfo;       // Byte 11–64
    TWSEFormat1WarrantInfo warrantInfo;   // Byte 65–103
    TWSEFormat1OtherInfo otherInfo;       // Byte 104–110
    BCD1 lineFlag;                         // Byte 111：線路註記
    BCD1 xorCheck;                         // Byte 112：XOR 驗證值
    BCD2 terminalCode;                     // Byte 113–114：封包結尾（0x0D 0x0A）

    // ✅ 封包驗證函式（.cpp 實作）
    bool checkXor() const;
    bool verifyEsc() const;
    bool verifyTerminal() const;
    bool verifyAll(bool& escOK, bool& termOK, bool& xorOK) const;

    /// ✅ 可用於 runtime 封包長度驗證
    static bool verifySize();
};
#pragma pack(pop)

// ✅ 編譯期驗證封包長度：應等於 114 bytes
static_assert(sizeof(TWSEStockBasicInfoFormat1) == 114,
              "❌ TWSEFormat1 錯誤：封包長度應為 114 bytes，請檢查欄位定義或對齊設定");

#endif // TWSEFORMAT1_H
