#ifndef TRABAJOLOCOMOTORAS_H
#define TRABAJOLOCOMOTORAS_H
#include <vector>
#include <ctime>
#include <algorithm>
#include "LugarVehiculo.h"
#include "workingValue.h"


using namespace std;

struct cargaVagones {
	int idlocomotive;
	string idWagon;
	double pesoCarga;
};

struct LocomotoraTrabajo {
	int idLocomotoraTrabajo;
	string tipoTrabajo;
	int idTrenTrabajo;
	int vagonesLocomotoraTrabajo;
	double millasLocomotoraTrabajo;
	double toneladasLocomotoraTrabajo;
};


bool trimaba = false;
int tamanyoVector = 0;
int i = 0; //el índice del vector



bool primeroEsLocomotora(string xmlString) { //se que en getTotalValues.h hay una variable primeroLoco pero está dentro de funcion y es local.
	int startTagPos = 0;
	int endTagPos = 0;
	string startTag = "<unitType>"; //buscamos un US_DieselEngine
	string endTag = "</unitType>";
	startTagPos = xmlString.find(startTag);
	endTagPos = xmlString.find(endTag);
	startTagPos += startTag.size();
	string texto = xmlString.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
	string palabra = "US_DieselEngine";
	return (texto.find(palabra) != string::npos) ? true : false;
}



bool trimTag(string xmlString) {
	int startTagPos = 0;
	int endTagPos = 0;
	string startTag = "<destinationTag>"; //para ayudar en seleccion de trabajos buscamos un TRIM en el tag
	string endTag = "</destinationTag>";
	startTagPos = xmlString.find(startTag);
	endTagPos = xmlString.find(endTag);
	startTagPos += startTag.size();
	string texto = xmlString.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
	string palabra = "trim";

	// Convertir ambas cadenas a minúsculas antes de la búsqueda
	for (size_t i = 0; i < texto.length(); i++) {  //size_t = std::string::size_type si no dará un error de signo, ya que no pueden ser negativos los valores de posición de characteres.
		texto[i] = tolower(texto[i]);
	}

	for (size_t i = 0; i < palabra.length(); i++) {
		palabra[i] = tolower(palabra[i]);
	}

	if (texto.find(palabra) != string::npos)  { return true; }
	else { return false; }
}

bool localTag(string xmlString) {
	int startTagPos = 0;
	int endTagPos = 0;
	string startTag = "<destinationTag>"; //buscaremos varias etiquetas de trenes Locales
	string endTag = "</destinationTag>";
	startTagPos = xmlString.find(startTag);
	endTagPos = xmlString.find(endTag);
	startTagPos += startTag.size();
	string texto = xmlString.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
	string etiquetas[] = { "barmcc", "bk001", "c-barwss", "gatx", "g-gtoguc", "g-isihnc", "g-roskin", "gsmeki", "kgtr", "m-winkgm", "pph", "mow", "r-cal", "rswe", "r-vuvu", "tcr", "udobk", "wca","z-imshtl" };
	// Convertir cadena texto encontrada a minusculas antes de la busqueda
	for (size_t i = 0; i < texto.length(); i++) {  //size_t = std::string::size_type si no dará un error de signo
		texto[i] = tolower(texto[i]);
	}

	bool encontrado = false;
	bool encontradoL = false;
	bool encontradoY = false;
	for (size_t i = 0; i < sizeof(etiquetas) / sizeof(*etiquetas); i++) {  //sizeof(etiquetas)=cantidad Bytes dividido cada tamaño elemento de matriz sizeof(*etiquetas)
		if (texto.find(etiquetas[i]) != string::npos) {
			encontrado = true;
			break;
		}
	}

	encontradoL = (!texto.empty() && texto[0] == 'l') ? true : false; //esto es igual a un if () else. ? separa expresion si verdadera : separa si es falsa.

	encontradoY = ((!texto.empty() && texto[0] == 'y') && (!trimTag(xmlString))) ? true : false;

	if (encontrado || encontradoL || encontradoY) {
		return true;
	}
	else {
		return false;
	}
}


bool roadTag(string xmlString) {
	int startTagPos = 0;
	int endTagPos = 0;
	string startTag = "<destinationTag>"; //buscaremos comienzos de letras para determinar trenes de carretera
	string endTag = "</destinationTag>";
	startTagPos = xmlString.find(startTag);
	endTagPos = xmlString.find(endTag);
	startTagPos += startTag.size();
	string texto = xmlString.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
	string etiquetas[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "k", "m", "o", "p", "q", "r", "s", "t", "u", "v", "x", "z"};
	
	// Convertir cadena texto encontrada a minúsculas antes de la búsqueda
	transform(texto.begin(), texto.end(), texto.begin(), ::tolower);

	// Buscar el primer carácter de texto en etiquetas
	int vecesEncontrado = count_if(etiquetas, etiquetas + sizeof(etiquetas) / sizeof(*etiquetas),
		[&](const string& s) { return s[0] == texto[0]; });

	if (!localTag(xmlString)) {
		return (vecesEncontrado > 0);
	}
	else
	{
		return false;
	}
}




bool locoEnReversa(string xmlString) { //busco si la locomotora esta en reversa para hacer trabajos de Trim o Locales. Los Road no van en reversa.
	int startTagPos = 0;
	int endTagPos = 0;
	string startTag = "<boolean>"; //la etiqueta de <reverseDirection> le sigue un <boolean>
	string endTag = "</boolean>";
	startTagPos = xmlString.find(startTag);
	endTagPos = xmlString.find(endTag);
	startTagPos += startTag.size();
	string texto = xmlString.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
	if (texto == "true") { return true; }
	else { return false; }
}



bool locoEnDeparture(string xmlString) { //busco si el train.xml la primera etiqueta es una locomotora y miro a ver si está en una via departure.
	int indiceRuta = 0;
	int indiceVia = 0;
	int startTagPos = 0;
	int endTagPos = 0;
	string startTag = "<unitType>";
	string endTag = "</unitType>";
	startTagPos = xmlString.find(startTag);
	endTagPos = xmlString.find(endTag);
	startTagPos += startTag.size();
	string texto = xmlString.substr(startTagPos, endTagPos - startTagPos); //sacas el texto que está entre el starttag y la longitud HASTA el endtag.
	
	//si es una locomotora, busco donde está en la ruta.
	if (texto == "US_DieselEngine") {
		startTagPos = xmlString.find("<int>", endTagPos);
		startTagPos += sizeof("<int>") - 1;
		endTagPos = xmlString.find("</int>", startTagPos);
		stringstream xmlValueRuta(xmlString.substr(startTagPos, endTagPos - startTagPos)); //stringstream saca el valor en string a valor numérico (si es un número)
		xmlValueRuta >> indiceRuta;
		
		//busco en la via
		startTagPos = xmlString.find("<currentTrackSectionIndex>", endTagPos);
		startTagPos += sizeof("<currentTrackSectionIndex>") - 1;
		endTagPos = xmlString.find("<int>", startTagPos);
		startTagPos = endTagPos;
		startTagPos += sizeof("<int>") - 1;
		endTagPos = xmlString.find("</int>", startTagPos);
		stringstream xmlValueVia(xmlString.substr(startTagPos, endTagPos - startTagPos));
		xmlValueVia >> indiceVia;
		
		//si está en las vias del yard, es que está trimando.
		if (!lineaDeparture(indiceRuta, indiceVia)) {
			return false;
		}
		return true;
	}
	return false;
}


LocomotoraTrabajo funcionLocomotoraTrabajo(char idioma, int LocoID, string tipoTRABAJO, int trenID, int locoVAGON, double locoMILLAS, double locoCARGA, std::vector<LocomotoraTrabajo>& locosTrabajo) {
	bool locoDentroVector = false;
	int tamanyoVector = locosTrabajo.size(); //tamaño del vector creado o no. Creamos un vector por que no sabemos cuantas locomotoras el usuario pillará.
	if (idioma == 's') {
		cout << "Numero de locomotoras ocupadas hoy: " << tamanyoVector - 1 << endl;
	}
	if (idioma == 'e') {
		cout << "Number of Locomotives used today: " << tamanyoVector - 1 << endl;
	}
		if (!locosTrabajo.empty()) { 
			for (i = 0; i < tamanyoVector; i++) {  //miro si la locomotora pasada por función había sido usada.
				//paso el num mayor por que puede el usuario partir el tren, y por lo tanto sumaría ese nuevo num menor (10 = 5 + 5 Si pilla ese otro 5 serían 15, cuando siguen siendo 10 vagones)
				if (LocoID == locosTrabajo[i].idLocomotoraTrabajo){ //Puede haber locomotora con distinto ID pero mismo tren. Se almacenaría, pero solo sacamos las millas en dineros.h
					locoDentroVector = true;
					locosTrabajo[i].vagonesLocomotoraTrabajo = locoVAGON;
					locosTrabajo[i].millasLocomotoraTrabajo = locoMILLAS;
					locosTrabajo[i].toneladasLocomotoraTrabajo = locoCARGA;
					locosTrabajo[i].tipoTrabajo = tipoTRABAJO; //esto será R,T o L para determinar el pago de dineros.h
					break;
				}


			}
		}
		// Agrega una nueva locomotora al vector
		if (locoDentroVector == false) {
			LocomotoraTrabajo LocoNueva = {LocoID, tipoTRABAJO, trenID, locoVAGON, 0.0, locoCARGA }; //salvamos los datos a una variable de la estructura. PASO A CERO LAS MILLAS EN NUEVO ELEMENTO YA QUE SI NO PILLA EL ANTERIOR DEL VECTOR.	
			locosTrabajo.push_back(LocoNueva);  //pero hay que "pushearlos" al vector para salvarlo //push_back
		}
	
	LocomotoraTrabajo LResultado;
	LResultado.idLocomotoraTrabajo = locosTrabajo[i].idLocomotoraTrabajo;
	LResultado.tipoTrabajo = locosTrabajo[i].tipoTrabajo;
	LResultado.idTrenTrabajo = locosTrabajo[i].idTrenTrabajo;
	LResultado.vagonesLocomotoraTrabajo = locosTrabajo[i].vagonesLocomotoraTrabajo;
	LResultado.millasLocomotoraTrabajo = locosTrabajo[i].millasLocomotoraTrabajo;
	LResultado.toneladasLocomotoraTrabajo = locosTrabajo[i].toneladasLocomotoraTrabajo;
	return LResultado;
}



double almacenIDVagones(string xmlString, vector<cargaVagones>& idsVagones, int idLocomotora) { //double& pesoTotal
	int startTagPos = 0;
	int endTagPos = 0;
	double pesoTotal = 0.0;
	bool encontrado = false;
	
	while ((startTagPos = xmlString.find("<unitType>", endTagPos)) != string::npos) {
		startTagPos += sizeof("<unitType>") - 1;
		endTagPos = xmlString.find("</unitType>", startTagPos);
		string texto = xmlString.substr(startTagPos, endTagPos - startTagPos);

		if (texto == "US_Freightcar") {

			// encontrar el peso y sumarlo al peso total
			startTagPos = xmlString.find("<loadWeightUSTons>", endTagPos);
			startTagPos += sizeof("<loadWeightUSTons>") - 1;
			endTagPos = xmlString.find("</loadWeightUSTons>", startTagPos);
			string pesoString = xmlString.substr(startTagPos, endTagPos - startTagPos);
			double peso = stod(pesoString);  //valor del peso de la carga del vagón


			startTagPos = xmlString.find("<unitNumber>", endTagPos);
			startTagPos += sizeof("<unitNumber>") - 1;
			endTagPos = xmlString.find("</unitNumber>", startTagPos);
			string idVagon = xmlString.substr(startTagPos, endTagPos - startTagPos); //cadena de numeros de ID del vagón

			for (auto& vagon : idsVagones) { //esto recorre todo el vector
				if ((vagon.idlocomotive == idLocomotora) && (vagon.idWagon == idVagon)) {
						encontrado = true;
						break;
				}
			}
			int kk = idLocomotora;
			if (!encontrado) {
				cargaVagones vagon1 = { idLocomotora, idVagon, peso };
				idsVagones.emplace_back(vagon1); //emplace_back
			}
		}
	}
	for (auto& vagon : idsVagones) {
		if (vagon.idlocomotive == idLocomotora) {
			pesoTotal += vagon.pesoCarga;
		}
	}
	return pesoTotal;
}

#endif;
