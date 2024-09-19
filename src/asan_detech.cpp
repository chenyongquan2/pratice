#include "asan_detech.h"
#include <cstring>
#include <iostream>

char global_buffer[10];

void stack_overflow() {
    //栈溢出
    char stack_buffer[10];
    strcpy(stack_buffer, "This string is too long for the buffer");
}

void heap_overflow() {
    //堆溢出
    char* heap_buffer = new char[10];
    strcpy(heap_buffer, "This will overflow the heap buffer");
    delete[] heap_buffer;
}

void global_overflow() {
    //全局缓冲区溢出
    strcpy(global_buffer, "Overflowing the global buffer");
}

void TestAsanDetech() {
    //三种缓冲区溢出
    stack_overflow();
    heap_overflow();
    global_overflow();
    std::cout << "Program completed" << std::endl;

}
