
#ifndef BUGS_B_GONE_H
#define BUGS_B_GONE_H

#ifdef _DEBUG

#define TRACE(args)     { trace args; }
#define CHECKPOINT(msg) { TRACE((__FILE__, __LINE__, "CHECKPOINT %s", msg)) }
#define ASSURE(expr)    { assure(__FILE__, __LINE__, expr, #expr); }

void trace(char*, int, const char*, ...);
void assure(char*, int, bool, char*);

#else

#define TRACE(args)
#define CHECKPOINT(msg)
#define ASSURE(expr)

#endif

#endif // BUGS_B_GONE_H
