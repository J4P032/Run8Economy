#ifndef COPIAARCHIVOTEXTO_H
#define COPIAARCHIVOTEXTO_H


#include <fstream>
#include <Windows.h>
#include <ctime>

using namespace std;

string leeXML(string archivoXML) {
	ifstream file(archivoXML);  //esto abre el archivo VernometerData.xml
	string xmlString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); //esto asigna todo el contenido desde el primer caracter al ultimo del xml a la variable xmlString
	file.close(); //esto cierra el archivo. Hay que hacerlo.
    return xmlString;
}

int horaActual() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    int anyo = ltm.tm_year + 1900;
    int mes = ltm.tm_mon + 1;
    int dia = ltm.tm_mday;
    int hora = ltm.tm_hour;
    int minuto = ltm.tm_min;
    int segundo = ltm.tm_sec;
    int hora_actual = anyo * 100000000 + mes * 1000000 + dia * 10000 + hora * 100 + minuto;
    return hora_actual;
}

int horaUltimaModificacion() {
    struct stat result;
    if (stat("./V3Routes/Regions/SouthernCA/AutoSaves/Auto Save Train.xml", &result) == 0) {
        std::time_t mod_time = result.st_mtime;
        std::tm mod_tm;
        localtime_s(&mod_tm, &mod_time);
        int anyo = mod_tm.tm_year + 1900;
        int mes = mod_tm.tm_mon + 1;
        int dia = mod_tm.tm_mday;
        int hora = mod_tm.tm_hour;
        int minuto = mod_tm.tm_min;
        int segundo = mod_tm.tm_sec;
        int hora_mod = anyo * 100000000 + mes * 1000000 + dia * 10000 + hora * 100 + minuto;
        return hora_mod;
    }
    else {
        std::cerr << "Could not obtain hour of last modification of the file" << std::endl;
        return -1;
    }

}

#endif



/*
Para obtener la hora del sistema Windows en C++, puedes utilizar la función GetLocalTime de la API de Windows. Esta función obtiene la fecha y hora del sistema local y la devuelve en una estructura SYSTEMTIME.

Para obtener la fecha y hora de creación y modificación del archivo XML, puedes utilizar las funciones GetFileTime de la API de Windows. Esta función obtiene la fecha y hora de creación, acceso y modificación de un archivo y la devuelve en una estructura FILETIME.

Una vez que tengas las fechas y horas, puedes calcular la diferencia en minutos utilizando la función SystemTimeToVariantTime y FileTimeToSystemTime.

Aquí te dejo un ejemplo de cómo podrías modificar tu función leeXML para que realice esta comparación:

#include <Windows.h>
#include <fstream>
#include <string>
using namespace std;

string leeXML(string archivoXML) {
    ifstream file(archivoXML);
    string xmlString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Obtener la hora del sistema
    SYSTEMTIME currentTime;
    GetLocalTime(&currentTime);

    // Obtener la fecha y hora de creación y modificación del archivo XML
    HANDLE hFile = CreateFileA(archivoXML.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    FILETIME creationTime, lastAccessTime, lastWriteTime;
    GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime);
    CloseHandle(hFile);

    // Convertir las fechas y horas a minutos
    double currentMinutes, fileMinutes;
    SystemTimeToVariantTime(&currentTime, &currentMinutes);
    FileTimeToSystemTime(&lastWriteTime, &currentTime);
    SystemTimeToVariantTime(&currentTime, &fileMinutes);

    // Comparar la diferencia en minutos
    if (fileMinutes + 3.0 / (24 * 60) < currentMinutes) {
        return "";
    }

    return xmlString;
}

Ten en cuenta que esta implementación utiliza la función CreateFileA, que es específica de Windows, por lo que no será portátil a otros sistemas operativos.
Si necesitas portabilidad, tendrás que utilizar bibliotecas multiplataforma como Boost o Qt para obtener la fecha y hora del sistema y la fecha y hora de
creación y modificación del archivo.



*/