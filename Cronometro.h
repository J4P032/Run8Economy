#ifndef CRONOMETRO_H
#define CRONOMETRO_H
#include <chrono>
#include <Windows.h>
using namespace std;


bool cronometro(int duracion_ms, char idioma)
{
    static std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now(); //punto de inicio
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::steady_clock::duration elapsed = now - start;
    std::chrono::milliseconds elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    if (idioma == 's'){
        cout << "Contador de Tiempo: " << elapsed_ms.count() / 1000 << " Segundos de 2 minutos total" << endl;
    }
    if (idioma == 'e'){
        cout << "Time elapsed: " << elapsed_ms.count() / 1000 << " Seconds of 2 minutes in total" << endl;
    }
    if (elapsed >= std::chrono::milliseconds(duracion_ms)) {  //cuando sea igual o mayor se dará un true, si no, false
        start = now;
        return true;
    }
    else {
        return false;
    }
}


#endif;


/*a esto en el main habrá que ponerle algo como esto o por el estilo.
while (!cronometro(duracion_ms)) {
    // do nothing, just wait
}
*/