#include <graphics.h>
#include <X11/Xlib.h>
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

char **getCommandOutput(char *cmd) {
    char* output = malloc(sizeof(char) * 4064);
    char* outputLine;
    char buf[BUFSIZE];
    int counter = 0;
    FILE *fp;

    fp = popen(cmd, "r");

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        outputLine = buf;
        strcpy(&output[counter * 16], outputLine);
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

    int gd = DETECT;
    int gm;

    srand((unsigned int)time(NULL));
    unsigned char* doubleBuffer = (unsigned char *) malloc(307200); // 300 kb

    if (doubleBuffer == NULL) {
        printf("Not enough memory for double buffer.\n");
        exit(EXIT_FAILURE);
    }

    char *ipRange = argv[1];
    char command[140] = "for ip in `seq 1 254`; do ping -c 1 ";
    strcat(command, ipRange);
    strcat(command, ".$ip | grep '64 bytes' | cut -d ' ' -f 4 | tr -d ':' & done");
    fflush(stdout);
    printf("Running command: '%s' \n", command);

    char *networkIPs;
    networkIPs = getCommandOutput(command);

    int devices = 0;
    for (int i = 0; i < 4064; i = i + 16) {
        if (strlen(&networkIPs[i]) != 0) {
            devices++;
        }
    }

    fflush(stdout);
    printf("Devices collected.\nDisplaying data.");
    initgraph(&gd, &gm, NULL);
    struct coordinate centre;
    centre.x = getmaxx() / 2;
    centre.y = getmaxy() / 2;
    int radius = 230;

    int pointPrecision = 360;

    struct coordinate circumferencePoints[pointPrecision];
    long double shift = (pi / 180);

    for (int i = 0; i < pointPrecision; ++i) {
        circumferencePoints[i].x = centre.x + cos(shift * i) * radius;
        circumferencePoints[i].y = centre.y + sin(shift * i) * radius;
    }


    struct coordinate points[devices];
    for (int i = 0; i < devices; ++i) {
        points[i].x = centre.x + (rand() % 340) - 170;
        points[i].y = centre.y + (rand() % 340) - 170;
    }

    int stop = 0;

    while (!stop){

        for (int i = 0; i < pointPrecision; ++i) {
            setcolor(GREEN);
            cleardevice();
            line(centre.x, centre.y, circumferencePoints[i].x, circumferencePoints[i].y);
            circle(centre.x, centre.y, radius);
            for (int j = 0; j < devices; ++j) {
                circle(points[j].x + 50, points[j].y, 5);
                floodfill(points[j].x + 50, points[j].y, GREEN);
                outtextxy(points[j].x + 55, points[j].y, &networkIPs[j*16]);
            }
            delay(50);
        }

        stop = grgetche();

    }

    closegraph();

    printf("Exiting.");
    return 0;
}
