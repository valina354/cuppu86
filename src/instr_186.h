#ifndef INSTR_186_H
#define INSTR_186_H

#include "types.h"
#include "cpu_extra.h"

void pusha(cpu* c);
void popa(cpu* c);

void enter(cpu* c, u16 storage, u8 level);
void leave(cpu* c);

void bound(cpu* c, reg r, u32 addr);

void insb(cpu* c);
void insw(cpu* c);
void outsb(cpu* c);
void outsw(cpu* c);

#endif