#include <stdio.h>
#include <getopt.h>

static int longFlag, shortFlag;
int main (int argc, char *argv[]) {
    int optIndex = 0;
    struct option opts[] = {
        {"hostfile", required_argument, &longFlag, 1},
        {0, 0, 0, 0}
    };

    while ((shortFlag = getopt_long (argc, argv, "h:", opts, &optIndex)) != -1) {
        switch (shortFlag) {
            case 0:
                printf ("%s\n", optarg);
                break;
            case 'h':
                printf ("%s\n", optarg);
                break;
            default :
                printf ("wrong options\n");
                break;
        }
    }
    

    return 0;
}