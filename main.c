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


char **getcommandoutput(char *cmd) {
    char* output = malloc(sizeof(char) * 4064);
    char* outputline;
    char buf[BUFSIZE];
    FILE *fp;

    fp = popen(cmd, "r");

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        outputline = buf;
        strcpy(&output[counter * 16], outputline);
    }

    return output;
}

void generatepoints(struct coordinate* points, int devices, struct coordinate centre) {

    for (int i = 0; i < devices; ++i) {
        points[i].x = centre.x + (rand() % 340) - 170;
        points[i].y = centre.y + (rand() % 340) - 170;
    }

}

void drawdevices(struct coordinate* points, int devices, char* networkips) {
    for (int j = 0; j < devices; ++j) {
        circle(points[j].x + 50, points[j].y, 5);
        floodfill(points[j].x + 50, points[j].y, GREEN);
        outtextxy(points[j].x + 55, points[j].y, &networkips[j*16]);
    }
}

void generatecirclepoints(struct coordinate* circumferencepoints, int pointprecision, int radius, struct coordinate centre) {
    long double shift = (pi / 180);

    for (int i = 0; i < pointprecision; ++i) {
        circumferencepoints[i].x = centre.x + cos(shift * i) * radius;
        circumferencepoints[i].y = centre.y + sin(shift * i) * radius;
    }

}


int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Invalid number of arguments.\nUsage: ./network-sonar-c <first 3 octets of ip, e.g. 192.168.1>\n");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL));

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
    struct coordinate points[devices];
    struct coordinate currentpoint;
    struct coordinate nextpoint;

    generatecirclepoints(circumferencepoints, pointprecision, radius, centre);
    generatepoints(points, devices, centre);

    while (1){
        setcolor(GREEN);

        for (int i = 0; i < pointprecision - 1; ++i) {
            drawdevices(points, devices, networkips);

            currentpoint.x = circumferencepoints[i].x;
            currentpoint.y = circumferencepoints[i].y;


            nextpoint.x = circumferencepoints[i+1].x;
            nextpoint.y = circumferencepoints[i+1].y;


            setcolor(BLACK);
            line(centre.x, centre.y, currentpoint.x, currentpoint.y);
            setcolor(GREEN);
            line(centre.x, centre.y, nextpoint.x, nextpoint.y);

            circle(centre.x, centre.y, radius);
            delay(50);

        }

        cleardevice();

    }

    closegraph();

    printf("Exiting.");
    return 0;
}
