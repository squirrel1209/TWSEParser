#pragma once

// ===============================
// 抽象基底類別：TWSE 封包驗證接口
// ===============================
// 所有封包格式（Format1、Format6⋯）皆應繼承本類別，實作以下驗證行為：
// - 起始碼是否正確（ESC）
// - 結尾碼是否正確（0x0D 0x0A）
// - XOR 驗證碼是否正確
// - 綜合驗證（verifyAll）

class TWSEPacketBase {
public:
    /// 驗證封包中的 XOR 校驗值是否正確
    virtual bool checkXor() const = 0;

    /// 驗證封包起始碼是否為合法的 ESC（0x1B）
    virtual bool verifyEsc() const = 0;

    /// 驗證封包結尾碼是否為 0x0D 0x0A（通常位於最後兩個位元組）
    virtual bool verifyTerminal() const = 0;

    /// 綜合驗證上述三項，並以參數回傳各步驟結果
    virtual bool verifyAll(bool& escOK, bool& termOK, bool& xorOK) const = 0;

    /// 虛擬解構子，確保子類能正確釋放
    virtual ~TWSEPacketBase() = default;
};
