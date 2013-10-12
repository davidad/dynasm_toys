#define DASM_OPEN do { dasm_init(Dst, 1); dasm_setup(Dst, actionlist); } while(0)
#define DASM_ALINK do { size_t n; int a = dasm_link(Dst,&n); buffer = mmap(NULL,n,PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1,0); dasm_encode(Dst, buffer); dasm_free(Dst); mprotect(buffer,n,PROT_EXEC|PROT_READ); } while(0)
