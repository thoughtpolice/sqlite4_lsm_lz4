#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lsm.h"

#define BUFFER_SIZE 128
#define INPUT_FILE "/usr/share/dict/words"

#ifdef USE_LSM_LZ4_COMPRESSOR
#define DATABASE_NAME "test-lz4.db"
#include "lsm-lz4.h"
#else
#define DATABASE_NAME "test.db"
#endif /* USE_LZ4_LSM_COMPRESSOR */

int main()
{
  int rc;
  lsm_db *db;

  /* Open input file */
  FILE *fp = fopen(INPUT_FILE, "r");
  if (fp == NULL) exit(1);

  /* Allocate a new database handle */
  rc = lsm_new(0, &db);
  if( rc!=LSM_OK ) exit(1);

#ifdef USE_LSM_LZ4_COMPRESSOR
  rc = lsm_lz4_config(db);
  if( rc!=LSM_OK ) exit(1);
#endif /* USE_LZ4_LSM_COMPRESSOR */

  /* Connect the database handle to database "test.db" */
  rc = lsm_open(db, DATABASE_NAME);
  if( rc!=LSM_OK ) exit(1);

  /* Insert all the lines from the input file */
  printf("Inserting all keys/values (using %s)\n", INPUT_FILE);

  char vbuf[BUFFER_SIZE] = {0,};
  char kbuf[BUFFER_SIZE] = {0,};
  int i = 0;
  while (fgets(vbuf, BUFFER_SIZE, fp) != NULL) {
    /* Remove string newlines */
    int vlen = strlen(vbuf);
    if (vlen <= 0) continue;
    if (vbuf[vlen-1] == '\n') vbuf[vlen-1] = '\0';

    /* Create bogus key value */
    snprintf(kbuf, BUFFER_SIZE, "KEY INPUT 0x%x", i);

#ifndef NDEBUG
    printf("insert: key = '%s', val = '%s'\n", kbuf, vbuf);
#endif /* !NDEBUG */
    rc = lsm_insert(db, kbuf, strlen(kbuf), vbuf, strlen(vbuf));
    if (rc != LSM_OK) {
      fprintf(stderr, "ERROR WRITING KEY 0x%x: '%s' -> '%s', rc = %d\n", i, kbuf, vbuf, rc);
      exit(-1);
    }

    i++; /* Increment index */
  }

  /* Get them all back to be sane */
  printf("Done. Now retrieving keys...\n");
  lsm_cursor *csr;
  rc = lsm_csr_open(db, &csr);
  if (rc != LSM_OK) exit(1);

  for (int j = 0; j < i; j++)
  {
    snprintf(kbuf, BUFFER_SIZE, "KEY INPUT 0x%x", j);
#ifndef NDEBUG
    printf("retrieve: key = '%s'\n", kbuf);
#endif /* !NDEBUG */
    rc = lsm_csr_seek(csr, kbuf, strlen(kbuf), LSM_SEEK_EQ);
    if (!lsm_csr_valid(csr)) {
      fprintf(stderr, "ERROR READING KEY 0x%x: '%s', rc = %d\n", j, kbuf, rc);
      exit(-1);
    }
  }
  lsm_csr_close(csr);
  printf("Done.\n");

  fclose(fp);
  rc = lsm_close(db);
  return 0;
}
