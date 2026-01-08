#!/bin/bash

# PyOCD 停止脚本
# 使用方法: ./stop_debug.sh

# 获取脚本目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=== 停止 PyOCD GDB Server ==="

# 查找并停止 PyOCD 进程
PIDS=$(pgrep -f "pyocd.*gdbserver")

if [ -z "$PIDS" ]; then
    echo "没有找到运行中的 PyOCD GDB Server"
else
    echo "找到 PyOCD 进程: $PIDS"
    echo "正在停止..."
    
    # 优雅地停止进程
    pkill -TERM -f "pyocd.*gdbserver"
    sleep 2
    
    # 检查是否还在运行
    if pgrep -f "pyocd.*gdbserver" > /dev/null; then
        echo "强制停止进程..."
        pkill -KILL -f "pyocd.*gdbserver"
        sleep 1
    fi
    
    if pgrep -f "pyocd.*gdbserver" > /dev/null; then
        echo "✗ 无法停止 PyOCD 进程"
        exit 1
    else
        echo "✓ PyOCD GDB Server 已停止"
    fi
fi

# 清理日志文件（可选）
if [ -f "$SCRIPT_DIR/pyocd_server.log" ]; then
    read -p "是否要清理日志文件? (y/N): " clean_log
    if [[ $clean_log =~ ^[Yy]$ ]]; then
        rm -f "$SCRIPT_DIR/pyocd_server.log"
        echo "日志文件已清理"
    fi
fi

echo "完成"