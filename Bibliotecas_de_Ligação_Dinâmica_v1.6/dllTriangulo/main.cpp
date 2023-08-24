#include "main.h"
#include <math.h>

void DLL_EXPORT Triangulo(struct stTriangulo *tri)
{
    if (((tri->lado1 > abs(tri->lado2 - tri->lado3)) && (tri->lado1 < tri->lado2 + tri->lado3)) &&
        ((tri->lado2 > abs(tri->lado1 - tri->lado3)) && (tri->lado2 < tri->lado1 + tri->lado3)) &&
        ((tri->lado3 > abs(tri->lado1 - tri->lado2)) && (tri->lado3 < tri->lado1 + tri->lado2)))
    {
        if (tri->lado1 == tri->lado2 && tri->lado2 == tri->lado3)
            tri->tipo = EQUILATERO;
        else
        {
            if (tri->lado1 != tri->lado2 && tri->lado1 != tri->lado3 && tri->lado2 != tri->lado3)
                tri->tipo = ESCALENO;
            else
                tri->tipo = ISOSCELES;
        }
        tri->perimetro = tri->lado1 + tri->lado2 + tri->lado3;
        tri->semiperimetro = (double)(tri->perimetro)/2;
        tri->area = sqrt((tri->semiperimetro)*(tri->semiperimetro - tri->lado1)*
                         (tri->semiperimetro - tri->lado2)*(tri->semiperimetro - tri->lado3));
    }
    else
    {
        tri->tipo = NAO_EH_TRI;
    }
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
