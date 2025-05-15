// TWSEPacketParser.h
#pragma once
#include "base/TWSEParsedResult.h"
#include <vector>
#include <memory>
#include <cstdint>

// ============================================
// TWSEPacketParser
// - 主流程控制器：從原始資料掃描封包
// - 根據 formatCode 自動分派對應解析器
// - 支援多筆封包解析與結果儲存
// ============================================
class TWSEPacketParser {
private:
    const std::vector<uint8_t>& rawData;  // 原始封包資料
    std::vector<std::shared_ptr<TWSEParsedResult>> results;  // 解析結果集合

public:
    /// 建構解析器，傳入原始資料參考
    TWSEPacketParser(const std::vector<uint8_t>& data) : rawData(data) {}

    /// 執行封包掃描與解析（呼叫對應 Format parser）
    void parseAll();

    /// 傳回所有成功解析的封包結果
    const std::vector<std::shared_ptr<TWSEParsedResult>>& getResults() const;
};