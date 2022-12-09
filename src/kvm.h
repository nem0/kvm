#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum { KVM_STACK_SIZE = 4096 };

typedef unsigned char kvm_u8;
typedef unsigned kvm_u32;
typedef unsigned long long kvm_u64;
typedef int kvm_i32;
typedef kvm_u32 kvm_label;

// Virtual machine
typedef struct KVM {
	kvm_u8 stack[KVM_STACK_SIZE];
	kvm_u32 sp;
	kvm_u8* environment;
	kvm_u32 environment_size;
} KVM;

// user provided callback
typedef void (*kvm_syscall)(KVM* kvm, kvm_u32 args_count);

// call this before vm is used
void kvm_init(KVM* vm, kvm_u8* environment, kvm_u32 environment_size);
// start executing bytecde from 'func'
void kvm_call(KVM* vm, const kvm_u8* bytecode, kvm_syscall syscall, kvm_label func);

// kvm_get* functions get value from stack (if idx < 0) or environment (if idx >= 0)
kvm_u32 kvm_get(KVM* vm, kvm_i32 idx);
kvm_u64 kvm_get64(KVM* vm, kvm_i32 idx);
void* kvm_get_ptr(KVM* vm, kvm_i32 idx);
float kvm_get_float(KVM* vm, kvm_i32 idx);

// push value to stack
void kvm_push(KVM* vm, kvm_u32 value);
void kvm_push_float(KVM* vm, float value);
void kvm_push_ptr(KVM* vm, void* ptr);

// creating bytecode
typedef struct kvm_bc_writer {
	kvm_u8* bytecode;
	kvm_u8* ip;
	kvm_u32 capacity;
	kvm_u32 labels_count;
	kvm_label labels[1024];
} kvm_bc_writer;

// kvm_bc_* functions are used the create bytecode

// call this before any writing to bytecode
void kvm_bc_start_write(kvm_bc_writer* writer, kvm_u8* bytecode, kvm_u32 capacity);
// call this after you finish writing to bytecode
void kvm_bc_end_write(kvm_bc_writer* writer);
// push r0 to stack
void kvm_bc_push(kvm_bc_writer* writer);
// pop from stack to r1
void kvm_bc_pop(kvm_bc_writer* writer);
// r1 = environment[env_idx] and push the value to stack
void kvm_bc_get(kvm_bc_writer* writer, kvm_u32 env_idx);
// environment[env_idx] = r1
void kvm_bc_set(kvm_bc_writer* writer, kvm_u32 env_idx);
// r1 += r2 as integers
void kvm_bc_add(kvm_bc_writer* writer);
// r1 += r2 as floats
void kvm_bc_addf(kvm_bc_writer* writer);
// r1 *= r2 as integers
void kvm_bc_mul(kvm_bc_writer* writer);
// r1 *= r2 as floats
void kvm_bc_mulf(kvm_bc_writer* writer);
// end execution
void kvm_bc_end(kvm_bc_writer* writer);
// call user provided syscall function, arguments need to be pushed on stack before this
// args are poped from stack after syscall returns
void kvm_bc_syscall(kvm_bc_writer* writer, kvm_u32 args_count);
//  push the value on stack and store it in r1
void kvm_bc_const(kvm_bc_writer* writer, kvm_u32 value);
//  push the value on stack and store it in r1
void kvm_bc_const_float(kvm_bc_writer* writer, float value);
//  push the value on stack
void kvm_bc_const64(kvm_bc_writer* writer, kvm_u64 value);
// jump to label if r1 == r2
void kvm_bc_eq(kvm_bc_writer* writer, kvm_label label);

// jump to r1
void kvm_bc_jmp(kvm_bc_writer* writer);
// call function
void kvm_bc_call(kvm_bc_writer* writer, kvm_label function);
// return from function call 
void kvm_bc_ret(kvm_bc_writer* writer);

// labels are used as jump destinations
kvm_label kvm_bc_create_label(kvm_bc_writer* writer);
// call this before the instruction you want to jump to
void kvm_bc_place_label(kvm_bc_writer* writer, kvm_label label);

#ifdef __cplusplus
}
#endif
