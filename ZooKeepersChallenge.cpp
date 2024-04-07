#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>

using namespace std;


struct Animal {
    string name;
    string species;
    string sex;
    string color;
    string weight;
    string habitat;
    string birthDate;
    string arrivalDate;
};


map<string, int> speciesCount;


string genUniqueID(string species) {
    speciesCount[species]++;
    string id = species.substr(0, 2);
    id += to_string(speciesCount[species]);
    return id;
}


string genBirthDay(int age, string season) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;

    int birthYear = currentYear - age;
    string birthSeason;

    if (season == "spring")
        birthSeason = "03-15";
    else if (season == "summer")
        birthSeason = "06-15";
    else if (season == "autumn")
        birthSeason = "09-15";
    else if (season == "winter")
        birthSeason = "12-15";
    else 
        birthSeason = to_string(ltm->tm_mon + 1) + "-" + to_string(ltm->tm_mday);

    return to_string(birthYear) + "-" + birthSeason;
}


vector<Animal> readArrivingAnimals(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    vector<Animal> animals;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int age;
        string sex, species, color, weight, habitat, season;
        iss >> age >> sex >> species;
        getline(iss, color, ',');
        getline(iss, weight, ',');
        getline(iss, habitat, ',');
        getline(iss, season, ',');

        string name;
        if (getline(file, name)) {
            Animal animal;
            animal.name = name;
            animal.species = species;
            animal.sex = sex;
            animal.color = color;
            animal.weight = weight;
            animal.habitat = habitat;
            animal.birthDate = genBirthDay(age, season);
            animals.push_back(animal);
        }
    }
    file.close();
    return animals;
}


map<string, string> readAnimalNames(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    map<string, string> names;
    string species, name;
    while (file >> species >> name) {
        names[species] = name;
    }
    file.close();
    return names;
}


void writeZooPopulation(const vector<Animal>& animals, const map<string, string>& names, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << " for writing" << endl;
        exit(1);
    }

    for (const auto& animal : animals) {
        string id = genUniqueID(animal.species);
        file << animal.species << " Habitat: " << id << "; " << animal.name << "; birth date " << animal.birthDate
             << "; " << animal.color << " color; " << animal.sex << "; " << animal.weight << " pounds; from "
             << animal.habitat << "; arrived " << animal.arrivalDate << endl;
    }
    file.close();
}

int main() {
    vector<Animal> arrivingAnimals = readArrivingAnimals("arrivingAnimals.txt");
    map<string, string> animalNames = readAnimalNames("animalNames.txt");

    for (auto& animal : arrivingAnimals) {
        if (animalNames.find(animal.species) != animalNames.end())
            animal.name = animalNames[animal.species];
        animal.arrivalDate = "2024-04-07"; // Current date
    }

    writeZooPopulation(arrivingAnimals, animalNames, "zooPopulation.txt");

    cout << "Zoo population report generated successfully." << endl;

    return 0;
}
