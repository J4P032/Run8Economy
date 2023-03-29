//#include <iostream>
//#include <fstream>
#include <Windows.h>
#include <conio.h>
//#include <string>
//#include <sstream>
//#include <vector>
//#include <stdlib.h>

#include "getTotalValues.h"
#include "writeEconomiaRun8TXT.h"
#include "workingValue.h"
#include "valorInicial.h"
#include "copiaArchivoTexto.h"
#include "abrirVentana.h"
#include "Cronometro.h"
#include "TrabajoLocomotoras.h"
#include "LugarVehiculo.h"
#include "Mercado.h"
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif 

using namespace std;




int main() {
    // Leer el archivo Run8Economy.txt para obtener los valores iniciales de las etiquetas registradas en el archivo de texto
    bool running = true;
    double initialTotalHours = 0.0;
    double initialTotalEstimatedMiles = 0.0;
    double initialTotalFuelUsed = 0.0;
    double initialBanco = 0.0;
    int trenIDUsuario = 0;
    int locoIDUsuario[2] = { 0, 0 };
    int tiempoCrono = 0; //milisegundos
    std::vector<LocomotoraTrabajo> locosTrabajo(1, { 0,"N", 0, 0, 0.0, 0.0 }); //tengo que inicializarlo o si no hará una excepción de memoria.
    vector<pair<int, double>> distRunLocoVector(1, make_pair(0, 0.0));
    double distRunLocoActiva = 0.0;
    bool distRunLocoVectorEncontrado = false;


    bool trabajoEsTrim = false;
    int vagonAux = 0;

    double cargaAux = 0;

    bool condicion1Trim = false;
    bool condicion2Trim = false;
    bool llaveTrim = false;
    double momentoTrabajoTrim = 0;
    bool trabajoEsLocal = false;
   
    int LnumVagAux = 0;
    int LnumVagAuxTrim = 0;
    bool llaveLocal = false;
    bool llaveTrim2 = false;
    vector<cargaVagones> idsVagones;
    double pesoAuxLocal = 0.0;
    double pesoTotalTrim = 0.0;
    string typeWork = "N";
    double distRunLoco = 0.0;
    char idioma = '0';
    bool idiomaOk = false;
    bool mercadoOK = false;
    char mercado = '0';

    //Preguntamos el idioma del programa
    while (!idiomaOk) {
        cout << "(E)nglish or (S)panish ?";
        idioma = tolower(_getch());
        if ((idioma == 'e') || (idioma == 's')) { idiomaOk = true; }
        system(CLEAR); //Limpia la consola definido en el #define CLEAR "cls"
    }
          
    //Preguntamos si quiere hacer el archivo (UnitNotesDatabase.xml) de mercado, ya que puede tardar mucho
    while (!mercadoOK) {
        
        if (idioma == 's') {
            cout << "Quiere construir el archivo de Mercado? (S/N)" << endl;
            cout << "Dependiendo de la complejidad del mundo, puede llevar bastante tiempo." << endl;
            mercado = tolower(_getch());
            if ((mercado == 's') || (mercado == 'n')) { mercadoOK = true; }
            system(CLEAR);
        }
        if (idioma == 'e') {
            cout << "Do you want to build the market file (Y/N)" << endl;
            cout << "Depending on the complexity of the world, it can take some time" << endl;
            mercado = tolower(_getch());
            if ((mercado == 'y') || (mercado == 'n')) { mercadoOK = true; }
            system(CLEAR);
        }
    }

    //LEEMOS DATOS DE RUN8ECONOMY.TXT SI EXISTE (si no, se creará uno). leemos en lso dos idiomas para no borrar en caso de cambiar de idioma.

        ifstream input("Run8Economy.txt");
        string line;
        while (getline(input, line)) {
            if ((line.find("Total Hours in your economy career: ") != string::npos) || ((line.find("Total Horas en Carrera: ") != string::npos))) {
                initialTotalHours = stod(line.substr(line.find(":") + 1));
            }
            else if ((line.find("Total Miles in your economy career: ") != string::npos) || ((line.find("Total Millas en Carrera: ") != string::npos))) {
                initialTotalEstimatedMiles = stod(line.substr(line.find(":") + 1));
            }
            else if ((line.find("Total burned fuel (gal.) in your career: ") != string::npos) || ((line.find("Total gasto Fuel en Carrera (GAL): ") != string::npos))) {
                initialTotalFuelUsed = stod(line.substr(line.find(":") + 1));
            }
            else if ((line.find("Bank: ") != string::npos) || ((line.find("Banco: ") != string::npos))) {
                initialBanco = stod(line.substr(line.find(":") + 1));
            }
        }
        input.close();
    

    //Leer por primera vez el VernometerData.xml para ver valores iniciales de trabajo y así restarle a la variación y ver el incremento
    double Hora = valorInicialHora(); //valorInicial.h
    double Milla = valorInicialMillas(); //valor de millas en el vernometer, pero leido una vez
    double Gallon = valorInicialGalon();
    double millaMiLoco = Milla;


    //Recorre el archivo UnitStatusDatabase.xml y pone en cada nota de cada locomotora el precio de alquiler y compra
    if ((mercado == 's') || (mercado == 'y')) {
        inicializarMercado(idioma);
    }



    //ventanuco();

    // Leer y actualizar el archivo VernometerData.xml cada 2 segundos    
    while (true) {
        string xmlString;

        //VernometerData.xml
        xmlString = leeXML("VernometerData.xml"); //copiaArchivoTexto.h
        double totalHours = getTotalHours(xmlString); //getTotalValues.h
        double totalMiles = getTotalMiles(xmlString); //getTotalValues.h
        double totalGallons = getTotalGallons(xmlString); //getTotalValues.h
        writeEconomyRun(idioma, totalHours, totalMiles, totalGallons, xmlString, Hora, Milla, Gallon, initialTotalHours, initialTotalEstimatedMiles, initialTotalFuelUsed, initialBanco, locosTrabajo); //writeEconomiaRun8TXT.h


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        
        //Auto Save Train.xml
        xmlString = leeXML("./V3Routes/Regions/SouthernCA/AutoSaves/Auto Save Train.xml");



        //   //Vagones y Peso
        NumCochesPeso* miVariable = getNumCars(xmlString);
        int numVagones = miVariable->iNumCars;
        int numVagonesValido = miVariable->iNumCarsValido;
        double totalWeight = miVariable->itotalWeight;
        double totalWeightValido = miVariable->itotalWeightValido;
        //el archivo xml del tren se queda guardado. Comparo la hora de la última modificación y si sobrepasa 3 minutos lo anulo el numvagones y peso
        // asi el programa al ser ejecutado no contabiliza trenes pasados salvados. En cuanto tome un tren nuevo sí contará.
        int hora_actual = horaActual();
        int hora_archivo = horaUltimaModificacion(); //copiaArchivoTexto.h
        //cout << "la hora actual es: " << hora_actual << endl;
        //cout << "la hora del archivo es: " << hora_archivo << endl;
        if ((hora_actual - hora_archivo) >= 3) { //saco la hora del archivo ultima vez modificado y la comparo con la hora actual y si es mayor de 3min entonces no cuento vagones.
            numVagonesValido = 0;
            numVagones = 0;
            totalWeight = 0;
            totalWeightValido = 0;
        }
        system(CLEAR); //Limpia la consola definido en el #define CLEAR "cls"
        if (idioma == 's') {
            cout << "el numero de vagones del tren es: " << numVagones << endl;
            cout << "el peso de la mercancia del tren es: " << fixed << setprecision(2) << totalWeight << endl;
        }
        if (idioma == 'e') {
            cout << "Number of wagons of this train: " << numVagones << endl;
            cout << "Carried stuff in this train: " << fixed << setprecision(2) << totalWeight << endl;
        }
        delete miVariable; //liberarmos memoria



   //   //Locomotoras e identificación
        Locomotoras miLocomotora = numLocomotoras(xmlString);
        if ((hora_actual - hora_archivo) >= 3) { //saco la hora del archivo ultima vez modificado y la comparo con la hora actual y si es mayor de 3min entonces no cuento locomotoras.
            for (int i = 0; i < miLocomotora.numLocos; i++) {
                miLocomotora.numsLocos[i] = 0;
            }
            miLocomotora.trenID = 0;
            miLocomotora.numLocos = 0;
        }

        LocomotoraTrabajo resultadoLocomotoras;



        /////////////////////////////////////////////////////////////////
        //INICIALIZACION DISTANCIA POR CAMBIO LOCOMOTORA - CABRON!!!!! HIJO DE PUTA!!!
        //todavia pasa en cambio antes el valor del vector anterior de locosTrabajo a la locomotora cambiada, pero en TrabajoLocomotoras.h he
        //quitado la condicional de que la cantidad sea mayor, para devolver el valor menor del vector distRunLocoVector.
        
        distRunLocoActiva = 0.0; //creo que hay que inicializarlo a cero o si no le pasará el valor anterior a la locomotora cambiada.
        distRunLocoVectorEncontrado = false;
        if (resultadoLocomotoras.idLocomotoraTrabajo != locoIDUsuario[0]) { //CON ESTO DETECTO EL MOMENTO DEL RELINQUISH A OTRA LOCOMOTORA.
            locoIDUsuario[1] = locoIDUsuario[0];
            locoIDUsuario[0] = resultadoLocomotoras.idLocomotoraTrabajo;
            millaMiLoco = totalMiles; //Actualizo el cero de la milla para calcular el num de millas hechas.
            distRunLoco = 0.0; 
            for (size_t i = 0; i < distRunLocoVector.size(); i++) {
                if (distRunLocoVector[i].first == resultadoLocomotoras.idLocomotoraTrabajo) {
                    distRunLoco = distRunLocoVector[i].second; //busco si el Loco ID está en el vector y le asigno su valor anterior...
                    break;
                }
            }
            vagonAux = 0; //inicializamos en relinquish para que cuente a partir de cero el vector idsVagones.
        }
        
        else {
            //busco a ver si el cambio de locomotora ya estaba en el vector
            for (size_t i = 0; i < distRunLocoVector.size(); i++) {
                if (distRunLocoVector[i].first == resultadoLocomotoras.idLocomotoraTrabajo) {
                    distRunLocoVectorEncontrado = true;
                    distRunLocoVector[i].second = (totalMiles - millaMiLoco) + distRunLoco; //... para sumarselo a la inicializacion de las nuevas millas recorridas
                    break;
                }
               
            }
            //si no estaba entonces lo mete como nuevo valor y hace más grande el vector.
            if (!distRunLocoVectorEncontrado) {
                distRunLocoVector.push_back(make_pair(resultadoLocomotoras.idLocomotoraTrabajo, resultadoLocomotoras.millasLocomotoraTrabajo)); // = (totalMiles - millaMiLoco) + distRunLoco;
            }
        }
       //y aqui es donde pillo el valor de la distancia recorrida que será pasada al vector LocosTrabajo, buscándo el ID en el vector distRunLocoVector. Si no está no lo asigna (pasara en segunda iteración del programa
        for (size_t i = 0; i < distRunLocoVector.size(); i++) {
            if (distRunLocoVector[i].first == resultadoLocomotoras.idLocomotoraTrabajo) {
                distRunLocoActiva = distRunLocoVector[i].second;
            }
        }

        ////////////////////////////////////
        // FIN INICIALIZACION DISTANCIA POR CAMBIO LOCOMOTORA - CABRON!!!!



        ////////////////////////////////////
        // DETERMINAR TIPOS DE TRABAJO Y REALIZAR ASIGNACIONES AL VECTOR LOCOSTRABAJO.

        //Detectar Relinquish
        if ((trenIDUsuario == miLocomotora.trenID) && (primeroEsLocomotora(xmlString))) { //si no pongo primero es loco enconces sumara al vector trenes con vagones solo
            tiempoCrono = 126000; //2.1 minutos. el train.xml se graba cada 2 minutos. Necesito un poco más para comprobar que sea igual trenIDusuario al trenID
            

            //DETERMINAR SI EL TRABAJO ES TRIM O NO.
            if (locosTrabajo.size() > 1) { // por que si no al iniciar y no pillar tren alguno ese numVagones = 0 y por lo tanto en cuantro entre en Departure aunque tenga vagones será un trim.
                if (locoEnDeparture(xmlString)) { condicion1Trim = true; }
                if (numVagones == 0) { condicion2Trim = true; }
                if ((condicion1Trim && condicion2Trim) || trimTag(xmlString)) { //si tiene la palabra "trim" en el tag ya vale como trimado.
                    trabajoEsTrim = true;
                    if (!llaveTrim) {
                        momentoTrabajoTrim = totalHours;
                        llaveTrim = true;
                    }
                }
            }

            //cout << "momentoTrabajoTrim: " << momentoTrabajoTrim << endl;
            //cout << "horaActual: " << totalHours << endl;
            if (((totalHours > (momentoTrabajoTrim + 0.5)) && (!locoEnReversa(xmlString)) && (!trimTag(xmlString))) || (localTag(xmlString)) || (roadTag(xmlString))) {
                condicion1Trim = false;
                condicion2Trim = false;
                trabajoEsTrim = false;
                llaveTrim = false;
                vagonAux = 0;
                cargaAux = 0;
            }

            //DETERMINAR SI EL TRABAJO ES ROAD
            if ((roadTag(xmlString)) && (locosTrabajo.size() != 1)) {
                typeWork = "R";
                // resultadoLocomotoras = funcionLocomotoraTrabajo(miLocomotora.numsLocos[0], typeWork, miLocomotora.trenID, numVagonesValido, distRunLocoActiva, totalWeightValido, locosTrabajo);
            }


            //DETERMINAR SI EL TRABAJO ES LOCAL
            if (locosTrabajo.size() > 1) {
                if (localTag(xmlString)) {
                    trabajoEsLocal = true;
                }
                else {
                    trabajoEsLocal = false;
                }
            }


            // NUEVA FORMA DE HACER TRABAJO DE TRIM BASADA EN LOCAL Y VECTOR DE ALMACENAMIENTO DE VAGONES
            switch (trabajoEsTrim) {
            case true:
                if (!locoEnDeparture(xmlString)) { 
                    typeWork = "T";
                    cargaAux = almacenIDVagones(xmlString, idsVagones, miLocomotora.numsLocos[0]); 
                    for (auto& vagon : idsVagones) {
                        if (vagon.idlocomotive == miLocomotora.numsLocos[0]) {
                                vagonAux++;
                        }
                    }
                    resultadoLocomotoras = funcionLocomotoraTrabajo(idioma, miLocomotora.numsLocos[0], typeWork, miLocomotora.trenID, vagonAux, distRunLocoActiva, cargaAux, locosTrabajo); 
                    vagonAux = 0;
                }
                else {
                    resultadoLocomotoras = funcionLocomotoraTrabajo(idioma, miLocomotora.numsLocos[0], typeWork, miLocomotora.trenID, numVagonesValido, distRunLocoActiva, cargaAux, locosTrabajo);
                }
                break;

            case false:
                //AQUI también debería estar los ROAD.
                if (!trabajoEsLocal) {
                    cargaAux = almacenIDVagones(xmlString, idsVagones, miLocomotora.numsLocos[0]);
                    for (auto& vagon : idsVagones) {
                        if (vagon.idlocomotive == miLocomotora.numsLocos[0]) {
                            vagonAux++;
                        }
                    }
                    resultadoLocomotoras = funcionLocomotoraTrabajo(idioma, miLocomotora.numsLocos[0], typeWork, miLocomotora.trenID, vagonAux, distRunLocoActiva, cargaAux, locosTrabajo);
                    vagonAux = 0;
                    
                    
                    
                    //resultadoLocomotoras = funcionLocomotoraTrabajo(idioma, miLocomotora.numsLocos[0], typeWork, miLocomotora.trenID, numVagonesValido, distRunLocoActiva, totalWeightValido, locosTrabajo);
                }
                break;
            }


           /*TRABAJOS LOCALES.Son muy dinamicos asi que hay que tener un vector dinamico que mira que vagones han sido pasados y solo escribe los nuevos
            ya que por el camino nos podemos encontrar nuevos a recoger o hacemos composiciones de colocación de vagones. De esta manera, sumando
            los pesos en la propia función que almacena la ID de los vagones en el vector idsVagones, podemos ver el peso total y el numero de vagones
            trabajados independientemente de las veces que rompamos el tren.
            */
           
            switch (trabajoEsLocal) {
            case true:
                typeWork = "L";
                cargaAux = almacenIDVagones(xmlString, idsVagones, miLocomotora.numsLocos[0]); //resultadoLocomotoras.idLocomotoraTrabajo
                for (auto& vagon : idsVagones) {
                    if (vagon.idlocomotive == miLocomotora.numsLocos[0]) { //resultadoLocomotoras.idLocomotoraTrabaj
                        vagonAux++;
                    }
                }
                resultadoLocomotoras = funcionLocomotoraTrabajo(idioma, miLocomotora.numsLocos[0], typeWork, miLocomotora.trenID, vagonAux, distRunLocoActiva, cargaAux, locosTrabajo);
                vagonAux = 0;
                break;
            case false:
                break;
            }

        }
        else {                      //detecto un relinquish. Da igual que sea 5seg. No puede ser 0 ya que  jamás se harían iguales trenIDUsuario y trenID
            tiempoCrono = 5000;
        }


        if (!cronometro(tiempoCrono, idioma)) { //
            trenIDUsuario = miLocomotora.trenID;

        }

        delete[] miLocomotora.numsLocos; //liberarmos memoria del new creado en la función.

        if (idioma == 's') {
            if (locosTrabajo.size() != 1) {
                cout << "--------------------------------------------------" << endl;
                cout << "La Locomotora ID " << resultadoLocomotoras.idLocomotoraTrabajo << " tiene estos resultados:" << endl;
                cout << "el tipo de trabajo es: ";
                if (resultadoLocomotoras.tipoTrabajo == "N") {};
                if (resultadoLocomotoras.tipoTrabajo == "R") { cout << "ES TREN DE RUTA"; }
                if (resultadoLocomotoras.tipoTrabajo == "L") { cout << "ES TREN LOCAL"; }
                if (resultadoLocomotoras.tipoTrabajo == "T") { cout << "ES UN TREN DE COMPOSICION (TRIM)"; }
                cout << endl;
                cout << "numero de vagones Trabajados: " << resultadoLocomotoras.vagonesLocomotoraTrabajo << endl;
                cout << "distancia recorrida Trabajada: " << fixed << setprecision(2) << resultadoLocomotoras.millasLocomotoraTrabajo << endl;
                cout << "carga transportada: " << fixed << setprecision(2) << resultadoLocomotoras.toneladasLocomotoraTrabajo << endl;
            }
            cout << endl;
        }

        if (idioma == 'e') {
            if (locosTrabajo.size() != 1) {
                cout << "--------------------------------------------------" << endl;
                cout << "The Locomotive ID " << resultadoLocomotoras.idLocomotoraTrabajo << " has this results:" << endl;
                cout << "Type of work is: ";
                if (resultadoLocomotoras.tipoTrabajo == "N") {};
                if (resultadoLocomotoras.tipoTrabajo == "R") { cout << "IT IS A ROAD TRAIN"; }
                if (resultadoLocomotoras.tipoTrabajo == "L") { cout << "IT IS A LOCAL TRAIN"; }
                if (resultadoLocomotoras.tipoTrabajo == "T") { cout << "IT IS A TRIM WORK"; }
                cout << endl;
                cout << "Number of worked wagons: " << resultadoLocomotoras.vagonesLocomotoraTrabajo << endl;
                cout << "Worked distance: " << fixed << setprecision(2) << resultadoLocomotoras.millasLocomotoraTrabajo << endl;
                cout << "Load transported: " << fixed << setprecision(2) << resultadoLocomotoras.toneladasLocomotoraTrabajo << endl;
            }
            cout << endl;
        }

       
        //CUIDADO CON LOCOMOTORAS DOBLES UNIDAS. DUPLICARÁ EL TRABAJO. BUSCAR resultadoLocomotoras.idTrenTrabajo y si es el mismo que uno ya, no cogerlo.

        //almacenar la distancia de la locomotora1.
        Sleep(2000); // esperar 2 segundos antes de leer y actualizar el archivo de nuevo
    } 
   return 0;
}




/*Para reconocer qué tipo de trabajo está haciendo el usuario se me ha ocurrido esto:
* La etiqueta anterior al "unitType" que nos indica si es una locomotora ("US_DieselEngine") hay otra que nos dice qué tipo de locomotora es.
* es la <rvXMLfilename> que hace mención a un xml (que no existe) en donde tiene al principio la marca de la locomotora y luego la compañia. 
* Por ejemplo este: R8_C449W_BNSF01.xml. El tema es coger hasta el segundo "_" (cuidado por que hay algunos que no tienen un segundo _ sino un .xml,
por lo tanto buscar segundo _ o un . y ver qué tipo de locomotora es:
* 
* R8_C449W = Ruta o Local
* R8_GP40-2 = Trim o Local
* R8_SD40-2 = Trim o Local
* R8_SD45-2 = Trim o Local
* Run8_ES44DC = Ruta o Local
* Run8_SD70ACE = Ruta o Local
* 
* Evidentemente esto es cuando se reconocido un número de vagones en la parte de atrás enganchados. Como diferenciar entre las dos opciones de cada uno?
* Entre Trim y Local es fácil. Ver la cantidad de millas recorridas, y si sobrepasa cierto número no muy alto, visto en el VernometerData.xml a partir del 
* momento en que se pilló ese tren, entonces está haciendo trim. Si es más distancia estará haciendo local (esto puede fallar, por que hay locales muy cerca
* de los yards)
* En cuanto a Ruta o Local, ya es más complicado, pero hay una manera. Los trenes de ruta SUELEN tener locomotoras <unitType>US_DieselEngine en la parte de atras
* por lo tanto si lo tiene, es ruta fijo. Si no lo tiene, es de ruta. 
* Puede haber trenes de ruta sin locos atras.. pero ya tendrían varias locomotoras adelante haciendo DPU. A parte que se puede hacer distinción por la longitud
* del tren también. Los locales no son muy largos (pero he visto alguno con locomotora détrás. Creo que debe ser eso O que sea un tren no muy largo. Por 
* ejemplo veo un tren de 3 locos adelante y una atras, pero son 1700pies de largo)
* 
* Tambien, se puede reconocer un trabajo e Trim, si se mira la etiqueta <currentTrackSectionIndex> <int>num y ese num permanece inalterable.
* Normalmente los trenes tienen en el <destinationTag> el añadido Trim, hump. Se puede buscar, pero no es muy fiable.
* 
* 
* 2-Creo que se puede saber cuando un tren es de ruta o no. El tema es saber si la etiqueta <CurrentRoutePrefix><int>num <currentTrackSectionIndex><int>num 
* <startNodeIndex><int>num marca donde están los departure yards en cada yard. 
* Si eso es así, se puede hacer que no se contabilizen los vagones que estén en esas zonas. Así por ejemplo si vamos con un loco trim con 10 vagones y entramos
* en esas lineas, entonces los vagones no serán contabilizados y serán 0, ya que al unirse a otros que haya, sumarán lo que están aparcados y nos darán más
* dinero, siendo falso. De esa manera solo quedarán los 10 que metimos. Y si cogemos un tren de ruta, ese tren tendrá cero vagones hasta que la locomotora esté
* fuera de esa linea. Una cosa a ver, es si se suman los vagones al conectarse con la linea y el loco trim está fuera empujando hacia atrás. El tema sería 
* comparar todos los vagones y si hay alguno que esté dentro de la totalidad del tren, dejar de buscar y anular la contabilidad.
* 
* BARSTOW: <currentRoutePrefix>150
* Departure: Sur6 = <currentTrackSectionIndex>(1435)-1440-1437-1438-491 entre paréntesis puede ser zona de agujas. quitar.
*           Sur5 = 5635-5604-5599-5594
*           Sur4 = 5632-5603-5598-5593-5586
*           Sur3 = 5631-5602-5597-5592-5587
*           Sur2 = 5630-5601-5596-5591-5588
*           Sur1 = 5621-5600-5595-5590-5589
*           Nor6 = (1477)-1479-1478-1480-629-534
*           Nor5 = 5605-5460-5455-5450-630
*           Nor4 = 5611-5606-5459-5454-5449-5444-(5437)
*           Nor3 = 5614-5458-5453-5448-5443-5438-
*           Nor2 = 5613-5457-5452-5447-5442-5439-
*           Nor1 = 5620-5456-5451-5446-5441-5440-
*/


////////////////////////// CAMBIO DE LOCOMOTORA O TREN

/*
ya he hecho el reconocer el número de vagones y guardarlo en un vector para recordarlo si volvemos a la misma locomotora. El problema es que siempre se 
almacenará la cantida mayor de vagones, de tal forma que si llevamos 10 y vamos a unirlos a 5, serán 15. Si desenganchamos la locomotora y buscamos 4 más, como
4 es menor al número almacenado 15, esos nuevos 4 vagones se perderán, y todavía tendremos 15. 
1. Si se unen a los otros 15 no hay problema por que serían 15+4=19 y estaría bien, pero ...
2. si se hace en un yard de departure, estos anularan todos los vagones situados allí (Para no sumar vagones que estuvieran allí antes del trabajo. por lo que
si llevamos 15 a 90 ya en un departure, en cuantro entren los 15 en el departure se leerá 0 vagones, pero el valor del vector todavía será 15. Bien.
Pero si cogemos 4 más en el bowl, y llevamos al departure, al ser 4 menor que 15 se perderán, por que siempre en el departure son 0. Como corregir esto?

A) Podemos ver si la locomotora ha soltado todos, o sea que el algún momento tiene cero vagones contabiizados por el contador instantaneo (train.xml actualizado) 
entonces cualquier vagón cogido después podría sumarse a esos 15 ya hechos, si sacamos la secuencia fuera del if, del vector y así pasamos 4+15, siendo 19, de
tal forma que al pasar dentro del departure serán 19 transformados a cero, pero ya serán 19 en el vector... (me parece buena solución)
*/