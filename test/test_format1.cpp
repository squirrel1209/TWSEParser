#include <gtest/gtest.h>
#include "format1/TWSEFormat1.h"
#include "format1/TWSEFormat1Parser.h"
#include "result/TWSEParsedFormat1Result.h"
#include <cstring>
#include <cstdint>
#include <memory>

/// ✅ 建立一個合法的 Format1 封包（大小為 114 bytes）
TWSEStockBasicInfoFormat1 createValidPacket() {
    TWSEStockBasicInfoFormat1 packet{};
    packet.escCode[0] = 0x1B;

    // 設定 Header 資訊
    packet.header.messageLength = BCD2{0x01, 0x14}; // 0x0114 → 114
    packet.header.businessCode = BCD1{0x01};
    packet.header.formatCode   = BCD1{0x01};
    packet.header.versionCode  = BCD1{0x09};
    packet.header.sequenceNumber = BCD4{0x00, 0x00, 0x00, 0x01};

    // 股票代號 "1101" + 2 個空白（共 6 字元）
    std::memcpy(packet.stockInfo.stockCode.data, "1101  ", 6);

    // 其他欄位留空或補 0，視結構預設而定（已由 constructor 清 0）

    // LineFlag 設定為 0
    packet.lineFlag[0] = 0x00;

    // XOR 計算（從 header.messageLength 到 lineFlag）
    const uint8_t* begin = reinterpret_cast<const uint8_t*>(&packet.header.messageLength);
    const uint8_t* end   = reinterpret_cast<const uint8_t*>(&packet.lineFlag);
    uint8_t xorValue = 0;
    for (const uint8_t* p = begin; p <= end; ++p) {
        xorValue ^= *p;
    }
    packet.xorCheck[0] = xorValue;

    // 結尾終止碼
    packet.terminalCode[0] = 0x0D;
    packet.terminalCode[1] = 0x0A;

    return packet;
}

// ✅ 測試封包 verifyAll() 三項驗證都通過
TEST(Format1StructTest, VerifyAllPassesOnValidPacket) {
    auto packet = createValidPacket();
    bool escOK = false, xorOK = false, termOK = false;

    EXPECT_TRUE(packet.verifyAll(escOK, termOK, xorOK));
    EXPECT_TRUE(escOK);
    EXPECT_TRUE(xorOK);
    EXPECT_TRUE(termOK);
}

// ✅ 測試 parser 可成功解析封包並轉換為解析結果
TEST(Format1ParserTest, ValidPacketParsesSuccessfully) {
    auto packet = createValidPacket();
    uint8_t raw[114];
    std::memcpy(raw, &packet, sizeof(packet));

    TWSEFormat1Parser parser;
    bool ok = parser.parse(raw, sizeof(raw));
    EXPECT_TRUE(ok);

    auto result = std::dynamic_pointer_cast<TWSEParsedFormat1Result>(parser.getParsedResult());
    ASSERT_TRUE(result);
    EXPECT_EQ(result->packet.stockInfo.stockCode.toString(), "1101");
}

// ✅ 測試 XOR 破壞後 parser 無法通過
TEST(Format1ParserTest, InvalidXORFails) {
    auto packet = createValidPacket();
    packet.xorCheck[0] ^= 0xFF; // 故意破壞 XOR

    uint8_t raw[114];
    std::memcpy(raw, &packet, sizeof(packet));

    TWSEFormat1Parser parser;
    EXPECT_FALSE(parser.parse(raw, sizeof(raw)));
}

// ✅ 測試 TERMINAL 改錯無法通過
TEST(Format1ParserTest, InvalidTerminalFails) {
    auto packet = createValidPacket();
    packet.terminalCode[0] = 0x00; // 改錯終止碼

    uint8_t raw[114];
    std::memcpy(raw, &packet, sizeof(packet));

    TWSEFormat1Parser parser;
    EXPECT_FALSE(parser.parse(raw, sizeof(raw)));
}

// ✅ 測試 ESC 改錯無法通過
TEST(Format1ParserTest, InvalidEscFails) {
    auto packet = createValidPacket();
    packet.escCode[0] = 0x00;

    uint8_t raw[114];
    std::memcpy(raw, &packet, sizeof(packet));

    TWSEFormat1Parser parser;
    EXPECT_FALSE(parser.parse(raw, sizeof(raw)));
}
