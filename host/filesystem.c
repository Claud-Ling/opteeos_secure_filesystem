#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <tee_client_api.h>
#include "trusted_key_manager_ta.h"
#include <stdio.h>
#include <err.h>

static int getattr_callback(const char *path, struct stat *stbuf) {
  return 0;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi) {
  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi) {
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
        struct fuse_file_info *fi) {
  return 0;
}

static struct fuse_operations operations = {
  .getattr = getattr_callback,
  .open = open_callback,
  .read = read_callback,
  .readdir = readdir_callback,
};

int main(int argc, char *argv[]){
//  return fuse_main(argc, argv, &operations, NULL);
  TEEC_UUID ta_id = TRUSTED_KEY_MANAGER_UUID;
  TEEC_Result res;
  TEEC_Context ctx;
  TEEC_Session sess;
  TEEC_Operation op;
  uint32_t err_origin;
  int i;

  unsigned char keys[48];
  unsigned char data[16];
  memset(keys, 0, 48);
  memset(data, 0, 16);

  TEEC_SharedMemory dataBuf;
  dataBuf.size = 16;
  dataBuf.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
  dataBuf.buffer = data;

  TEEC_SharedMemory keysBuf;
  keysBuf.size = 48;
  keysBuf.flags = TEEC_MEM_INPUT;
  keysBuf.buffer = keys;

  res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
  }

  res = TEEC_OpenSession(&ctx, &sess, &ta_id, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
  }

  res = TEEC_RegisterSharedMemory(&ctx, &dataBuf);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_RegisterSharedMemory failed with code 0x%x origin 0x%x", res, err_origin);
  }

  res = TEEC_RegisterSharedMemory(&ctx, &keysBuf);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_RegisterSharedMemory failed with code 0x%x origin 0x%x", res, err_origin);
  }

  memset(&op, 0, sizeof(op));
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_WHOLE, TEEC_MEMREF_WHOLE,
					 TEEC_NONE, TEEC_NONE);

  op.params[0].memref.parent = &keysBuf;
  op.params[1].memref.parent = &dataBuf;

  res = TEEC_InvokeCommand(&sess, ENCRYPT_VALUE, &op, NULL);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }

  printf("Output data: 0x");
  for(i=0; i<16; i++){
    printf("%02x", data[i]);
  }
  printf("\n");
}
