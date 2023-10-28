/** my first test
 *
 * by Nitnelav00
 * 30/09/2023
 *
 */

#include "raylib.h"
#include <stdio.h>

#include "render.h"
#include "cpu.h"

int main()
{
    Render *rend = InitRender();
    CPU *cpu = InitCPU(rend, "aaaa.bin", 10);

    InitWindow(1024, 512, "emu test");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
            CycleCPU(cpu);
    }

    printf("1");
    FreeRender(rend);
    printf("2");
    FreeCPU(cpu);
    printf("3");
    CloseWindow();

    return 0;
}
