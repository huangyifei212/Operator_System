/**
 * @file    simplefs.h
 * @brief   Setup in FAT16 file system.
 * @details Macro definitions, structs such as FCB and FAT, and some global variable.
 * @author  Leslie Van
 * @date    2018-12-19 to
 */

#ifndef OPERATOR_SYSTEM_EXP4_SIMPLEFS_H
#define OPERATOR_SYSTEM_EXP4_SIMPLEFS_H
#define BLOCK_SIZE      1024
#define BLOCK_NUM       1024
#define DISK_SIZE       1048576
#define SYS_PATH        "./fsfile"
#define END             0xffff  /**< End of the block, a flag in FAT. */
#define FREE            0x0000  /**< Unused block, a flag in FAT. */
#define ROOT            "/"     /**< Root directory name.*/
#define ROOT_BLOCK_NUM  2       /**< Block of the initial root directory. */
#define MAX_OPENFILE    10      /**< Max files to open at the same time. */
#define NAMELENGTH      24
#define PATHLENGTH      128
#define DELIM           "/"
#define FOLDER_COLOR    "\e[1;32m"
#define DEFAULT_COLOR   "\e[0m"

/**
 * @brief Store virtual disk information.
 * Contain info like block size, block count and some other information about disk.
 */
typedef struct BLOCK0 {
    char information[200];
    unsigned short root;        /**< Block number of the root directory. */
    unsigned char *start_block; /**< Location of the first data block. */
} block0;

/**
 * @brief File control block.
 * Store file info both the description and current state.
 */
typedef struct FCB {
    char filename[8];
    char exname[3];
    unsigned char attribute;    /**< 0: directory or 1: file. */
    unsigned char reserve[10];
    unsigned short time;        /**< File create time. */
    unsigned short date;        /**< File create date. */
    unsigned short first;       /**< First block num of the file. */
    unsigned long length;       /**< Block count of the file. */
    char free;
} fcb;

/**
 * @brief File allocation table.
 * Record the next block num of file.
 * When value is 0xffff, this block is the last block of the file.
 */
typedef struct FAT {
    unsigned short id;
} fat;

/**
 * @brief A file entry opened by user.
 * Contain file control block and current state.
 */
typedef struct USEROPEN {
    /** FCB. */
    fcb open_fcb;
    /** Current state. */
    char dir[80];
    int count;
    char fcb_state;
    char free;
} useropen;

/** Global variables. */
unsigned char *fs_head;         /**< Initial address of the virtual disk. */
useropen openfile_list[MAX_OPENFILE];   /**< File array opened by user. */
int curdir;                     /**< File descriptor of current directory. */
char current_dir[80];           /**< Current directory name. */
unsigned char *start;           /**< Location of the first data block. */

/** Declaration of functions */
int start_sys(void);

int my_format(char **args);

int do_format(void);

int my_cd(char **args);

void do_chdir(int fd);

int my_pwd(char **args);

int my_mkdir(char **args);

int do_mkdir(char *dirname);

int my_rmdir(char **args);

int do_rmdir();

int my_ls(char **args);

void do_ls(int first, char mode);

int my_create(char **args);

int do_create(const char *path);

int my_rm(char **args);

int do_rm();

int my_open(char **args);

int do_open(char *filename);

int my_close(char **args);

int do_close(int fd);

int my_write(char **args);

int do_write(int fd, char *text, int len, char mode);

int my_read(char **args);

int do_read(int fd, int len, char *text);

int my_exit_sys();

int get_free(int count);

int set_free(unsigned short first, unsigned short length, int mode);

int set_fcb(fcb *f, char *filename, char *exname, unsigned char attr, unsigned short first, unsigned long length,
            char ffree);

unsigned short get_time(struct tm *timeinfo);

unsigned short get_date(struct tm *timeinfo);

int fcb_cpy(fcb *dest, fcb *src);

int get_abspath(char *abspath, const char *relpath);

int get_useropen();

fcb *find_fcb(const char *path);

fcb *find_fcb_r(char *token, int root);

void init_folder(int first, int second);

void get_fullname(char *fullname, fcb *fcb1);

#endif //OPERATOR_SYSTEM_EXP4_SIMPLEFS_H