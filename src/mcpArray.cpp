#include <Wire.h>
#include "Adafruit_MCP23X17.h"

class MCPArray {
public:
    MCPArray() {
        // MCPデバイスの初期化
        for (int i = 0; i < MCP_COUNT; i++) {
            mcp_devices[i].begin_I2C(MCP_BASE_ADDR + i, &Wire);
        }
    }

    Adafruit_MCP23X17& operator[](uint8_t addr) {
        return mcp_devices[addr - MCP_BASE_ADDR];
    }

private:
    static const int MCP_COUNT = 4;  // 接続されているMCP23017の数
    static const int MCP_BASE_ADDR = 0x21;  // 最初のMCP23017のアドレス
    Adafruit_MCP23X17 mcp_devices[MCP_COUNT];
};

MCPArray mcp;

