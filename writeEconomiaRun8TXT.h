#ifndef WRITECONOMIARUN8TXT_H
#define WRITECONOMIARUN8TXT_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip> //para precision decimal de dos cifras (setprecision(2))
#include "workingValue.h"
#include "dineros.h"
#include "TrabajoLocomotoras.h"
using namespace std;





//hacemos un vector para que las ganancias de los diferentes trabajos no se pierdan.
vector<pair<int, double>> dolaresRoad(1, make_pair(0, 0.0));
vector<pair<int, double>> dolaresTrim(1, make_pair(0, 0.0));
vector<pair<int, double>> dolaresHump(1, make_pair(0, 0.0));
vector<pair<int, double>> dolaresLocal(1, make_pair(0, 0.0));
vector<gastoAlquiler> LocomotorasAlquiladas;
int idLocoTrabajo = 0;
bool vectorDolaresTrim = false;
bool vectorDolaresLocal = false;
bool vectorDolaresRoad = false;




void writeEconomyRun(char idioma, double totalHours, double totalMiles, double totalGallons, string xmlString, double Hora, double Milla, double Galon, double initialTotalHours, double initialTotalEstimatedMiles, double initialTotalFuelUsed, double initialBanco, std::vector<LocomotoraTrabajo>& locosTrabajo) {
    // Escribir el archivo EconomiaRun8.txt con las etiquetas requeridas
    string startTag1 = "<TotalHoursInSim>";
    string endTag1 = "</TotalHoursInSim>";
    string startTag2 = "<TotalEstimatedMilesAsEngineer>";
    string endTag2 = "</TotalEstimatedMilesAsEngineer>";
    string startTag3 = "<TotalFuelUsedGallons>";
    string endTag3 = "</TotalFuelUsedGallons>";
    int startTagPos1 = xmlString.find(startTag1);
    int endTagPos1 = xmlString.find(endTag1);
    int startTagPos2 = xmlString.find(startTag2);
    int endTagPos2 = xmlString.find(endTag2);
    int startTagPos3 = xmlString.find(startTag3);
    int endTagPos3 = xmlString.find(endTag3);
    if (startTagPos1 == -1 || endTagPos1 == -1 || startTagPos2 == -1 || endTagPos2 == -1 || startTagPos3 == -1 || endTagPos3 == -1) {
        throw runtime_error("Required Tags not found");
    }
    startTagPos1 += startTag1.size();
    startTagPos2 += startTag2.size();
    startTagPos3 += startTag3.size();
    stringstream xmlValue1(xmlString.substr(startTagPos1, endTagPos1 - startTagPos1));
    stringstream xmlValue2(xmlString.substr(startTagPos2, endTagPos2 - startTagPos2));
    stringstream xmlValue3(xmlString.substr(startTagPos3, endTagPos3 - startTagPos3));
    double totalEstimatedMiles;
    double totalFuelUsed;
    double hHoras;
    double hMillas;
    hHoras = SacarValorTrabajado(totalHours, Hora); //workingValue.h
    hMillas = SacarValorTrabajado(totalMiles, Milla);
    xmlValue2 >> totalEstimatedMiles;
    xmlValue3 >> totalFuelUsed;
    ofstream output("Run8Economy.txt");
    if (idioma == 's') {
        output << "Total Horas en Run8: " << fixed << setprecision(2) << totalHours << endl;
        output << "Total Millas como Maquinista: " << fixed << setprecision(2) << totalEstimatedMiles << endl;
        output << "Total Galones Consumidos: " << fixed << setprecision(2) << totalFuelUsed << endl;
        output << endl;
        output << "Horas Trabajadas Hoy: " << fixed << setprecision(2) << SacarValorTrabajado(totalHours, Hora) << endl;
        output << "Millas Recorridas Hoy: " << fixed << setprecision(2) << SacarValorTrabajado(totalMiles, Milla) << endl;
        output << "Galones Gastados Hoy: " << fixed << setprecision(2) << SacarValorTrabajado(totalGallons, Galon) << endl;
        output << endl;
        output << "Total Horas en Carrera: " << fixed << setprecision(2) << initialTotalHours + SacarValorTrabajado(totalHours, Hora) << endl;
        output << "Total Millas en Carrera: " << fixed << setprecision(2) << initialTotalEstimatedMiles + SacarValorTrabajado(totalMiles, Milla) << endl;
        output << "Total gasto Fuel en Carrera (GAL): " << fixed << setprecision(2) << initialTotalFuelUsed + SacarValorTrabajado(totalGallons, Galon) << endl;
        output << endl;
        output << "INFO DE LOCOMOTORAS USADAS HOY" << endl;
        output << "------------------------------" << endl;
        output << "Total locomotoras trabajadas: " << locosTrabajo.size() - 1 << endl;
    }

    if (idioma == 'e') {
        output << "Total Hours in Run8: " << fixed << setprecision(2) << totalHours << endl;
        output << "Total Miles as Engineer: " << fixed << setprecision(2) << totalEstimatedMiles << endl;
        output << "Total Galons burned: " << fixed << setprecision(2) << totalFuelUsed << endl;
        output << endl;
        output << "Worked Hours today: " << fixed << setprecision(2) << SacarValorTrabajado(totalHours, Hora) << endl;
        output << "Miles runned today: " << fixed << setprecision(2) << SacarValorTrabajado(totalMiles, Milla) << endl;
        output << "Gallons burned today: " << fixed << setprecision(2) << SacarValorTrabajado(totalGallons, Galon) << endl;
        output << endl;
        output << "Total Hours in your economy career: " << fixed << setprecision(2) << initialTotalHours + SacarValorTrabajado(totalHours, Hora) << endl;
        output << "Total Miles in your economy career: " << fixed << setprecision(2) << initialTotalEstimatedMiles + SacarValorTrabajado(totalMiles, Milla) << endl;
        output << "Total burned fuel (gal.) in your career: " << fixed << setprecision(2) << initialTotalFuelUsed + SacarValorTrabajado(totalGallons, Galon) << endl;
        output << endl;
        output << "TODAY's USED LOCOMOTIVES INFO" << endl;
        output << "-----------------------------" << endl;
        output << "Total used locomotives: " << locosTrabajo.size() - 1 << endl;
    }

    int totalWorkVagones = 0;
    double totalWorkTons = 0.0;
    double totalWorkMiles = 0.0;
    int idLOCOMOTORA = 0;
    double gananciasRoad = 0.0;
    double gananciasTrim = 0.0;
    double gananciasLocal = 0.0;

   
    
    //vector dolaresRoad
    gananciasRoad = 0.0;
    for (size_t i = 0; i < locosTrabajo.size(); i++) {
        if (locosTrabajo[i].tipoTrabajo == "R") {
            vectorDolaresRoad = false;
            for (size_t j = 0; j < dolaresRoad.size(); j++) {
                if ((locosTrabajo[i].idLocomotoraTrabajo == dolaresRoad[j].first) && (locosTrabajo[i].idLocomotoraTrabajo != 0)) {
                    vectorDolaresRoad = true;
                    gananciasRoad = cuantoGanoHoy(locosTrabajo[i].idLocomotoraTrabajo, locosTrabajo).dolaresRoad;
                    dolaresRoad[j].second = gananciasRoad;
                    break;
                }
            }
            if ((!vectorDolaresRoad) && (locosTrabajo[i].idLocomotoraTrabajo != 0)) {
                dolaresRoad.push_back(make_pair(locosTrabajo[i].idLocomotoraTrabajo, gananciasRoad));
            }
        }
    }
  

    //HAY UN ERROR AL SUMAR TRABAJOS, QUE SE SUMAN LAS MILLAS DE UNO A OTRO LOCAL A TRIM.
    //vector dolaresTrim
    //buscar si alguno de los IDs de Locos de LocosTrabajo están en el vector dolaresTrim
    gananciasTrim = 0.0; 
    for (size_t i = 0; i < locosTrabajo.size(); i++) {
        if (locosTrabajo[i].tipoTrabajo == "T") {
            vectorDolaresTrim = false;
            for (size_t j = 0; j < dolaresTrim.size(); j++) {
                if ((locosTrabajo[i].idLocomotoraTrabajo == dolaresTrim[j].first) && (locosTrabajo[i].idLocomotoraTrabajo != 0)) {
                    vectorDolaresTrim = true;
                    gananciasTrim = cuantoGanoHoy(locosTrabajo[i].idLocomotoraTrabajo, locosTrabajo).dolaresTrim;
                    //if (ganancias > dolaresTrim[j].second){
                    dolaresTrim[j].second = gananciasTrim;
                    //}
                    break;
                }
            }
            if ((!vectorDolaresTrim) && (locosTrabajo[i].idLocomotoraTrabajo != 0)) {
                dolaresTrim.push_back(make_pair(locosTrabajo[i].idLocomotoraTrabajo, gananciasTrim));
            }
        }
    }


    //vector dolaresLocal
    gananciasLocal = 0.0;
    for (size_t i = 0; i < locosTrabajo.size(); i++) {
        if (locosTrabajo[i].tipoTrabajo == "L") {
            vectorDolaresLocal = false;
            for (size_t j = 0; j < dolaresLocal.size(); j++) {
                if ((locosTrabajo[i].idLocomotoraTrabajo == dolaresLocal[j].first) && (locosTrabajo[i].idLocomotoraTrabajo != 0)) {
                    vectorDolaresLocal = true;
                    gananciasLocal = cuantoGanoHoy(locosTrabajo[i].idLocomotoraTrabajo, locosTrabajo).dolaresLocal;
                    dolaresLocal[j].second = gananciasLocal;
                    break;
                }
            }
            if ((!vectorDolaresLocal) && (locosTrabajo[i].idLocomotoraTrabajo != 0)) {
                dolaresLocal.push_back(make_pair(locosTrabajo[i].idLocomotoraTrabajo, gananciasLocal));
            }
        }
    }
    
    //REPRESENTACION DE CADA LOCOMOTORA CON SU DINERO GANADO.
    for (size_t i = 1; i < locosTrabajo.size(); i++) {
        
        if (idioma == 's') {
            output << "Locomotora ID " << locosTrabajo[i].idLocomotoraTrabajo << " " << locosTrabajo[i].tipoTrabajo << ": " << locosTrabajo[i].vagonesLocomotoraTrabajo << " Vagones Trabajados || " << locosTrabajo[i].millasLocomotoraTrabajo << " millas recorridas || " << locosTrabajo[i].toneladasLocomotoraTrabajo << " TON. carga transportada " << " || $";
        }
        if (idioma == 'e') {
            output << "Locomotive ID " << locosTrabajo[i].idLocomotoraTrabajo << " " << locosTrabajo[i].tipoTrabajo << ": " << locosTrabajo[i].vagonesLocomotoraTrabajo << " Worked wagons || " << locosTrabajo[i].millasLocomotoraTrabajo << " miles runned || " << locosTrabajo[i].toneladasLocomotoraTrabajo << " TONs. transported " << " || $";
        }
        if (locosTrabajo[i].tipoTrabajo == "R") {
            for (size_t j = 0; j < dolaresRoad.size(); j++) { //aquí fallaba por que el tamaño de los vectores dolares no es lo mismo que los locosTrabajo.
                if (dolaresRoad[j].first == locosTrabajo[i].idLocomotoraTrabajo) {
                    output << dolaresRoad[j].second << endl;
                }
            }
        }
        if (locosTrabajo[i].tipoTrabajo == "T") {
            for (size_t j = 0; j < dolaresTrim.size(); j++) {
                if (dolaresTrim[j].first == locosTrabajo[i].idLocomotoraTrabajo) {
                    output << dolaresTrim[j].second << endl;
                }
            }
        }
        if (locosTrabajo[i].tipoTrabajo == "L") {
            for (size_t j = 0; j < dolaresLocal.size(); j++) {
                if (dolaresLocal[j].first == locosTrabajo[i].idLocomotoraTrabajo) {
                    output << dolaresLocal[j].second << endl;
                }
            }
        }
        if (locosTrabajo[i].tipoTrabajo == "N") {
            output << endl;
        }
        //cout << "tipoTrabajo: " << locosTrabajo[i].tipoTrabajo << endl;
        totalWorkVagones += locosTrabajo[i].vagonesLocomotoraTrabajo;
        totalWorkMiles += locosTrabajo[i].millasLocomotoraTrabajo;
        totalWorkTons += locosTrabajo[i].toneladasLocomotoraTrabajo;
    }

    // double GananciaTotalHoy = dolaresRoad[i].second + dolaresTrim[i].second + dolaresHump[i].second + dolaresLocal[i].second;
    double gananciaTotalHoy = 0.0;
    double gananciaTotalHoyRoad = 0.0;
    double gananciaTotalHoyTrim = 0.0;
    double gananciaTotalHoyLocal = 0.0;
    double gananciaTotalHoyHump = 0.0;
    for (size_t i = 0; i < dolaresRoad.size(); i++) {
        gananciaTotalHoyRoad += dolaresRoad[i].second;
    }
    for (size_t i = 0; i < dolaresTrim.size(); i++) {
        gananciaTotalHoyTrim += dolaresTrim[i].second;
    }
    for (size_t i = 0; i < dolaresLocal.size(); i++) {
        gananciaTotalHoyLocal += dolaresLocal[i].second;
    }
    for (size_t i = 0; i < dolaresHump.size(); i++) {
        gananciaTotalHoyHump += dolaresHump[i].second;
    }
    gananciaTotalHoy = gananciaTotalHoyRoad + gananciaTotalHoyTrim + gananciaTotalHoyLocal + gananciaTotalHoyHump;


  //calculo del alquiler mirando en el vector LocomotorasAlquiladas con el precio de UnitNotesDatabase.xml
    double alquilerTotal = 0;
    for (size_t i = 0; i < LocomotorasAlquiladas.size(); i++) {
        alquilerTotal += LocomotorasAlquiladas[i].precioRenting;
    }
    alquilerTotal = alquilerTotal * hHoras;
    double dineroInOut = 0.0;
    dineroInOut = ((hHoras * 100) + gananciaTotalHoy) - alquilerTotal - (SacarValorTrabajado(totalGallons, Galon) * 3.92); //3.92$/gal en 2013 en USA chatGPT dixit


    if (idioma == 's') {
        output << endl;
        output << "FINANZAS($)" << endl;
        output << "-----------" << endl;
        output << "Banco: " << fixed << setprecision(2) << initialBanco + dineroInOut << endl;
        output << "Ingresos Hoy: " << fixed << setprecision(2) << (hHoras * 100) + gananciaTotalHoy << endl;
        output << "Gastos de alquiler hoy: " << LocomotorasAlquiladas.size() << " locomotoras alquiladas por " << fixed << setprecision(2) << hHoras << " horas = $" << alquilerTotal << endl;
        output << "Gastos de combustible : " << SacarValorTrabajado(totalGallons, Galon) << " galones por 3.92$/gal = $" << SacarValorTrabajado(totalGallons, Galon) * 3.92 << endl;
        output << "Total hoy: $" << dineroInOut << endl;
        output.close();
    }
    if (idioma == 'e') {
        output << endl;
        output << "MONEY($)" << endl;
        output << "--------" << endl;
        output << "Bank: " << fixed << setprecision(2) << initialBanco + dineroInOut << endl;
        output << "Today's income: " << fixed << setprecision(2) << (hHoras * 100) + gananciaTotalHoy << endl;
        output << "Today's renting expenses: " << LocomotorasAlquiladas.size() << " rented locomotives for " << fixed << setprecision(2) << hHoras << " hours = $" << alquilerTotal << endl;
        output << "Today's Fuel expenses : " << SacarValorTrabajado(totalGallons, Galon) << " gallons per 3.92$/gal = $" << SacarValorTrabajado(totalGallons, Galon) * 3.92 << endl;
        output << "Today's Total: $" << dineroInOut << endl;
        output.close();
    }
}

#endif