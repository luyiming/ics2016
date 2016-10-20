#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include "FLOAT.h"

extern char _vfprintf_internal;
extern char _fpmaxtostr;
extern char _ppfs_setargs;
extern int __stdio_fwrite(char *buf, int len, FILE *stream);

__attribute__((used)) static int format_FLOAT(FILE *stream, FLOAT f) {
	/* TODO: Format a FLOAT argument `f' and write the formating
	 * result to `stream'. Keep the precision of the formating
	 * result with 6 by truncating. For example:
	 *              f          result
	 *         0x00010000    "1.000000"
	 *         0x00013333    "1.199996"
	 */

	// Here f is actually &f
	int p[7], t, i;
	f = *(FLOAT*)f;
	if(f < 0) {
		f = -f;
		p[0] = -(f >> 16);
	}
	else
		p[0] = f >> 16;
	t = (f & 0xffff) * 10;
	for(i = 1; i < 7; i++) {
		p[i] = t >> 16;
		t = ((t & 0xffff) << 1) + ((t & 0xffff) << 3);
	}
	char buf[80];
	int len = sprintf(buf, "%d.%1d%1d%1d%1d%1d%1d",
					  p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
	return __stdio_fwrite(buf, len, stream);
}

static void modify_vfprintf() {
	/* TODO: Implement this function to hijack the formating of "%f"
	 * argument during the execution of `_vfprintf_internal'. Below
	 * is the code section in _vfprintf_internal() relative to the
	 * hijack.
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = _fpmaxtostr(stream,
				(__fpmax_t)
				(PRINT_INFO_FLAG_VAL(&(ppfs->info),is_long_double)
				 ? *(long double *) *argptr
				 : (long double) (* (double *) *argptr)),
				&ppfs->info, FP_OUT );
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

	/* You should modify the run-time binary to let the code above
	 * call `format_FLOAT' defined in this source file, instead of
	 * `_fpmaxtostr'. When this function returns, the action of the
	 * code above should do the following:
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = format_FLOAT(stream, *(FLOAT *) *argptr);
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

	int32_t p = (int32_t)&_vfprintf_internal + 0x8048861 - 0x804855b;
/*
	mprotect((void*)((p - 100) & 0xfffff000), 4096 * 2,
			 PROT_READ | PROT_WRITE | PROT_EXEC);
*/
	int32_t o = (int32_t)&format_FLOAT - (int32_t)&_fpmaxtostr;
	*(int32_t*)(p + 1) = *(int32_t*)(p + 1) + o;
	/*
	8048854:	83 ec 0c             	sub    $0xc,%esp
    8048857:	db 3c 24             	fstpt  (%esp)
    804885a:	ff b4 24 8c 01 00 00 	pushl  0x18c(%esp)
	--->
xx	804883f:	db 2a                	fldt   (%edx)
    8048841:	eb 02                	jmp    8048845 <_vfprintf_internal+0x2ea>
xx	8048843:	dd 02                	fldl   (%edx)
    8048845:	53                   	push   %ebx
    8048846:	53                   	push   %ebx
    8048847:	68 f4 84 04 08       	push   $0x80484f4
    804884c:	8d 84 24 a4 00 00 00 	lea    0xa4(%esp),%eax
    8048853:	50                   	push   %eax
	8048854:	83 ec 08             	sub    $0x8,%esp
	8048857:	52                   	push   %edx
	8048858:	90                      nop
	8048859:	90                      nop
	804885a:	ff b4 24 8c 01 00 00 	pushl  0x18c(%esp)
p:	8048861:	e8 d8 0e 00 00       	call   804973e <_fpmaxtostr>
	*/
	*(uint32_t*)(p - 7 - 4) = 0x90905208;
	*(uint16_t*)(p - 30) = 0x9090;
	*(uint16_t*)(p - 34) = 0x9090;
}

static void modify_ppfs_setargs() {
	/* TODO: Implement this function to modify the action of preparing
	 * "%f" arguments for _vfprintf_internal() in _ppfs_setargs().
	 * Below is the code section in _vfprintf_internal() relative to
	 * the modification.
	 */

#if 0
	enum {                          /* C type: */
		PA_INT,                       /* int */
		PA_CHAR,                      /* int, cast to char */
		PA_WCHAR,                     /* wide char */
		PA_STRING,                    /* const char *, a '\0'-terminated string */
		PA_WSTRING,                   /* const wchar_t *, wide character string */
		PA_POINTER,                   /* void * */
		PA_FLOAT,                     /* float */
		PA_DOUBLE,                    /* double */
		__PA_NOARG,                   /* non-glibc -- signals non-arg width or prec */
		PA_LAST
	};

	/* Flag bits that can be set in a type returned by `parse_printf_format'.  */
	/* WARNING -- These differ in value from what glibc uses. */
#define PA_FLAG_MASK		(0xff00)
#define __PA_FLAG_CHAR		(0x0100) /* non-gnu -- to deal with hh */
#define PA_FLAG_SHORT		(0x0200)
#define PA_FLAG_LONG		(0x0400)
#define PA_FLAG_LONG_LONG	(0x0800)
#define PA_FLAG_LONG_DOUBLE	PA_FLAG_LONG_LONG
#define PA_FLAG_PTR		(0x1000) /* TODO -- make dynamic??? */

	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			case (PA_INT|PA_FLAG_LONG):
				GET_VA_ARG(p,ul,unsigned long,ppfs->arg);
				break;
			case PA_CHAR:	/* TODO - be careful */
				/* ... users could use above and really want below!! */
			case (PA_INT|__PA_FLAG_CHAR):/* TODO -- translate this!!! */
			case (PA_INT|PA_FLAG_SHORT):
			case PA_INT:
				GET_VA_ARG(p,u,unsigned int,ppfs->arg);
				break;
			case PA_WCHAR:	/* TODO -- assume int? */
				/* we're assuming wchar_t is at least an int */
				GET_VA_ARG(p,wc,wchar_t,ppfs->arg);
				break;
				/* PA_FLOAT */
			case PA_DOUBLE:
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			case (PA_DOUBLE|PA_FLAG_LONG_DOUBLE):
				GET_VA_ARG(p,ld,long double,ppfs->arg);
				break;
			default:
				/* TODO -- really need to ensure this can't happen */
				assert(ppfs->argtype[i-1] & PA_FLAG_PTR);
			case PA_POINTER:
			case PA_STRING:
			case PA_WSTRING:
				GET_VA_ARG(p,p,void *,ppfs->arg);
				break;
			case __PA_NOARG:
				continue;
		}
		++p;
	}
#endif

	/* You should modify the run-time binary to let the `PA_DOUBLE'
	 * branch execute the code in the `(PA_INT|PA_FLAG_LONG_LONG)'
	 * branch. Comparing to the original `PA_DOUBLE' branch, the
	 * target branch will also prepare a 64-bit argument, without
	 * introducing floating point instructions. When this function
	 * returns, the action of the code above should do the following:
	 */

#if 0
	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
			here:
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			// ......
				/* PA_FLOAT */
			case PA_DOUBLE:
				goto here;
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			// ......
		}
		++p;
	}
#endif

	/*
p:	8049199:	8d 5a 08    lea    0x8(%edx),%ebx
    804919c:	dd 02       fldl   (%edx)
	804919e:	89 58 4c    mov    %ebx,0x4c(%eax)
	80491a1:	dd 19      	fstpl  (%ecx)
	-->
	8049199:	eb 30    	jmp  80491cb <_ppfs_setargs+0xa3>
    804919b:	90	       	nop
 	804919c:	dd 02       fldl   (%edx)
	804919e:	89 58 4c    mov    %ebx,0x4c(%eax)
    80491a1:	dd 19       fstpl  (%ecx)
	*/
	/*
	int32_t p = (int32_t)&_ppfs_setargs + 0x71;

	mprotect((void*)((p - 100) & 0xfffff000), 4096 * 2,
			 PROT_READ | PROT_WRITE | PROT_EXEC);

	*(uint16_t*)p = 0x30eb;
	*(uint8_t*)(p + 2) = 0x90;
*/
	/*
	8049199:	8d 5a 08    lea    0x8(%edx),%ebx
p:	804919c:	dd 02       fldl   (%edx)
	804919e:	89 58 4c    mov    %ebx,0x4c(%eax)
	80491a1:	dd 19      	fstpl  (%ecx)
	-->
	8049199:	8d 5a 08    lea    0x8(%edx),%ebx
	804919c:	eb 2d    	jmp  80491cb <_ppfs_setargs+0xa3>
	804919e:	89 58 4c    mov    %ebx,0x4c(%eax)
    80491a1:	dd 19       fstpl  (%ecx)
	*/
	int32_t p = (int32_t)&_ppfs_setargs + 0x74;
/*
	mprotect((void*)((p - 100) & 0xfffff000), 4096 * 2,
			 PROT_READ | PROT_WRITE | PROT_EXEC);
*/
	*(uint16_t*)p = 0x2deb;
}

void init_FLOAT_vfprintf() {
	modify_vfprintf();
	modify_ppfs_setargs();
}
