#include <kvm.h>
#include <stdio.h>
#include <string.h>

void syscall(KVM* vm, kvm_u32 args_count) {
	const kvm_u32 a = kvm_get(vm, -(int)args_count + 0);
	const kvm_u32 b = kvm_get(vm, -(int)args_count + 1);
	if (a == 2)
		printf("hello from subroutine call - %d", b);
	else
		printf("error");
}

int main() {
	kvm_u8 bytecode[1024];

	kvm_bc_writer writer;
	kvm_bc_start_write(&writer, bytecode, sizeof(bytecode));

	kvm_bc_const(&writer, 2);
	kvm_bc_const(&writer, 42);
	kvm_bc_syscall(&writer, 2);

	kvm_bc_end_write(&writer);

	KVM vm;
	kvm_init(&vm, NULL, 0);
	kvm_call(&vm, bytecode, syscall, 0);
}