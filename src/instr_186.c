#include "instr_186.h"
#include "cpu.h"
#include "push_pop.h"
#include "flagops.h"
#include "interrupts.h"

void pusha(cpu* c) {
    u16 temp_sp = c->sp;
    push_r(c, AX);
    push_r(c, CX);
    push_r(c, DX);
    push_r(c, BX);
    push16(c, temp_sp);
    push_r(c, BP);
    push_r(c, SI);
    push_r(c, DI);
}

void popa(cpu* c) {
    pop_r(c, DI);
    pop_r(c, SI);
    pop_r(c, BP);
    c->sp += 2;
    pop_r(c, BX);
    pop_r(c, DX);
    pop_r(c, CX);
    pop_r(c, AX);
}

void enter(cpu* c, u16 storage, u8 level) {
    push_r(c, BP);
    u16 frame_ptr = c->sp;

    if (level > 0) {
        for (u8 i = 1; i < (level % 32); i++) {
            c->bp -= 2;
            push16(c, cpu_read_u16_at(c, base_offset(c->ss, c->bp)));
        }
        push16(c, frame_ptr);
    }

    c->bp = frame_ptr;
    c->sp -= storage;
}

void leave(cpu* c) {
    c->sp = c->bp;
    pop_r(c, BP);
}

void bound(cpu* c, reg r, u32 addr) {
    i16 val = (i16)get_reg16_val(c, r);
    i16 lower_bound = (i16)cpu_read_u16_at(c, addr);
    i16 upper_bound = (i16)cpu_read_u16_at(c, addr + 2);

    if (val < lower_bound || val > upper_bound) {
        interrupt(c, 5);
    }
}

void insb(cpu* c) {
    u8 val = cpu_read_u8_iop(c, c->dx);
    cpu_write_u8_at(c, base_offset(c->es, c->di), val);

    if (getDF(c) == 0) {
        c->di++;
    }
    else {
        c->di--;
    }
}

void insw(cpu* c) {
    u16 val = cpu_read_u16_iop(c, c->dx);
    cpu_write_u16_at(c, base_offset(c->es, c->di), val);

    if (getDF(c) == 0) {
        c->di += 2;
    }
    else {
        c->di -= 2;
    }
}

void outsb(cpu* c) {
    u8 val = cpu_read_u8_at(c, base_offset(c->ds, c->si));
    cpu_write_u8_iop(c, c->dx, val);

    if (getDF(c) == 0) {
        c->si++;
    }
    else {
        c->si--;
    }
}

void outsw(cpu* c) {
    u16 val = cpu_read_u16_at(c, base_offset(c->ds, c->si));
    cpu_write_u16_iop(c, c->dx, val);

    if (getDF(c) == 0) {
        c->si += 2;
    }
    else {
        c->si -= 2;
    }
}