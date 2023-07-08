#pragma once

#define CONTEXT_SWITCH_X64 __asm  \
{                                 \
    __asm _emit 0x6A              \
    __asm _emit 0x33              \
    __asm _emit 0xE8              \
    __asm _emit 0x00              \
    __asm _emit 0x00              \
    __asm _emit 0x00              \
    __asm _emit 0x00              \
    __asm _emit 0x3E              \
    __asm _emit 0x83              \
    __asm _emit 0x04              \
    __asm _emit 0x24              \
    __asm _emit 0x06              \
    __asm _emit 0xCB              \
}                                 \

#define CONTEXT_SWITCH_X32 __asm  \
{                                 \
    __asm _emit 0xE8              \
    __asm _emit 0x00              \
    __asm _emit 0x00              \
    __asm _emit 0x00              \
    __asm _emit 0x00              \
    __asm _emit 0xC7              \
    __asm _emit 0x44              \
    __asm _emit 0x24              \
    __asm _emit 0x04              \
    __asm _emit 0x23              \
    __asm _emit 0x00              \
    __asm _emit 0x00              \
    __asm _emit 0x00              \
    __asm _emit 0x83              \
    __asm _emit 0x04              \
    __asm _emit 0x24              \
    __asm _emit 0x0D              \
    __asm _emit 0xCB              \
}                                 \
