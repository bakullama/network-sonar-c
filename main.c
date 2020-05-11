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
//    char* output[254];
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



int main()
{
    srand((unsigned int)time(NULL));
    unsigned char* doubleBuffer = (unsigned char *) malloc(640*480); // 300 kb
    if (doubleBuffer == NULL) {
        printf("Not enough memory for double buffer.\n");
        exit(1);
    }


    int gd = DETECT;
    int gm;
    XInitThreads();
    char command[104] = "for ip in `seq 1 254`; do ping -c 1 10.0.0.$ip | grep '64 bytes' | cut -d ' ' -f 4 | tr -d ':' & done";
    char *networkIPs;
    networkIPs = getCommandOutput(command);

//    printf(&networkIPs[0]);

//    char str[5];
    int devices = 0;
    for (int i = 0; i < 4064; i = i + 16) {
        if (strlen(&networkIPs[i]) != 0) {
            devices++;
        }
//        sprintf(str, "%d", i);
//        str[4] = '\n';
//        printf(str);
//        fflush(stdout);

        printf(&networkIPs[i]);


    }

    initgraph(&gd,&gm,NULL);
    struct coordinate centre;
    centre.x = getmaxx() / 2; // 320
    centre.y = getmaxy() / 2; // 240
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








    while (1){
//        getCommandOutput("ls");

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

    }


//    delay(5000);
    closegraph();

    return 0;
}
