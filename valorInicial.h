#ifndef VALORINICIAL_H
#define VALORINICIAL_H

//El archivo VernometerData.xml se puede alterar solo con cargar RUN8 y a lo mejor tengo que hacerlo para pruebas. Por lo que los valores de una jornada
//de trabajo serían mayores. Así veo el valor cuando ejecute el programa (que significa que quiero empezar jornada económica)

double valorInicialHora() {
    double initialWorkHour = 0.0;
    ifstream file("VernometerData.xml");
    string xmlString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    initialWorkHour = getTotalHours(xmlString);
    return initialWorkHour;
}

double valorInicialMillas() {
    double initialWorkMile = 0.0;
    ifstream file("VernometerData.xml");
    string xmlString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    initialWorkMile = getTotalMiles(xmlString);
    return initialWorkMile;
}

double valorInicialGalon() {
    double initialWorkGallon = 0.0;
    ifstream file("VernometerData.xml");
    string xmlString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    initialWorkGallon = getTotalGallons(xmlString);
    return initialWorkGallon;
}

#endif
