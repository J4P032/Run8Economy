#ifndef DINEROS_H
#define DINEROS_H
#include <vector>
#include <map>
#include "TrabajoLocomotoras.h"
#include "getTotalValues.h"
#include "copiaArchivoTexto.h"
#include "mercado.h"
#include "writeEconomiaRun8TXT.h"




const double factorPagoTrim = 10;  //debe haber un factor de conversion por que dejarlo como suma de vagones haria que uno que entra y pilla un vagon solo ya sumaria mucha cantidad sin hacer nada.
const double factorPagoLocal = 2.5;



struct dinerosPorCurro {
    double dolaresRoad;
    double dolaresTrim;
    double dolaresHump;
    double dolaresLocal;
};


struct gastoAlquiler {
    int idLocoAlquiler;
    double precioRenting;
};



double buscaPrecioAlquiler(int idLoco) {
    int idLocoXML = 0;
    double valorAlquiler = 0;
    int startTagPos = 0;
    int endTagPos = 0;
    string notesXML = leeXML("./V3RailVehicles/UnitNotesDatabase.xml");
    while ((startTagPos = notesXML.find("<roadNameNumber>", endTagPos)) != string::npos) {
        startTagPos += sizeof("<roadNameNumber>") - 1;
        endTagPos = notesXML.find("</roadNameNumber>", startTagPos);
        string cadena = notesXML.substr(startTagPos, endTagPos - startTagPos);
        idLocoXML = convertirANumero(cadena);
        if (idLocoXML == idLoco) {
            startTagPos = notesXML.find("Hour: $", endTagPos);
            startTagPos += sizeof("Hour: $") - 1;
            endTagPos = notesXML.find("</noteText>", startTagPos);
            string cadena = notesXML.substr(startTagPos, endTagPos - startTagPos);
            stringstream valorAlquilerXML(notesXML.substr(startTagPos, endTagPos - startTagPos)); 
            valorAlquilerXML >> valorAlquiler;
            return valorAlquiler;
        }
        else {
            continue;
        }
    }
    return valorAlquiler;
}




void meterLocomotorasAlquiladas(vector<gastoAlquiler>& LocomotorasAlquiladas) {
    int startTagPos = 0;
    int endTagPos = 0;
    int indiceID = 0;
    bool encontrado = false;
    string xmlString3 = leeXML("./V3Routes/Regions/SouthernCA/AutoSaves/Auto Save Train.xml");
    while ((startTagPos = xmlString3.find("<unitType>", endTagPos)) != string::npos) {
        startTagPos += sizeof("<unitType>") - 1;
        endTagPos = xmlString3.find("</unitType>", startTagPos);
        string cadena = xmlString3.substr(startTagPos, endTagPos - startTagPos);
        if (cadena == "US_DieselEngine") {
            startTagPos = xmlString3.find("<unitNumber>", endTagPos);
            startTagPos += sizeof("<unitNumber>") - 1;
            endTagPos = xmlString3.find("</unitNumber>", startTagPos);
            stringstream xmlValorID(xmlString3.substr(startTagPos, endTagPos - startTagPos));
            xmlValorID >> indiceID;
            for (auto& loco : LocomotorasAlquiladas) { //nos recorremos el vector
                if (loco.idLocoAlquiler == indiceID) {
                    encontrado = true;
                    break;
                }
                if (!encontrado) {
                    gastoAlquiler loco1 = { indiceID, buscaPrecioAlquiler(indiceID) };
                    LocomotorasAlquiladas.emplace_back(loco1);
                }
            }
        }
        else { continue; }
    }
   /* gastoAlquiler valorAlquiler;
    valorAlquiler.idLocoAlquiler = indiceID;
    valorAlquiler.precioRenting = buscaPrecioAlquiler(indiceID);
    return valorAlquiler;*/
}


dinerosPorCurro cuantoGanoHoy(int idLOCOMOTORA,  std::vector<LocomotoraTrabajo>& locosTrabajo) {
    
    //VER SI INICIALIZANDO A CERO SE SALVA LO DE VOLVERSE A CERO LOS INGRESOS DENTRO DE LOCOS DEL MISMO TREN.
    std::vector<int> idNums(0, 0); //para meter los numeros ID encontrados y no volverlos a sumar en la busqueda.
    idNums.clear();
    double dolaresRoad = 0.0;
    double dolaresTrim = 0.0;
    double dolaresHump = 0.0;
    double dolaresLocal = 0.0;
    double dolares = 0.0;
    double millasAux = 0.0;
    int posicionIndexLocosTrabajo = 0;


    for (size_t i = 0; i < locosTrabajo.size(); i++) {
        if (locosTrabajo[i].idLocomotoraTrabajo == idLOCOMOTORA) {
            posicionIndexLocosTrabajo = i;
            break;
        }
    }

    if (locosTrabajo[posicionIndexLocosTrabajo].tipoTrabajo == "R") {
        //0.04$ Ton/milla tranportada. 
        dolaresRoad = locosTrabajo[posicionIndexLocosTrabajo].toneladasLocomotoraTrabajo * 0.04 * (locosTrabajo[posicionIndexLocosTrabajo].millasLocomotoraTrabajo + millasAux);
    }
    if (locosTrabajo[posicionIndexLocosTrabajo].tipoTrabajo == "L") {
        //hay un factor de conversion por trabajo y milla hecha con num de vagones movidos. El 5 es un factor de conseguir misma cantidad que 100$ por vagon. A eso sumar mismo Ton/milla
        dolaresLocal = (locosTrabajo[posicionIndexLocosTrabajo].toneladasLocomotoraTrabajo * 0.04 * (locosTrabajo[posicionIndexLocosTrabajo].millasLocomotoraTrabajo + millasAux)) + ((locosTrabajo[posicionIndexLocosTrabajo].millasLocomotoraTrabajo + millasAux) * locosTrabajo[posicionIndexLocosTrabajo].vagonesLocomotoraTrabajo * factorPagoLocal);
    }
    if (locosTrabajo[posicionIndexLocosTrabajo].tipoTrabajo == "T") {
        //aquí no se contabilizan las millas hechas ya que son irrisorias. Pero sí el número de vagones compuestos. Pero tengo que poner millas por que si no habría trampa de jugador solo cogiendo el tren.
        dolaresTrim = (locosTrabajo[posicionIndexLocosTrabajo].millasLocomotoraTrabajo + millasAux) * locosTrabajo[posicionIndexLocosTrabajo].vagonesLocomotoraTrabajo * factorPagoTrim;
    }

    
    //Horas a 100$ la hora mas dinero por vagones recibidos. El dinero por Horas está ya incluido en el writeEconomiaRun8TXT.h
	dolares = dolaresRoad + dolaresLocal + dolaresTrim + dolaresHump;

    dinerosPorCurro resultadoDinero;
    resultadoDinero.dolaresRoad = dolaresRoad;
    resultadoDinero.dolaresLocal = dolaresLocal;
    resultadoDinero.dolaresTrim = dolaresTrim;
    resultadoDinero.dolaresHump = dolaresHump;
    
    return resultadoDinero;
}


#endif
