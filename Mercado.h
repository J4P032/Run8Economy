#ifndef MERCADO_H
#define MERCADO_H

#include <sstream>
#include <iostream>
#include <fstream>
#include "copiaArchivoTexto.h"
using namespace std;

string mercadoXML;
string tipoLocoXML;


void testar(string cadena, int num_cadena2){
    ofstream outfile("./V3RailVehicles/test.txt"); 
    outfile << "tipo tren " << cadena << ": " << num_cadena2 << endl;
    //outfile.close();
}


//lo usaré para sacar de la cadena de UnitStatusDatabase el numero que buscaré en el auto save world mas moderno
int convertirANumero(string cadena) {
    int numero = 0;
    bool empezoNumeros = false;
    for (char c : cadena) {
        if (isdigit(c) && !empezoNumeros) {
            empezoNumeros = true;
            numero = c - '0'; //0 en ascii = 48, 1 = 49 ... 9 = 57 asi se saca el número a int. Aquí saca el primer número
        }
        else if (isdigit(c) && empezoNumeros) { //.. y a ese primer numero le sumamos decenas (luego centenas, miles etc..) con los otros numeros
            numero = numero * 10 + (c - '0');
        }
    }
    return numero;
}


//Saca el tipo de locomotora que es, leyendo el auto Save World y buscando el nombre de la locomotora pasada comparándolo con el ID.
int tipoLocomotora(string cadena) {
    // 1 = C449W dash9; 2 = GP40; 3 = SD40; 4 = SD45; 5 = ES44 GEVO; 6 = SD70
    tipoLocoXML = leeXML("./V3Routes/Regions/SouthernCA/AutoSaves/Auto Save World.xml");

    int startTagPos = 0;
    int endTagPos = 0;
    int idLoco = 0;
    int num_cadena2 = 0;

    // Buscar la etiqueta rvXMLfilename que tiene el modelo de locomotora y extraer su valor
    while ((startTagPos = tipoLocoXML.find("<rvXMLfilename>", endTagPos)) != string::npos) {
        startTagPos += sizeof("<rvXMLfilename>") - 1;
        endTagPos = tipoLocoXML.find("</rvXMLfilename>", startTagPos);
        string cadena2 = tipoLocoXML.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.

        // buscar si la cadena contiene alguna de las locomotoras permitidas
        if (cadena2.find("C449W") != string::npos) {
            num_cadena2 = 1;
        }
        else if (cadena2.find("GP40") != string::npos) {
            num_cadena2 = 2;
        }
        else if (cadena2.find("SD40") != string::npos) {
            num_cadena2 = 3;
        }
        else if (cadena2.find("SD45") != string::npos) {
            num_cadena2 = 4;
        }
        else if (cadena2.find("ES44") != string::npos) {
            num_cadena2 = 5;
        }
        else if (cadena2.find("SD70") != string::npos) {
            num_cadena2 = 6;
        }

        startTagPos = tipoLocoXML.find("<unitNumber>", endTagPos);
        startTagPos += sizeof("<unitNumber>") - 1;
        endTagPos = tipoLocoXML.find("</unitNumber>", startTagPos);
        stringstream xmlValor2(tipoLocoXML.substr(startTagPos, endTagPos - startTagPos)); //stringstream saca el valor en string a valor numérico (si es un número)
        xmlValor2 >> idLoco;

        if (idLoco == convertirANumero(cadena)) {
            return num_cadena2;
        }
        else {
            continue;
        }
    }
    return 0;
}



void mantenimientoLocomotora() {
    //Dash9 SD40 ES44 SD70 (184k millas)
    //GP40 SD45  (92k millas)
    //mantenimiento costo de  100-300k Dolares.
}

long long  precioCompra(int millasHechas, int tipoLoco) {
    long long precioMaximo = 0;
    long long precioMinimo = 0;
    long long precioFinal = 0;
    const int maxMillas = 300000;
    if (tipoLoco == 1) { precioMaximo = 500000; precioMinimo = 300000; } //dash9 C449W (1993-2004)
    if (tipoLoco == 2) { precioMaximo = 150000; precioMinimo = 50000; } //GP40 (1965-1972)
    if (tipoLoco == 3) { precioMaximo = 225000; precioMinimo = 100000; } //SD40 (1966-1986)
    if (tipoLoco == 4) { precioMaximo = 250000; precioMinimo = 150000; } //SD45 (1965-1971)
    if (tipoLoco == 5) { precioMaximo = 2200000; precioMinimo = 1200000;} //ES44 GEVO (2003-hoy)
    if (tipoLoco == 6) { precioMaximo = 2500000; precioMinimo = 1500000; } //SD70 (1992-2019)
    precioFinal = precioMaximo - ((precioMaximo - precioMinimo) * millasHechas / maxMillas);
    return precioFinal;

}

//Alquiler precio por hora
long long precioAlquiler(int millasHechas, int tipoLoco) {
    long long precioAlquilerMax = 0;
    long long precioAlquilerMin = 0;
    long long precioFinalAlquiler = 0;
    const int maxMillas = 300000;
    if (tipoLoco == 1) { precioAlquilerMax = 50; precioAlquilerMin = 35; } //dash9 C449W (1993-2004)
    if (tipoLoco == 2) { precioAlquilerMax = 30; precioAlquilerMin = 10; } //GP40 (1965-1972)
    if (tipoLoco == 3) { precioAlquilerMax = 35; precioAlquilerMin = 13; } //SD40 (1966-1986)
    if (tipoLoco == 4) { precioAlquilerMax = 38; precioAlquilerMin = 15; } //SD45 (1965-1971)
    if (tipoLoco == 5) { precioAlquilerMax = 65; precioAlquilerMin = 42; } //ES44 GEVO (2003-hoy)
    if (tipoLoco == 6) { precioAlquilerMax = 75; precioAlquilerMin = 50; } //SD70 (1992-2019)
    precioFinalAlquiler = precioAlquilerMax - ((precioAlquilerMax - precioAlquilerMin) * millasHechas / maxMillas);
    return precioFinalAlquiler;
}


//escribe la seccion comun de cada locomotora del archivo UnitNotesDatabase.xml
void writeUnitNotesDatabase(ofstream& outfile, string cadena, int millasHechas, char idioma) {
    long long precioVenta = 0;
    long long precioRenting = 0;
    precioVenta = precioCompra(millasHechas, tipoLocomotora(cadena));
    precioRenting = precioAlquiler(millasHechas, tipoLocomotora(cadena));

    outfile << "  <UnitNote>" << endl;
    outfile << "    <roadNameNumber>" << cadena << "</roadNameNumber>" << endl;
    if (idioma == 's') {
        outfile << "    <noteText>" << "Compra: $" << precioVenta << " Alquiler/Hora: $" << precioRenting << "</noteText>" << endl;
    }
    if (idioma == 'e') {
        outfile << "    <noteText>" << "Buy: $" << precioVenta << " Rent/Hour: $" << precioRenting << "</noteText>" << endl;
    }
    outfile << "  </UnitNote>" << endl;
}



void totalMillasMotor(string mercadoXML, char idioma) {
    int startTagPos = 0;
    int endTagPos = 0;
    int idLocomotive = 0;
    int millasHechas = 0;
    int numUnitNotes = 0;

    int totalChars = mercadoXML.length();
    int charsProcesados = 0;
    float porcentajeCompleto = 0.0;

    ofstream outfile("./V3RailVehicles/UnitNotesDatabase.xml"); // Abrir el archivo antes del ciclo while
    outfile << "<?xml version=\"1.0\"?>" << endl;
    outfile << "<ArrayOfUnitNote xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" << endl;

    // Buscar la etiqueta roadNameNumber y extraer su valor
    while ((startTagPos = mercadoXML.find("<roadNameNumber>", endTagPos)) != string::npos) { 
        startTagPos += sizeof("<roadNameNumber>") - 1;
        endTagPos = mercadoXML.find("</roadNameNumber>", startTagPos);
        string cadena = mercadoXML.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
       
        //sacar total millas hechas
        startTagPos = mercadoXML.find("<totalMileage>", endTagPos);
        startTagPos += sizeof("<totalMileage>") - 1;
        endTagPos = mercadoXML.find("</totalMileage>", startTagPos);
        stringstream xmlValor(mercadoXML.substr(startTagPos, endTagPos - startTagPos)); //stringstream saca el valor en string a valor numérico (si es un número)
        xmlValor >> millasHechas; //stringstream saca el valor string de la etiqueta y >> lo pasa a valor entero.

        writeUnitNotesDatabase(outfile, cadena, millasHechas, idioma);
        numUnitNotes++; //incrementa el valor para decirle cuando debe cerrar el archivo
    
    //Actualizar el procentaje completado y mostrarlo en pantalla cada 5.0%
        charsProcesados += endTagPos - startTagPos;
        float nuevoPorcentaje = (float)charsProcesados / (float)totalChars * 100;
        if (nuevoPorcentaje - porcentajeCompleto > 5.0) {
            porcentajeCompleto = nuevoPorcentaje;
            cout << "progreso: " << porcentajeCompleto << "%" << endl;
        }
    
    }

    if (numUnitNotes > 0) {
        outfile << "</ArrayOfUnitNote>" << endl; // Escribir la etiqueta de cierre solo si se escribió al menos una nota de unidad.
    }
    outfile.close(); //cerrar el archivo despues del ciclo while

}


void inicializarMercado(char idioma) {
	mercadoXML = leeXML("./V3RailVehicles/UnitStatusDatabase.xml");
    totalMillasMotor(mercadoXML, idioma);
}

#endif

