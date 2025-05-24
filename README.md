
# 📦 TWSEPacketParser

TWSEPacketParser 是一套專為解析台灣證券交易所（TWSE）封包格式所設計的高效能解析系統。  
專案採用現代 C++（C++17/20 標準）、多型設計、模組化架構，支援多格式封包（Format 1 / 6）解析，結合驗證、轉碼工具與資料封裝，適合作為金融市場數據解析、後端應用開發的學習範例。

---

## 📂 專案目錄結構

```
TWSEPacketParser/
├── include/                         ← 所有 .h 標頭檔案
│   ├── base/                        ← 抽象介面與工廠類別
│   ├── common/                      ← 共用工具（編碼、BCD、固定長度陣列）
│   ├── format1/                     ← Format1（基本資料格式）相關類別
│   ├── format6/                     ← Format6（撮合價格格式）相關類別
│   ├── result/                      ← 各格式解析結果封裝（OOP 樣式）
│   └── controller/                  ← 控制器與應用層
├── src/                             ← 所有 .cpp 實作檔案
├── test/                            ← ✅ 單元測試（建議搭配 GoogleTest）
├── data/                            ← 測試封包資料（可模擬來自 TCP stream）
├── CMakeLists.txt                   ← CMake 編譯腳本
└── README.md                        ← 專案說明（本檔案）
```

---

## 🧱 架構設計概念

### 🎯 核心模組

- **`base/`**  
  - `TWSEPacketBase.h`：封包基底類別，提供 `verifyAll()` 驗證邏輯
  - `TWSEFormatParser.h`：抽象解析器介面，定義 `parse()` 行為
  - `TWSEParserFactory.h`：工廠介面，根據 `formatCode` 建立對應解析器
  - `TWSEParsedResult.h`：抽象解析結果類別，封裝解析後資料

- **`common/`**  
  - `TBCD.h`：BCD 編碼轉換工具
  - `FixedArray.h` / `FixedArray.tpp`：固定長度模板陣列
  - `AsciiUtils.h` / `EncodingUtils.h`：文字轉換、補空格、Big5→UTF8 工具

- **`format1/` & `format6/`**  
  - 專屬格式的封包定義（例：`TWSEFormat1.h`）、解析器（例：`TWSEFormat1Parser.h`）、驗證邏輯與欄位封裝

- **`controller/`**  
  - `TWSEPacketParser.h`：分流處理多筆封包
  - `TWSEApp.h`：主應用流程封裝，呼叫 `run()` 進行處理

- **`result/`**  
  - `TWSEParsedFormat1Result.h` / `TWSEParsedFormat6Result.h`：各格式解析結果封裝

---

## ⚙️ 編譯與執行

### 1️⃣ 環境需求

- C++17 或以上編譯器（GCC / Clang / MSVC）
- CMake 3.15 以上
- 建議安裝 [GoogleTest](https://github.com/google/googletest) 進行單元測試

### 2️⃣ 編譯流程

```bash
# 建立編譯目錄
mkdir build && cd build

# 產生 Makefile
cmake ..

# 編譯
cmake --build .
```

### 3️⃣ 執行主程式

```bash
./TWSEPacketParser
```

或根據 `TWSEApp.h` 中的應用邏輯啟動程式。

### 4️⃣ 執行測試

```bash
./test/test_format1
./test/test_format6
./test/test_parser_factory
```

---

## 🧪 範例資料與輸出

### 輸入範例

- 位於 `data/Tse.bin`，模擬 TWSE 封包的二進位資料，可作為測試與驗證使用。

### 輸出範例

執行後會產生 CSV 檔案，例如 `2330_history.csv`，內容格式如下：

```csv
Time,ProductID,Price,Volume,Buy/Sell
093000,2330,595.00,20,B
093001,2330,596.00,5,S
093002,2330,597.00,10,B
```

### 欄位說明

| 欄位名稱     | 說明                             |
|--------------|----------------------------------|
| Time         | 時間（HHMMSS 格式）               |
| ProductID    | 股票代碼（例：2330 台積電）        |
| Price        | 成交價格                           |
| Volume       | 成交數量（股數或口數）              |
| Buy/Sell     | 買賣方向（B=買進, S=賣出）         |

---

## 🚀 使用流程快速總結

1️⃣ 放置 TWSE 封包資料（如 `Tse.bin`）於 `data/` 資料夾  
2️⃣ 編譯並執行程式  
3️⃣ 程式自動解析並產出 CSV 檔（如 `2330_history.csv`）  
4️⃣ 使用 Excel、Pandas 等工具開啟分析

---

## 📌 待補充項目

- [ ] 支援其他格式（如 Format2, Format7）
- [ ] TCP Stream 讀取整合（實現完整收封包流程）
- [ ] 多執行緒處理與效能優化
- [ ] 更多單元測試覆蓋

---

## 📞 聯絡方式

專案由 [陳鈺翔] 維護，如有問題歡迎聯繫！  
Email: your_email@example.com
