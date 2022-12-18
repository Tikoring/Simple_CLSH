#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <spawn.h>
#include <fcntl.h>

#define MAX_NODE 999        // clsh로 연결 가능한 최대 노드 갯수
#define MAX_COMMAND 999     // clsh로 호출 가능한 최대 명령어 단어 갯수
#define MAX_NAME 999        // clsh로 연결 가능한 노드 이름의 최대 길이

static int optionNum;
int main (int argc, char **argv) {
    int optIndex = 0;                                                           // opts에 해당하는 index값을 얻기 이ㅜ함
    char *nodes = NULL, *nodeFile = NULL;                                       // -h 옵션과 --hostfile 옵션의 arg를 입력받기 위한 포인터
    char *nodeList[MAX_NODE];                                                   // 실제 node들의 이름을 저장하는 array
    int pidList[MAX_NODE];                                                      // 각 ssh의 pid를 저장하는 array
    int pipeListIn[MAX_NODE][2], pipeListOut[MAX_NODE][2];                      // clsh로 들어오는 pipe를 저장할 array, clsh로 나가는 pipe를 저장할 array
    extern int optind, optopt;                                                  // getopt 에서 사용되는 extern 변수들
    struct option opts[] = {
        {"hostfile", required_argument, 0, 1},                                  // flag에 0을 설정해서 getopt_long의 return으로 1을 입력받도록 조정
        {0, 0, 0, 0}
    };

    // option 처리
    while ((optionNum = getopt_long (argc, argv, "h:", opts, &optIndex)) != -1) {
        switch (optionNum) {
            // --hostfile 옵션
            case 1:
                nodeFile = (char *) malloc ((strlen (optarg) + 1) * sizeof (char));
                strcpy (nodeFile, optarg);
                printf ("%s\n", nodeFile);
                break;
            // -h 옵션
            case 'h':
                nodes = (char *) malloc ((strlen (optarg) + 1) * sizeof (char));
                strcpy (nodes, optarg);
                break;
            default:
                // 옵션 인자가 주어지지 않은 경우
                if (optopt == 1 || optopt == 'h') return 1;
                // 옵션이 주어지지 않은 경우
                return 2;
        }
    }
    
    // 아무런 옵션이 지정되지 않은 경우 처리
    if (nodes == NULL && nodeFile == NULL) {
        char *env;
        // CLSH_HOSTS 환경변수가 존재하는 경우
        if (env = getenv ("CLSH_HOSTS")) {
            nodes = strtok (env, "=");
            nodes = strtok (NULL, "=");
            printf ("%s\n", env);
        // CLSH_HOSTFILE 환경변수가 존재하는 경우
        } else if (env = getenv ("CLSH_HOSTFILE")) {
            nodeFile = strtok (env, "=");
            nodeFile = strtok (NULL, "=");
            printf ("%s\n", env);
        // .hostfile이 존재하는 경우 처리
        } else {
            DIR *current = opendir (getcwd (NULL, BUFSIZ));
            struct dirent *lists;
            while (lists = readdir (current)) {
                if (strcmp (lists->d_name, ".hostfile") == 0) {
                    nodeFile = (char *) malloc ((strlen (lists->d_name) + 1) * sizeof (char));
                    strcpy (nodeFile, lists->d_name);
                    printf ("%s\n", nodeFile);
                }
            }

            if (nodeFile == NULL) {
                printf ("ERROR: --hostfile 옵션이 제공되지 않았습니다\n");
                return 1;
            }
        }
    }
    
    if (nodeFile != NULL && optind < argc) {
        char *commands[MAX_COMMAND];
        FILE *fp;
        int idx = 0;
        if ((fp = fopen (nodeFile, "r")) == NULL) {
            perror ("파일 열기에 실패했습니다");
            return 2;
        }

        while (fgets (nodeList[idx++], MAX_NAME, fp));
        fclose (fp);

        for (int i = 0; i < idx; i++) {

        }
        
    } else if (nodes != NULL && optind < argc) {
        char *commands[MAX_COMMAND];
        printf ("%s\n", nodes);
    } else if (optind >= argc) {
        printf ("ERROR: 명령어가 제공되지 않았습니다\n");
        return 10;
    } else {
        printf ("ERROR: 식별되지 않은 에러\n");
        return 100;
    }


    if (nodeFile != NULL) {
        free (nodeFile);
    }
    if (nodes != NULL) {
        free (nodes);
    }

    return 0;
}