#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>

static int getattr_callback(const char *path, struct stat *stbuf) {
  return NULL;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi) {
  return NULL;
}

static int open_callback(const char *path, stuct fuse_file_info *fi) {
  return NULL;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
        struct fuse_file_info *fi) {
  return NULL;
}

static struct fuse_operations operations = {
  .getattr = getattr_callback,
  .open = open_callback,
  .read = read_callback,
  .readdir = readdir_callback,
};

int main(int argc, char *argv[]){
  return fuse_main(argc, argv, &operations, NULL);
}
