#include "controller/TWSEApp.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "❌ 用法: " << argv[0] << " <封包檔案路徑>\n";
        return 1;
    }

    const std::string filename = argv[1];
    TWSEApp app;
    app.run(filename);

    return 0;
}
