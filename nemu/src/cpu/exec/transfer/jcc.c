#include "cpu/exec/helper.h"

#define	DATA_BYTE	4
#include "jcc-template.h"
#undef	DATA_BYTE

#define	DATA_BYTE	2
#include "jcc-template.h"
#undef	DATA_BYTE

#define	DATA_BYTE	1
#include "jcc-template.h"
#undef	DATA_BYTE

make_helper_v(ja)
make_helper_v(jae)
make_helper_v(jb)
make_helper_v(jbe)
make_helper_v(je)
make_helper_v(jecxz)
make_helper_v(jg)
make_helper_v(jge)
make_helper_v(jl)
make_helper_v(jle)
make_helper_v(jne)
make_helper_v(jno)
make_helper_v(jnp)
make_helper_v(jns)
make_helper_v(jo)
make_helper_v(jp)
make_helper_v(js)
