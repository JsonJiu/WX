#!/bin/bash
# PyOCD 自动启动脚本（增强版）
# 使用方法: ./start_debug.sh

# 获取脚本所在目录，然后找到项目根目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
PYOCD_PATH="/home/noah/Program/Embeded_work_space/PyOCD/venv/bin/pyocd"
TARGET="FM33LG04x"
FREQUENCY="1000000"
PACK_FILE="$PROJECT_DIR/VscodeGcc/FMSH.FM33LG0XX_DFP.3.0.1.pack"
PORT="3333"
TELNET_PORT="4444"

echo "=== FM33LG04x Debug Helper (Enhanced) ==="
echo "脚本目录: $SCRIPT_DIR"
echo "项目目录: $PROJECT_DIR"
echo "Pack 文件: $PACK_FILE"

# 检查 Pack 文件是否存在
if [ ! -f "$PACK_FILE" ]; then
    echo "错误: Pack 文件不存在: $PACK_FILE"
    exit 1
fi

# 检查是否已有 PyOCD 服务器在运行
if pgrep -f "pyocd.*gdbserver" > /dev/null; then
    echo "检测到 PyOCD GDB Server 正在运行"
    
    # 检查服务器是否响应
    if netstat -ln | grep ":$PORT " > /dev/null 2>&1; then
        echo "服务器正在监听端口 $PORT"
        
        # 尝试重置目标板
        echo "尝试重置目标板..."
        timeout 3 bash -c 'echo -e "reset halt\nquit" | telnet localhost 4444' 2>/dev/null || echo "重置命令发送失败（这是正常的）"
        
        read -p "服务器似乎正常运行，是否要重启? (y/N): " restart
        if [[ ! $restart =~ ^[Yy]$ ]]; then
            echo "使用现有的服务器，目标板已重置"
            exit 0
        fi
    fi
    
    echo "停止现有的 PyOCD 服务器..."
    pkill -f "pyocd.*gdbserver"
    sleep 3
    
    # 确保完全停止
    if pgrep -f "pyocd.*gdbserver" > /dev/null; then
        echo "强制停止 PyOCD 服务器..."
        pkill -9 -f "pyocd.*gdbserver"
        sleep 1
    fi
fi

# 构建项目
echo "构建项目..."
cd "$PROJECT_DIR"
if [ -d "build" ]; then
    cmake --build build
    if [ $? -ne 0 ]; then
        echo "构建失败！"
        exit 1
    fi
else
    echo "build 目录不存在，正在创建并配置..."
    mkdir -p build
    cd build
    cmake ..
    if [ $? -ne 0 ]; then
        echo "CMake 配置失败！"
        exit 1
    fi
    cd ..
    echo "正在构建项目..."
    cmake --build build
    if [ $? -ne 0 ]; then
        echo "构建失败！"
        exit 1
    fi
fi

# 启动 PyOCD GDB Server
echo "启动 PyOCD GDB Server..."
echo "工作目录: $PROJECT_DIR"
echo "命令: $PYOCD_PATH gdbserver --target $TARGET --frequency $FREQUENCY --pack $PACK_FILE --port $PORT --telnet-port $TELNET_PORT"

# 切换到项目目录启动 PyOCD
cd "$PROJECT_DIR"
nohup $PYOCD_PATH gdbserver \
    --target $TARGET \
    --frequency $FREQUENCY \
    --pack "$PACK_FILE" \
    --port $PORT \
    --telnet-port $TELNET_PORT \
    --persist \
    > "$SCRIPT_DIR/pyocd_server.log" 2>&1 &

PYOCD_PID=$!
echo "PyOCD GDB Server 已启动 (PID: $PYOCD_PID)"
echo "日志文件: $SCRIPT_DIR/pyocd_server.log"

# 等待服务器启动
echo "等待服务器启动..."
for i in {1..15}; do
    if netstat -ln | grep ":$PORT " > /dev/null 2>&1; then
        echo "✓ PyOCD GDB Server 已成功启动在端口 $PORT"
        echo "✓ Telnet 接口在端口 $TELNET_PORT"
        echo ""
        echo "现在你可以："
        echo "1. 在 VSCode 中按 F5 开始调试"
        echo "2. 运行: ./restart_debug.sh 来重置调试会话"
        echo "3. 运行: ./stop_debug.sh 来停止服务器"
        echo ""
        echo "要查看服务器日志，运行: tail -f $SCRIPT_DIR/pyocd_server.log"
        exit 0
    fi
    sleep 1
done

echo "✗ 服务器启动可能失败，请检查日志："
echo "  tail -f $SCRIPT_DIR/pyocd_server.log"
exit 1