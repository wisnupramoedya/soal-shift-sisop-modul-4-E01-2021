#define FUSE_USE_VERSION 28
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static char *dirpath = "/home/wisnupramoedya/Downloads";
static char *logpath = "/home/wisnupramoedya/SinSeiFS.log";
static char *ATOZ = "AtoZ_";
static char *RX = "RX_";
const int INFO = 1;
const int WARNING = 2;

void log_v1(char *from, char *to) {
    int i;
    for (i = strlen(to); i >= 0; i--) {
        if (to[i] == '/') break;
    }
    if (strstr(to + i, ATOZ) == NULL) return;

    FILE *log_file = fopen(logpath, "a");
    fprintf(log_file, "%s -> %s\n", from, to);
}

void log_v2(char *str, int type) {
    FILE *log_file = fopen(logpath, "a");

    time_t current_time;
    time(&current_time);
    struct tm *time_info;
    time_info = localtime(&current_time);

    if (type == INFO) {
        fprintf(log_file, "INFO::%d%d%d-%d:%d:%d:%s\n", time_info->tm_mday,
                time_info->tm_mon, time_info->tm_year, time_info->tm_hour,
                time_info->tm_min, time_info->tm_sec, str);
    } else if (type == WARNING) {
        fprintf(log_file, "WARNING::%d%d%d-%d:%d:%d:%s\n", time_info->tm_mday,
                time_info->tm_mon, time_info->tm_year, time_info->tm_hour,
                time_info->tm_min, time_info->tm_sec, str);
    }
}

void atbash(char *str, int start, int end) {
    for (int i = start; i < end; i++) {
        if (str[i] == '/') continue;
        if (i != start && str[i] == '.') break;

        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = 'Z' + 'A' - str[i];
        else if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = 'z' + 'a' - str[i];
    }
}

void encode_atbash(char *str) {
    if (!strcmp(str, ".") || !strcmp(str, "..")) return;
    atbash(str, 0, strlen(str));

    printf("==== enc:atb:%s\n", str);
}

void decode_atbash(char *str) {
    if (!strcmp(str, ".") || !strcmp(str, "..")) return;
    if (strstr(str, "/") == NULL) return;
    printf("==== before:%s\n", str);
    int str_length = strlen(str), s = 0, i;
    for (i = str_length; i >= 0; i--) {
        if (str[i] == '/') break;

        if (str[i] == '.') {
            str_length = i;
            break;
        }
    }
    for (i = 0; i < str_length; i++) {
        if (str[i] == '/') {
            s = i + 1;
            break;
        }
    }

    atbash(str, s, str_length);
    printf("==== dec:atb:%s\n", str);
}

static int xmp_getattr(const char *path, struct stat *stbuf) {
    char *enc1 = strstr(path, ATOZ);
    printf("getattr:%s=enc:%s\n", path, enc1);
    if (enc1 != NULL) {
        printf("YEY::getattr:%s\n", path);
        decode_atbash(enc1);
    }

    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    printf("== getattr::fpath:%s\n", fpath);
    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi) {
    char *enc1 = strstr(path, ATOZ);
    printf("readdir:%s=enc:%s\n", path, enc1);
    if (enc1 != NULL) {
        printf("YEY::readdir:%s\n", path);
        decode_atbash(enc1);
    }

    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    } else
        sprintf(fpath, "%s%s", dirpath, path);

    int res = 0;
    DIR *dp;
    struct dirent *de;
    (void)offset;
    (void)fi;
    dp = opendir(fpath);
    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (enc1 != NULL) {
            encode_atbash(de->d_name);
            printf("== readdir::encode:%s\n", de->d_name);
        }

        res = (filler(buf, de->d_name, &st, 0));

        if (res != 0) break;
    }

    closedir(dp);
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode) {
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    } else
        sprintf(fpath, "%s%s", dirpath, path);

    log_v1(dirpath, fpath);

    int res = mkdir(fpath, mode);

    char str[100];
    sprintf(str, "MKDIR::%s", path);
    log_v2(str, INFO);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev) {
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    } else
        sprintf(fpath, "%s%s", dirpath, path);

    int res;
    if (S_ISREG(mode)) {
        res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
        if (res >= 0) res = close(res);
    } else if (S_ISFIFO(mode))
        res = mkfifo(fpath, mode);
    else
        res = mknod(fpath, mode, rdev);

    char str[100];
    sprintf(str, "CREATE::%s", path);
    log_v2(str, INFO);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_unlink(const char *path) {
    char *enc1 = strstr(path, ATOZ);
    printf("unlink:%s=enc:%s\n", path, enc1);
    if (enc1 != NULL) {
        printf("YEY::unlink\n");
        decode_atbash(enc1);
    }

    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    } else
        sprintf(fpath, "%s%s", dirpath, path);

    int res = unlink(fpath);

    char str[100];
    sprintf(str, "REMOVE::%s", path);
    log_v2(str, WARNING);

    if (res == -1) return -errno;

    return 0;
}

static int xmp_rmdir(const char *path) {
    char *enc1 = strstr(path, ATOZ);
    printf("rmdir:%s=enc:%s\n", path, enc1);
    if (enc1 != NULL) {
        printf("YEY::rmdir:%s\n", path);
        decode_atbash(enc1);
    }

    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = rmdir(fpath);

    char str[100];
    sprintf(str, "RMDIR::%s", path);
    log_v2(str, WARNING);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_rename(const char *from, const char *to) {
    printf("rename\n");
    char fileFrom[1000], fileTo[1000];
    sprintf(fileFrom, "%s%s", dirpath, from);
    sprintf(fileTo, "%s%s", dirpath, to);

    log_v1(fileFrom, fileTo);

    int res = rename(fileFrom, fileTo);

    char str[100];
    sprintf(str, "RENAME::%s::%s", from, to);
    log_v2(str, INFO);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi) {  // open file
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res;
    res = open(fpath, fi->flags);
    if (res == -1) return -errno;

    close(res);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;

        sprintf(fpath, "%s", path);
    } else
        sprintf(fpath, "%s%s", dirpath, path);

    int res = 0;
    int fd = 0;

    (void)fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;

    close(fd);
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi) {  // write file

    char newPath[1000];
    sprintf(newPath, "%s%s", dirpath, path);

    int res = 0;
    int fd = 0;

    (void)fi;
    fd = open(newPath, O_WRONLY);
    if (fd == -1) return -errno;

    char str[100];
    sprintf(str, "WRITE::%s", path);
    log_v2(str, INFO);

    res = pwrite(fd, buf, size, offset);
    if (res == -1) res = -errno;

    close(fd);
    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .mknod = xmp_mknod,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,
    .open = xmp_open,
    .write = xmp_write,
};

int main(int argc, char *argv[]) {
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}