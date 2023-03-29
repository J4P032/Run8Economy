#ifndef GETTOTALVALUES_H
#define GETTOTALVALUES_H

#include <iostream>
#include <sstream>
#include "LugarVehiculo.h"
using namespace std;

//Obtiene el numero de horas jugadas en Run8
double getTotalHours(string xmlString) {
    // Buscar la etiqueta TotalHoursInSim y extraer su valor
    string startTag = "<TotalHoursInSim>";
    string endTag = "</TotalHoursInSim>";
    int startTagPos = xmlString.find(startTag);
    int endTagPos = xmlString.find(endTag);
    if (startTagPos == -1 || endTagPos == -1) {
        throw runtime_error("Tag root or TotalHoursInSim not found");
    }
    startTagPos += startTag.size();
    double totalHours;
    stringstream xmlValue(xmlString.substr(startTagPos, endTagPos - startTagPos));
    xmlValue >> totalHours;
    return totalHours;
}






//Obtiene el numero de millas hechas en Run8 como maquinista
double getTotalMiles(string xmlString) {
    //Buscar la etiqueta TotalEstimatedMilesAsEngineer y extraer su valor
    string startTag = "<TotalEstimatedMilesAsEngineer>";
    string endTag = "</TotalEstimatedMilesAsEngineer>";
    int startTagPos = xmlString.find(startTag);
    int endTagPos = xmlString.find(endTag);
    if (startTagPos == -1 || endTagPos == -1) {
        throw runtime_error("Tag root or TotalKilotonMilesAsEngineer not found");
    }
    startTagPos += startTag.size();
    double totalMiles;
    stringstream xmlValue(xmlString.substr(startTagPos, endTagPos - startTagPos));
    xmlValue >> totalMiles;
    return totalMiles;
}







//Obtiene el numero de galones usados como maquinista en Run8
double getTotalGallons(string xmlString) {
    //Buscar la etiqueta TotalFuelUsedGallons y extraer su valor
    string startTag = "<TotalFuelUsedGallons>";
    string endTag = "</TotalFuelUsedGallons>";
    int startTagPos = xmlString.find(startTag);
    int endTagPos = xmlString.find(endTag);
    if (startTagPos == -1 || endTagPos == -1) {
        throw runtime_error("Tag root or TotalFuelUsedGallons not found");
    }
    startTagPos += startTag.size();
    double totalGallons;
    stringstream xmlValue(xmlString.substr(startTagPos, endTagPos - startTagPos));
    xmlValue >> totalGallons;
    return totalGallons;
}




//Obtiene el peso DE CARGA y numero de vagones si tienen una locomotora en cabeza del tren.
struct NumCochesPeso {
    int iNumCars;
    int iNumCarsValido;
    double itotalWeight;
    double itotalWeightValido;
};

 NumCochesPeso* getNumCars(string xmlString) {
    //Buscar la etiqueta unitType y ver si es un US_Freightcar (que lo pueden ser los vagones de pasajeros, pero mirar si la etiqueta es un amtrak)

    int NumCars = 0;        //el que contabiliza del xml
    int NumCarsValido = 0; //el que descuenta de lineas departure, etc...
    double totalWeight = 0;
    double totalWeightValido = 0;
    double carWeight = 0;
    int startTagPos = 0;
    int endTagPos = 0;
    bool primeroLoco = false;
    int indiceRuta = 0;
    int indiceVia = 0;

    while ((startTagPos = xmlString.find("<unitType>", endTagPos)) != string::npos) { //(lo que se quiere encontrar, a partir de qué posición)
        startTagPos += sizeof("<unitType>") - 1; //sumamos el tamaño de la etiqueta al startTagPos para buscar la endtagpos.
        endTagPos = xmlString.find("</unitType>", startTagPos);
        string texto = xmlString.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
        if (texto == "US_DieselEngine") {
            if (NumCars == 0) {
                primeroLoco = true; //tiene que haber una llave por que puede contar vagones que no tengan locomotora adelante pero sí en medio.
            }
          }
        if (texto == "US_Freightcar") { //tiene que contabilizar si o si los vagones, por que si no contaría vagones con locomotoras en medio
            //busco valor indice de la Ruta
           
            startTagPos = xmlString.find("<int>", endTagPos);
            startTagPos += sizeof("<int>") - 1;
            endTagPos = xmlString.find("</int>", startTagPos);
            //string stringRuta = xmlString.substr(startTagPos, endTagPos - startTagPos);
            stringstream xmlValueRuta(xmlString.substr(startTagPos, endTagPos - startTagPos)); //stringstream saca el valor en string a valor numérico (si es un número)
            xmlValueRuta >> indiceRuta;
            //busco Valor indice de la via
            
            startTagPos = xmlString.find("<currentTrackSectionIndex>", endTagPos);
            startTagPos += sizeof("<currentTrackSectionIndex>") - 1;
            endTagPos = xmlString.find("<int>", startTagPos);
            startTagPos = endTagPos;
            startTagPos += sizeof("<int>") - 1;
            endTagPos = xmlString.find("</int>", startTagPos);
            stringstream xmlValueVia(xmlString.substr(startTagPos, endTagPos - startTagPos));
            xmlValueVia >> indiceVia;

             
            if (!lineaDeparture(indiceRuta, indiceVia)) {
           
            NumCarsValido += 1;
            }
            NumCars += 1;

            

            // Sumar pesos de todos vagones. Se empieza a partir de la posición después de encontrar el unitType que fuera un US_Freightcar.
            startTagPos = xmlString.find("<loadWeightUSTons>", endTagPos);
            startTagPos += sizeof("<loadWeightUSTons>") - 1;
            endTagPos = xmlString.find("</loadWeightUSTons>", startTagPos);
            string stringPeso = xmlString.substr(startTagPos, endTagPos - startTagPos);
            stringstream xmlValue(xmlString.substr(startTagPos, endTagPos - startTagPos)); //stringstream saca el valor en string a valor numérico (si es un número)
            xmlValue >> carWeight;
            if (!lineaDeparture(indiceRuta, indiceVia)) {

                totalWeightValido = totalWeightValido + carWeight;
            }

            totalWeight = totalWeight + carWeight;
        }
    }
    if (!primeroLoco) { 
        NumCars = 0; //como le he dejado contar vagones y peso, si no tiene una locomotora al inicio, lo vuelve todo cero.
        totalWeight = 0;
        totalWeightValido = 0;
        NumCarsValido = 0;
    }

    struct NumCochesPeso resultado {
        NumCars,
        NumCarsValido,
        totalWeight,
        totalWeightValido
    };
    return new NumCochesPeso{ resultado };

}
 
 //Obtiene el numero de locomotoras que compone el tren y cada uno de sus números de referencia. Sirve para saber si se está en el mismo tren a lo largo del tiempo.
 struct Locomotoras {
     int trenID;
     int numLocos;
     int* numsLocos;
};
 


 Locomotoras numLocomotoras(string xmlString) {
   
     int numLocos = 0;
     int numIdentLoco = 0;
     int startTagPos = 0;
     int endTagPos = 0;
     int trenID = 0;
     

     // Buscar la etiqueta TrainID y extraer su valor
     string startTag = "<trainID>";
     string endTag = "</trainID>";
     startTagPos = xmlString.find(startTag);
     endTagPos = xmlString.find(endTag);
     if (startTagPos == -1 || endTagPos == -1) {
         throw runtime_error("Tag root or TotalHoursInSim not found");
     }
     startTagPos += startTag.size();
     stringstream xmlValue(xmlString.substr(startTagPos, endTagPos - startTagPos));
     xmlValue >> trenID;
      
     startTagPos = 0; //reiniciamos para volver a buscar desde el principio.
     endTagPos = 0;

     // Saca el número de locomotoras
     while ((startTagPos = xmlString.find("<unitType>", endTagPos)) != string::npos) { //(lo que se quiere encontrar, a partir de qué posición)
         startTagPos += sizeof("<unitType>") - 1; //sumamos el tamaño de la etiqueta al startTagPos para buscar la endtagpos.
         endTagPos = xmlString.find("</unitType>", startTagPos);
         string texto = xmlString.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
         if (texto == "US_DieselEngine") {
             numLocos += 1;
         }
     }
  
  
     Locomotoras LocosResultado;
     LocosResultado.trenID = trenID;
     LocosResultado.numLocos = numLocos;
     LocosResultado.numsLocos = new int[numLocos];

    
     //Saca el Número id de cada locomotora (los IDs de locomotoras en medio o finales no corresponden con los impresos en el morro de la locomotora.
     startTagPos = 0;
     endTagPos = 0;
     int i = 0;
     while ((startTagPos = xmlString.find("<unitNumber>", endTagPos)) != string::npos) { //(lo que se quiere encontrar, a partir de qué posición)
         startTagPos += sizeof("<unitNumber>") - 1; //sumamos el tamaño de la etiqueta al startTagPos para buscar la endtagpos.
         endTagPos = xmlString.find("</unitNumber>", startTagPos);
         stringstream xmlValue(xmlString.substr(startTagPos, endTagPos - startTagPos));
         xmlValue >> numIdentLoco;
         if (i >= numLocos) { 
             break; //si i es mayor que el numero de locomotoras se produce un acceso no autorizado a posición de memoria.
         }
         LocosResultado.numsLocos[i] = numIdentLoco;
         i++;
     }
         
     return LocosResultado;
 }



#endif

