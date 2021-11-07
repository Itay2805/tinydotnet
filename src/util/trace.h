#pragma once

#include <stdio.h>

void trace_hex(const void* data, size_t size);

#define TRACE(fmt, ...) printf("[*] " fmt "\n\r", ## __VA_ARGS__)
#define WARN(fmt, ...)  printf("[!] " fmt "\n\r", ## __VA_ARGS__)
#define ERROR(fmt, ...) printf("[-] " fmt "\n\r", ## __VA_ARGS__)

#define TRACE_HEX(data, size) trace_hex(data, size);
