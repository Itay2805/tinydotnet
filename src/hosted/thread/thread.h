#pragma once

#include <dotnet/gc/gc_thread_data.h>
#include <hosted/sync/wait_group.h>
#include <util/defs.h>

#include <sys/types.h>
#include <pthread.h>


typedef enum thread_status {
    /**
     * Means this thread was just allocated and has not
     * yet been initialized
     */
    THREAD_STATUS_IDLE,

    /**
     * Means this thread is on a run queue. It is
     * not currently executing user code.
     */
    THREAD_STATUS_RUNNABLE,

    /**
     * Means this thread may execute user code.
     */
    THREAD_STATUS_RUNNING,

    /**
     * Means this thread is blocked in the runtime.
     * It is not executing user code. It is not on a run queue,
     * but should be recorded somewhere so it can be scheduled
     * when necessary.
     */
    THREAD_STATUS_WAITING,

    /**
     * Means the thread stopped itself for a suspend
     * preemption. IT is like THREAD_STATUS_WAITING, but
     * nothing is yet responsible for readying it. some
     * suspend must CAS the status to THREAD_STATUS_WAITING
     * to take responsibility for readying this thread
     */
    THREAD_STATUS_PREEMPTED,

    /**
     * Means this thread is currently unused. It may be
     * just exited, on a free list, or just being initialized.
     * It is not executing user code.
     */
    THREAD_STATUS_DEAD,

    /**
     * Indicates someone wants to suspend this thread (probably the
     * garbage collector).
     */
    THREAD_SUSPEND = 0x1000,
} thread_status_t;

typedef struct thread_fx_save_state {
    uint16_t fcw;
    uint16_t fsw;
    uint16_t ftw;
    uint16_t opcode;
    uint32_t eip;
    uint16_t cs;
    uint16_t _reserved1;
    uint32_t dataoffset;
    uint16_t ds;
    uint8_t _reserved2[2];
    uint32_t mxcsr;
    uint32_t mxcsr_mask;
    uint8_t st0mm0[10];
    uint8_t _reserved3[6];
    uint8_t st1mm1[10];
    uint8_t _reserved4[6];
    uint8_t st2mm2[10];
    uint8_t _reserved5[6];
    uint8_t st3mm3[10];
    uint8_t _reserved6[6];
    uint8_t st4mm4[10];
    uint8_t _reserved7[6];
    uint8_t st5mm5[10];
    uint8_t _reserved8[6];
    uint8_t st6mm6[10];
    uint8_t _reserved9[6];
    uint8_t st7mm7[10];
    uint8_t _reserved10[6];
    uint8_t xmm0[16];
    uint8_t xmm1[16];
    uint8_t xmm2[16];
    uint8_t xmm3[16];
    uint8_t xmm4[16];
    uint8_t xmm5[16];
    uint8_t xmm6[16];
    uint8_t xmm7[16];
    uint8_t xmm8[16];
    uint8_t xmm9[16];
    uint8_t xmm10[16];
    uint8_t xmm11[16];
    uint8_t xmm12[16];
    uint8_t xmm13[16];
    uint8_t xmm14[16];
    uint8_t xmm15[16];
    uint8_t _reserved11[6 * 16];
} PACKED thread_fx_save_state_t;
STATIC_ASSERT(sizeof(thread_fx_save_state_t) == 512);

typedef struct thread_save_state {
    // fpu/sse/sse2
    thread_fx_save_state_t fx_save_state;

    // gprs
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;
    uint64_t rflags;
    uint64_t rsp;
} thread_save_state_t;

typedef struct thread_control_block {
    struct thread_control_block* tcb;

    // The per-thread data for the gc
    gc_thread_data_t gc_data;
} thread_control_block_t;


typedef struct wait_group wait_group_t;
typedef struct thread {
    // --- general pentagon info
    // thread name
    char name[64];
    // entrypoint function
    void* entry;
    // ctx passed to chreate_thread
    void* ctx;
    // base of the stack (smallest valid addr)
    uintptr_t stack_top;
    // used for gc_data
    thread_control_block_t* tcb;
    // save state, NOTE: only GPRs are filled
    thread_save_state_t save_state;

    // --- hosted specific trickery
    pthread_t pthread;
    // raw linux syscalls don't accept a pthread_t 
    // so store the information identifying a thread
    uid_t uid;
    pid_t pid;
    pid_t tid;
    // TODO: remove dead thread from g_all_threads
    // for now this is enough 
    bool dead;
    // waitgroup for syncronization: used to make sure all init has completed
    // and to ensure the register save has completed before returning
    wait_group_t wg;
} thread_t;

/**
 * For thread-locals
 */
#define THREAD_LOCAL _Thread_local

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread creation and destruction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void(*thread_entry_t)(void* ctx);

/**
 * Create a new thread
 */
thread_t* create_thread(thread_entry_t entry, void* ctx, const char* fmt, ...);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All thread iteration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * All the threads in the system are part of this array
 */
extern thread_t** g_all_threads;

void lock_all_threads();

void unlock_all_threads();
