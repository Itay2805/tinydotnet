#pragma once

#include "printf.h"

/**
 * Initialize the kernel tracing
 */
void trace_init();

void trace_hex(const void* data, size_t size);

#define TRACE(fmt, ...) printf("[*] " fmt "\r\n", ## __VA_ARGS__)
#define WARN(fmt, ...)  printf("[!] " fmt " (%s:%d)\r\n", ## __VA_ARGS__, __FILE_NAME__, __LINE__)
#define ERROR(fmt, ...) printf("[-] " fmt "\r\n", ## __VA_ARGS__)

#define TRACE_HEX(data, size) trace_hex(data, size);
