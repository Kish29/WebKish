//
// Created by 蒋澳然 on 2021/8/7.
// email: 875691208@qq.com
// $desc
//

#include "http_server.h"
#include "http_interface.h"
#include "dirent.h"
#include "sys/stat.h"

class file_resolver : public kish::http_interface {

public:
    file_resolver() {

    }

private:

};


int main(int argc, char *argv[]) {
    if (argc < 2) exit(EXIT_SUCCESS);
//    const char *file_path = argv[1];
    const char *file_path = ".";
    struct stat s_buf{};

    stat(file_path, &s_buf);

    if (S_ISDIR(s_buf.st_mode)) {
        printf("[%s] is a directory\n", file_path);

        struct dirent *filename{};
        DIR *dir_ptr = opendir(file_path);
        int i = 0;
        while ((filename = readdir(dir_ptr)) != nullptr) {
            char buf[200];
            bzero(buf, 200);
//            strcat(buf, file_path);
            strcat(buf, "/");
            strcat(buf, filename->d_name);

            stat(filename->d_name, &s_buf);
            if (S_ISDIR(s_buf.st_mode)) {
                printf("[%s] is a directory\n", buf);
            } else if (S_ISREG(s_buf.st_mode)) {
                printf("[%s] is a normal file\n", buf);
                FILE *fptr = fopen(filename->d_name, "r");
                setvbuf(fptr, nullptr, _IOFBF, 8196);
                if (fptr) {
                    char buf2[512];
                    size_t readn;
                    do {
                        bzero(buf2, 512);
                        readn = fread(buf2, 1, 512, fptr);
                        printf("buf2 is %s\n", buf2);
                    } while (readn);

                    fclose(fptr);
                }
            }

            i++;
        }
        printf("total file num is %d\n", i);
        closedir(dir_ptr);

    } else if (S_ISREG(s_buf.st_mode)) {
        printf("[%s] is a normal file!\n", file_path);
    }

}
