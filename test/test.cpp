#include "controller/TWSEPacketParser.h"
#include "result/TWSEParsedFormat6Result.h"
#include <fstream>
#include <vector>
#include <memory>
#include <map>
#include <iomanip>
#include <filesystem>
#include <iostream>
#include <string>
#include <set>

// 分線資料結構
struct MinuteBar {
    int minuteKey;              // 時間索引（例如 0930 代表 09:30）
    double open = 0;            // 開盤價
    double high = 0;            // 最高價
    double low = 0;             // 最低價
    double close = 0;           // 收盤價
    uint64_t volume = 0;        // 成交量（總和）
};

// 加入單筆成交資料進對應時間區間的 bar 中，更新 OHLCV
void addTradeToBar(std::map<int, MinuteBar>& bars, int key, double price, uint64_t volume) {
    auto& bar = bars[key];
    bar.minuteKey = key;
    if (bar.volume == 0) {
        // 第一筆：同時設定為 open/high/low/close
        bar.open = bar.high = bar.low = bar.close = price;
    } else {
        // 更新 high/low/close
        if (price > bar.high) bar.high = price;
        if (price < bar.low)  bar.low  = price;
        bar.close = price;
    }
    bar.volume += volume;
}

// 將 BCD6 時間欄位（6 bytes）轉成 HHMMSS 整數格式
int BCD6ToHHMMSS(const BCD6& bcd) {
    const uint8_t* bytes = &bcd.data()[0];
    int hh = (bytes[0] >> 4) * 10 + (bytes[0] & 0x0F);
    int mm = (bytes[1] >> 4) * 10 + (bytes[1] & 0x0F);
    int ss = (bytes[2] >> 4) * 10 + (bytes[2] & 0x0F);
    return hh * 10000 + mm * 100 + ss;
}

// 去除字串前後空白
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(' ');
    size_t end = s.find_last_not_of(' ');
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// 計算分線時間區間的索引，例如 0936 對 10 分線會回傳 0930
int calcIntervalKey(int hhmm, int interval) {
    int hour = hhmm / 100;
    int minute = hhmm % 100;
    int grouped = (minute / interval) * interval;
    return hour * 100 + grouped;
}

// 將分線資料寫出為 CSV 檔案
void exportToCSV(const std::string& filename, const std::map<int, MinuteBar>& bars) {
    std::ofstream out(filename);
    out << "Time,Open,High,Low,Close,Volume\n";
    for (const auto& [k, bar] : bars) {
        out << std::setw(4) << std::setfill('0') << k << ","
            << bar.open << "," << bar.high << "," << bar.low << ","
            << bar.close << "," << bar.volume << "\n";
    }
}

int main() {
    // 開啟封包檔案
    std::ifstream file("../data/Tse.bin", std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "❌ 無法開啟封包檔案 ../data/Tse.bin\n";
        return 1;
    }

    // 讀取封包資料
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> raw(size);
    file.read(reinterpret_cast<char*>(raw.data()), size);

    // 初始化解析器並解析全部封包
    TWSEPacketParser parser(raw);
    parser.parseAll();
    const auto& results = parser.getResults();

    // 建立多種分線對應的 bar 容器
    std::map<int, MinuteBar> bars1, bars3, bars5, bars10, bars15, bars30, bars60;
    int total = 0, format6Count = 0, matched2330 = 0;

    // 遍歷所有封包結果
    for (const auto& result : results) {
        total++;
        auto f6 = std::dynamic_pointer_cast<TWSEParsedFormat6Result>(result);
        if (!f6) continue;               // 只處理 Format 6 封包
        format6Count++;

        const auto& body = f6->fixed.body;
        std::string code = trim(body.stockCode.toString());
        if (code != "2330") continue;   // 只分析 2330
        matched2330++;

        if (f6->dealPrices.empty()) continue; // 無成交價則略過

        // 解析時間為 HHMM 格式
        int hhmmss = BCD6ToHHMMSS(body.matchTime);
        int hhmm = hhmmss / 100;

        // 取得第一筆成交價與成交量
        double price = f6->dealPrices[0].price.toDouble(2);
        uint64_t volume = static_cast<uint64_t>(f6->dealPrices[0].quantity.toInt());

        // 寫入各個分線桶
        addTradeToBar(bars1, hhmm, price, volume);
        addTradeToBar(bars3, calcIntervalKey(hhmm, 3), price, volume);
        addTradeToBar(bars5, calcIntervalKey(hhmm, 5), price, volume);
        addTradeToBar(bars10, calcIntervalKey(hhmm, 10), price, volume);
        addTradeToBar(bars15, calcIntervalKey(hhmm, 15), price, volume);
        addTradeToBar(bars30, calcIntervalKey(hhmm, 30), price, volume);
        addTradeToBar(bars60, calcIntervalKey(hhmm, 60), price, volume);
    }

    // 印出統計結果
    std::cout << "📊 總封包數：" << total
              << "，Format6 封包數：" << format6Count
              << "，符合 2330 且有成交價的封包：" << matched2330 << "\n";

    // 建立輸出資料夾
    std::filesystem::create_directories("output");

    // 輸出各分線 K 線 CSV 檔案
    exportToCSV("output/2330_1min_kline.csv", bars1);
    exportToCSV("output/2330_3min_kline.csv", bars3);
    exportToCSV("output/2330_5min_kline.csv", bars5);
    exportToCSV("output/2330_10min_kline.csv", bars10);
    exportToCSV("output/2330_15min_kline.csv", bars15);
    exportToCSV("output/2330_30min_kline.csv", bars30);
    exportToCSV("output/2330_60min_kline.csv", bars60);

    std::cout << "✅ 所有分線 K 線輸出完成 output/2330_*.csv\n";
    return 0;
}
