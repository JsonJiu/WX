/**
 * @file syscalls.c
 * @brief Newlib 系统调用桩函数实现（仅用于 GCC
 * 编译器）,没有这个文件其实也不影响，只是会有链接器警告
 * @note  消除链接器关于 _close, _lseek, _read, _write 等未实现的警告
 *        Keil 使用 MicroLIB，不需要这些函数，通过条件编译排除
 */

/* 仅在 GCC 编译器下编译此文件 */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)

#include <errno.h>
#include <sys/stat.h>

/* 避免 unused parameter 警告 */
#define UNUSED(x) (void)(x)

/**
 * @brief 关闭文件（桩函数）
 * @param fd 文件描述符
 * @return -1 表示失败
 */
int _close(int fd) {
  UNUSED(fd);
  errno = EBADF;
  return -1;
}

/**
 * @brief 文件定位（桩函数）
 * @param fd 文件描述符
 * @param offset 偏移量
 * @param whence 定位方式
 * @return -1 表示失败
 */
int _lseek(int fd, int offset, int whence) {
  UNUSED(fd);
  UNUSED(offset);
  UNUSED(whence);
  errno = EBADF;
  return -1;
}

/**
 * @brief 读取文件（桩函数）
 * @param fd 文件描述符
 * @param buf 缓冲区
 * @param count 字节数
 * @return -1 表示失败
 */
int _read(int fd, char *buf, int count) {
  UNUSED(fd);
  UNUSED(buf);
  UNUSED(count);
  errno = EBADF;
  return -1;
}

/**
 * @brief 写入文件（桩函数）
 * @param fd 文件描述符
 * @param buf 缓冲区
 * @param count 字节数
 * @return -1 表示失败
 */
int _write(int fd, char *buf, int count) {
  UNUSED(fd);
  UNUSED(buf);
  UNUSED(count);
  errno = EBADF;
  return -1;
}

/**
 * @brief 获取文件状态（桩函数）
 * @param fd 文件描述符
 * @param st 状态结构体
 * @return -1 表示失败
 */
int _fstat(int fd, struct stat *st) {
  UNUSED(fd);
  UNUSED(st);
  errno = EBADF;
  return -1;
}

/**
 * @brief 检查是否为终端（桩函数）
 * @param fd 文件描述符
 * @return 0 表示不是终端
 */
int _isatty(int fd) {
  UNUSED(fd);
  return 0;
}

/**
 * @brief 获取进程 ID（桩函数）
 * @return 1
 */
int _getpid(void) { return 1; }

/**
 * @brief 发送信号（桩函数）
 * @param pid 进程 ID
 * @param sig 信号
 * @return -1 表示失败
 */
int _kill(int pid, int sig) {
  UNUSED(pid);
  UNUSED(sig);
  errno = EINVAL;
  return -1;
}

#endif /* __GNUC__ && !__ARMCC_VERSION */
