#include "lz4.h"
#include "lsm.h"
#include "lsm-lz4.h"

static int
lsm_lz4_xBound(void *pCtx, int nIn)
{
  return LZ4_compressBound(nIn);
}

static int
lsm_lz4_xCompress(void *pCtx, void *pOut, int *pnOut, const void *pIn, int nIn)
{
  int rc = LZ4_compress((const char*)pIn, (char*)pOut, nIn);
  *pnOut = rc;
  return (rc != 0 ? LSM_OK : LSM_ERROR);
}

static int
lsm_lz4_xUncompress(void *pCtx, void *pOut, int *pnOut, const void *pIn, int nIn)
{
  int rc = LZ4_decompress_fast((const char*)pIn, (char*)pOut, *pnOut);
  return (rc > 0 ? LSM_OK : LSM_ERROR);
}


int lsm_lz4_config(lsm_db *db)
{
  lsm_compress lz4_comp = {
    .pCtx = NULL,
    .iId  = LSM_LZ4_ID,
    .xBound = lsm_lz4_xBound,
    .xCompress = lsm_lz4_xCompress,
    .xUncompress = lsm_lz4_xUncompress,
    .xFree = NULL,
  };
  return lsm_config(db, LSM_CONFIG_SET_COMPRESSION, &lz4_comp);
}
