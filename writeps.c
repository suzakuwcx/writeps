#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false 0
#define Parse(s) (strcmp(argv[i],(s)) == 0)

void usage();

int main(int argc,char *argv[]){
    bool show_help = false;
    bool no_enter = false;
    char *program_name = argv[0];
    char *message = NULL;
    char * pid = NULL;

    int fd = -1;
    char * proc_path = malloc(sizeof(char) * 30);

    for (int i = 1; i < argc; ++i)
    {
        if(Parse("-h") || Parse("--help"))
            show_help = true;
        else if (Parse("-n"))
            no_enter = true;
        else if (argv[i][0] == '-')
        {
            fprintf(stderr, "%s: invalid option -- '%s' (use -h for help) \n", program_name,argv[i]);
            usage();
            return EXIT_FAILURE;
        }
        else
        {
            char temp = 1;
            int j = 0;
            //check if pid is valid
            while (temp != '\0')
            {
                temp = argv[i][j];
                ++j;
                if(!(temp >= '0' && temp <= '9' || temp == '\0'))
                {
                    fprintf(stderr, "%s: invalid pid '%s' (use -h for help) \n", program_name,argv[i]);
                    return EXIT_FAILURE;
                }
            }
            pid = argv[i];

            ++i;
            if (i > argc - 1)
            {
                fprintf(stderr, "%s: no message specified (use -h for help) \n", program_name);
                return EXIT_FAILURE;
            }
            if (i < argc - 1)
            {
                fprintf(stderr, "%s: too many argument (use -h for help) \n", program_name);
                return EXIT_FAILURE;
            }
            message = argv[i];
        }
    }

    if (argc == 1 || show_help == true)
    {
        usage();
        return EXIT_SUCCESS;
    }
    //open stdin in /proc/xxx/fd/0
    strcat(proc_path,"/proc/");
    strcat(proc_path,pid);
    strcat(proc_path,"/fd/0");

    fd  = open(proc_path,O_RDONLY);

    while (*message != '\0')
    {
        if (ioctl(fd, TIOCSTI, message))
        {
            perror("ioctl");
            return EXIT_FAILURE;
        }
        ++message;
    }

    if (!no_enter)
    {
        char enter = 10;
        if (ioctl(fd, TIOCSTI,&enter))
        {
            perror("ioctl");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

void usage(){
    printf("\n"
            "Usage: writeps [optional...] <pid> <message>"                  "\n"
                                                                            "\n"
            "Send a message to a process stdin"                             "\n"
                                                                            "\n"
            "Options:"                                                      "\n"
            "    -h,   --help                 display this help and exit"   "\n"
            "    -n                           do not append ENTER behind"   "\n"
    );
}