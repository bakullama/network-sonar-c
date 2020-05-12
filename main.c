#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


#define pi 3.141592653589793238462643383279502884197169399375105820974
#define BUFSIZE 128

struct coordinate {
    double x;
    double y;
};

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char **getcommandoutput(char *cmd) {
    char* output = malloc(sizeof(char) * 4064);
    char* outputline;
    char buf[BUFSIZE];
    int counter = 0;
    FILE *fp;

    fp = popen(cmd, "r");

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        outputline = buf;
        strcpy(&output[counter * 16], outputline);
        counter++;
    }

    return output;
}


int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Invalid number of arguments.\nUsage: ./network-sonar-c <first 3 octets of ip, e.g. 192.168.1>\n");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL));
    unsigned char* doublebuffer = (unsigned char *) malloc(307200); // 300 kb

    if (doublebuffer == NULL) {
        printf("Not enough memory for double buffer.\n");
        exit(EXIT_FAILURE);
    }

    char *iprange = argv[1];
    char command[140] = "for ip in `seq 1 254`; do ping -c 1 ";
    strcat(command, iprange);
    strcat(command, ".$ip | grep '64 bytes' | cut -d ' ' -f 4 | tr -d ':' & done");
    fflush(stdout);
    printf("Running command: '%s' \n", command);

    char *networkips;
    networkips = getcommandoutput(command);

    int devices = 0;
    for (int i = 0; i < 4064; i = i + 16) {
        if (strlen(&networkips[i]) != 0) {
            devices++;
        }
    }

    fflush(stdout);
    printf("Devices collected.\nDisplaying data.");

    int graphdriver = DETECT;
    int graphmode;
    initgraph(&graphdriver, &graphmode, NULL);
    struct coordinate centre;
    centre.x = getmaxx() / 2;
    centre.y = getmaxy() / 2;
    int radius = 230;

    int pointprecision = 360;

    struct coordinate circumferencepoints[pointprecision];
    long double shift = (pi / 180);

    for (int i = 0; i < pointprecision; ++i) {
        circumferencepoints[i].x = centre.x + cos(shift * i) * radius;
        circumferencepoints[i].y = centre.y + sin(shift * i) * radius;
    }


    struct coordinate points[devices];
    for (int i = 0; i < devices; ++i) {
        points[i].x = centre.x + (rand() % 340) - 170;
        points[i].y = centre.y + (rand() % 340) - 170;
    }

    int stop = 0;

    while (!stop){
        setcolor(GREEN);

        for (int j = 0; j < devices; ++j) {
            circle(points[j].x + 50, points[j].y, 5);
            floodfill(points[j].x + 50, points[j].y, GREEN);
            outtextxy(points[j].x + 55, points[j].y, &networkips[j*16]);
        }

        for (int i = 0; i < pointprecision; ++i) {

            cleardevice();
            line(centre.x, centre.y, circumferencepoints[i].x, circumferencepoints[i].y);
            circle(centre.x, centre.y, radius);


            delay(50);
        }



    }

    closegraph();

    printf("Exiting.");
    return 0;
}
