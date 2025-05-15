// TWSEApp.h
#pragma once
#include <string>

/// =======================================
/// TWSEApp
/// - 封裝應用層邏輯：載入封包 → 解析 → 存入資料庫
/// - 由 main.cpp 呼叫，為使用者介面進入點
/// =======================================
class TWSEApp {
public:
    /// 執行應用主流程
    /// @param filename 封包檔案的完整路徑
    void run(const std::string& filename);
};
