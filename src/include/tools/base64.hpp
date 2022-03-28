#include "defs.h"

#ifdef __cplusplus

int ToBase64Fast(const unsigned char *pSrc, int nLenSrc, char *pDst, int nLenDst);

int FromBase64Fast(const BYTE *pSrc, int nLenSrc, char *pDst, int nLenDst);

int FromBase64Fast(const BYTE *pSrc, int nLenSrc, BYTE *pDst, int nLenDst);

#else

int FromBase64Fast_C(const BYTE *pSrc, int nLenSrc, BYTE *pDst, int nLenDst);

#endif
