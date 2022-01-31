/* Raitiotie
 *
 * Kuvaus:
 * Ohjelma toteutaa raitiotie linjaston. Ohjelmaan annetaan tiedosto,
 * jossa on linjoja ja niiden pysäkit etäisyyksineen. Käyttäjä voi erilaisilla
 * syötteillä tutkia tutkia raitiotien linjoja. Linjoja ja pysäkkejä voidaan
 * lisätä tai poistaa, pysäkkien välisia etäisyyksia laskea, tulostaa kaikki
 * linjat tai pysäkit aakkosjärjestyksessä, sekä tulostaa näkyviin kaikki tietyn
 * linjan pysäkit etäisyyksien mukaisessa järjestyksessä. Ohjelma tarkistaa
 * käyttäjän syötteet aina kun niitä annetaan. Ohjelma pyörii niin kauan, kunnes
 * käyttäjä syöttää quit- komennon.
 *
 * Ohjelman kirjoittaja
 * Nimi: Simeon Huhtala
 * Käyttäjätunnus: nxsihu
 * E-Mail: simeon.huhtala@tuni.fi
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <algorithm>
#include <math.h>

using Route = std::map <std::string, std::map<std::string, double>>;

// The most magnificent function in this whole program.
// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}

// Funktio split jakaa parametrina annetun merkkijonon osiin parametrina syötetyn
// merkin kohdalta. Split-funktiota käytetaan käyttäjän syötteiden käsittelyyn ja
// syötetyn tiedoston rivin jakamiseen osiin. Jaetut osat tallennetaan vektoriin
// result, joka palautetaan paluuarvona.
std::vector<std::string> split(std::string& s, const char delimiter)
{
    std::vector<std::string> result;
    std::string part;
    bool is_long = false;

    for (char c : s) {
        if (c == '"' and not is_long) {
            is_long = true;
        } else if (c == '"') {
            result.push_back(part);
            part.clear();
            is_long = false;
        } else if (c == delimiter and not is_long) {
            result.push_back(part);
            part.clear();
        } else {
            part += c;
        }
    }
    if (is_long) {
        part.clear();
        result.push_back("");
    }
    if(not (part.empty())) {
        result.push_back(part);
    }
    return result;
}

// Funktio tarkistaa onko tiedoston rivi määrittelyn mukainen ja lisää vektoriin
// row_separated nollan, jos etäisyyttä ei ole annettu tiedoston rivillä.
// Palauttaa totuusarvon.
bool is_line_correct(std::vector <std::string>& row_separated)
{
    std::string separator = " ";

    if (row_separated.size() > 3 or row_separated.size() < 2) {
        return false;
    }
    if (row_separated.at(0) == separator or row_separated.at(1) == separator) {
        return false;
    }
    if (row_separated.size() == 2) {
        row_separated.push_back("0");
    }
    return true;
}

// Funktio tarkistaa onko pysäkki tai etäisyys jo olemassa. Palauttaa totuusarvon.
bool does_stop_or_distance_exist(Route route, const std::string name_of_the_route,
                                 const std::string stop, const double distance)
{
    if(route.find(name_of_the_route) != route.end()) {

        for (auto stopping : route.at(name_of_the_route)) {

            if (stopping.first == stop) {
                std::cout << "Error: Stop/line already exists." << std::endl;
                return true;

            } else if (stopping.second == distance) {
                std::cout << "Error: Stop/line already exists." << std::endl;
                return true;
            } else {
                continue;
            }
        }
    }
    return false;
}

// Funktio tarkistaa onko linja olemassa jo entuudestaan. Käyetään addline- funktion
// yhteydessä. Palauttaa totuusarvon paluuarvona.
bool does_line_exist(const Route& route, std::string line)
{
    if (route.find(line) != route.end()) {
        return false;
    } else {
       return true;
    }
}

// Funktio tarkistaa, onko käyttäjän antamaa syötettä vastaavaa pysäkkiä olemassa.
// Palauttaa totuusarvon.
bool does_stop_exist(const Route& route, std::string stop)
{
    for ( std::pair <std::string, std::map <std::string, double>> key_value_pair :
          route) {

        std::string name_of_the_route = key_value_pair.first;
        std::map <std::string, double> stop_distance = key_value_pair.second;
        if (stop_distance.find(stop) != stop_distance.end()) {
            return false;
        }
    }
    return true;
}

// Funktio tulostaa kaikki linjat aakkosjärjestyksessä. Ei paluuarvoa.
void print_the_lines_where_stop_is_located(const Route& route)
{
    std::cout << "All tramlines in alphabetical order:" << std::endl;

    for ( std::pair <std::string, std::map <std::string, double>>
          key_value_pair : route) {

        std::string name_of_the_route = key_value_pair.first;
        std::map <std::string, double> stop_distance = key_value_pair.second;

        std::cout << name_of_the_route << std::endl;
    }
}

// Funktio tarkistaa kumman pysäkin etäisyys on suurempi. Palauttaa suuruusarvon.
bool is_distance_larger(std::pair<std::string, double>& stop_distance1,
                        std::pair<std::string, double>& stop_distance2)
{
    return stop_distance1.second < stop_distance2.second;
}

// Funktio tulostaa yhden linjan kaikki pysäkit etäisyyden mukaisessa järjestyksessä.
// Ei paluuarvoa.
void print_stops_in_line(std::vector<std::string> parts, const Route& route)
{
    std::vector<std::pair<std::string, double>> stop_distance_pair;

    if (does_line_exist(route, parts.at(1)) == true) {
        std::cout << "Error: Line could not be found." <<std::endl;

    } else {
        std::cout << "Line " << parts.at(1) << " goes through these stops "
                                               "in the order they are listed:"
                  << std::endl;

        for (auto& it : route.at(parts.at(1))) {
            stop_distance_pair.push_back(it);
        }
        sort(stop_distance_pair.begin(), stop_distance_pair.end(), is_distance_larger);

        for ( std::pair <std::string, double> key_value_pair : stop_distance_pair) {
            std::string stop = key_value_pair.first;
            double distance = key_value_pair.second;

            if (stop == "" and distance == 0 ) {
                continue;
            } else {
                std::cout << "- " << stop << " : " << distance << std::endl;
            }
        }
    }
}

// Funktio tulostaa kaikki linjat, joilta käyttäjän syöttämä pysäkki löytyy.
// Ei palauta mitään.
void print_the_lines_where_stop_is_located(std::vector<std::string> parts,
                                           const Route& route)
{
    if (does_stop_exist(route, parts.at(1)) == true) {
        std::cout << "Error: Stop could not be found." <<std::endl;
    }
    else {
        std::cout << "Stop " << parts.at(1) << " can be found on the following lines:"
                  << std::endl;

        for ( std::pair <std::string, std::map <std::string, double>>
              key_value_pair : route) {

            std::string name_of_the_route = key_value_pair.first;
            std::map <std::string, double> stop_distance = key_value_pair.second;

            if (stop_distance.find(parts.at(1)) != stop_distance.end()) {
                std::cout << "- " << name_of_the_route << std::endl;
            }
        }
    }
}

// Funktio tulostaa käyttäjän antamien pysäkkien välisen etäisyyden.
void print_distance_between_stops(std::vector<std::string> parts,
                                  const Route& route)
{
    if (does_line_exist(route, parts.at(1)) == true) {
        std::cout << "Error: Line could not be found." <<std::endl;

    } else if (does_stop_exist(route, parts.at(2)) == true
               or does_stop_exist(route, parts.at(3)) == true) {
        std::cout << "Error: Stop could not be found." <<std::endl;

    } else if (parts.at(2) == parts.at(3)) {
        std::cout << "Distance between " << parts.at(2) << " and "
                  << parts.at(3) << " is 0" << std::endl;

    } else {
        double distance1 = 0;
        double distance2 = 0;

        for ( std::pair <std::string, double> key_value_pair :
              route.at(parts.at(1))) {

            std::string stop = key_value_pair.first;
            double distance = key_value_pair.second;

            if (stop == parts.at(2)) {
                distance1 += distance;
            } else if (stop == parts.at(3)) {
                distance2 += distance;
            } else {
                continue;
            }
        }
        std::cout << "Distance between " << parts.at(2) << " and "
                  << parts.at(3) << " is " << abs(distance1 - distance2) << std::endl;
    }
}

// Funktio tulostaa kaikki pysäkit aakkosjärjestyksessä kutsuttaessa.
void print_all_stops(const Route& route)
{
    std::cout << "All stops in alphabetical order:" << std::endl;
    std::set <std::string> all_stops;

    for ( std::pair <std::string, std::map <std::string, double>>
          key_value_pair : route) {

        std::string name_of_the_ = key_value_pair.first;
        std::map <std::string, double> stop_distance = key_value_pair.second;

        for (std::pair <std::string, double> current_stop : stop_distance) {
            std::string name = current_stop.first;

            if (all_stops.find(name) == all_stops.end()) {
                all_stops.insert(name);
            }
        }
    }
    for (auto single_stop : all_stops) {
        std::cout << single_stop << std::endl;
    }
}

// Funktio lisää linjan parametrina saamaansa tietorakenne viitteeseen.
// Ei paluuarvoa.
void add_line(std::vector<std::string> parts, Route& route)
{
    if (does_line_exist(route, parts.at(1)) == false) {
        std::cout << "Error: Stop/line already exists." <<std::endl;

    } else {
        std::map <std::string, double> stop_distance = {};
        route.insert({parts.at(1), stop_distance});
        std::cout << "Line was added." << std::endl;
    }
}

// Funktio lisää tietorankenteessa jo olemassa olevaan linjaan parin, jossa
// ensimmäisenä on pysäkki ja toisena etäisyys. Ei paluuarvoa.
void add_stop(std::vector<std::string> parts, Route& route)
{
    if (does_line_exist(route, parts.at(1)) == true) {
        std::cout << "Error: Line could not be found." <<std::endl;

    } else if (does_stop_or_distance_exist(route, parts.at(1), parts.at(2),
                                           stod(parts.at(3))) == true) {

    } else {
        route.at(parts.at(1)).insert({parts.at(2), stod(parts.at(3))});
        std::cout << "Stop was added." << std::endl;
    }
}

// Funktio poistaa käyttäjän syöttämän pysäkin kaikilta tietorakenteessa
// olevilta linjoilta. Ei paluuarvoa.
void remove_stop(std::vector<std::string> parts, Route& route)
{
    if (does_stop_exist(route, parts.at(1)) == true) {
        std::cout << "Error: Stop could not be found." <<std::endl;

    } else {
        for ( std::pair <std::string, std::map <std::string, double>>
              key_value_pair : route) {

            std::string name_of_the_route = key_value_pair.first;
            std::map <std::string, double> stop_distance = key_value_pair.second;

            for (std::pair <std::string, double> current_stop_distance : stop_distance) {
                std::string name = current_stop_distance.first;

                if (name == parts.at(1)) {
                    route.at(name_of_the_route).erase(name);
                }
            }
        }
        std::cout << "Stop was removed from all lines." << std::endl;
    }
}

// Funktio lukee käyttäjän syöttämän tiedoston, tarkistaa sen oikeellisuuden
// ja tallentaa sen viitteenä saatuun tietorakenteeseen, jonka avulla sitä
// voidaan käsitellä. Palauttaa totuus arvon, siitä onko tiedosto oikeanlainen.
bool checks_and_saves_file(Route& route)
{
    std::string file_name;
    std::cout << "Give a name for input file: ";
    std::cin >> file_name;
    std::cin.ignore();

    std::ifstream file_in(file_name);
    if (not file_in) {
        std::cout << "Error: File could not be read." << std::endl;
        return false;
    }

    std::vector <std::string> lines;
    std::vector <std::string> row_separated;
    std::string line;

    while (getline(file_in, line)) {
        lines.push_back(line);
    }
    file_in.close();

    for (size_t indeksi = 0; indeksi < lines.size(); ++indeksi) {

        double distance;
        row_separated = split(lines.at(indeksi), ';');

        if (is_line_correct(row_separated) == false) {
            std::cout << "Error: Invalid format in file." << std::endl;
            return false;
        }

        std::string name_of_the_route = row_separated.at(0);
        std::string stop = row_separated.at(1);
        distance = stod(row_separated.at(2));

        if (does_stop_or_distance_exist(route, name_of_the_route,
                                        stop, distance) == true) {

        }
        if (route.find(name_of_the_route) != route.end()) {
            if (route.at(name_of_the_route).find(stop) !=
                    route.at(name_of_the_route).end()) {
                continue;
            } else {
                route.at(name_of_the_route).insert({stop, distance});
            }
        } else {
            std::map <std::string, double> stop_distance = {{stop, distance}};
            route.insert({name_of_the_route, stop_distance});
        }
    }
    return true;
}

// Lukee käyttäjän syötteet ja tallentaa ne vektoriin parts split- funktion
// avulla. Vektorin ensimmäisen merkkijonon mukaan toteutetaan komentoja.
// Lopuksi, kun käyttäjä haluaa lopettaa ohjelman suorituksen, funktio
// palauttaa EXIT_SUCCESS.
bool processes_user_input(Route& route)
{
    while(true) {
        std::string user_input;
        std::cout << "tramway> ";
        getline(std::cin, user_input);

        std::vector<std::string> parts = split(user_input, ' ');

        for (std::string part : parts) {
            if (part.empty()) {
               std::cout << "Error: Invalid input." << std::endl;
               parts.clear();
               break;
            }
        }
        if (parts.size() == 0) {
            continue;
        }
        std::string command = parts.at(0);
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);

        if (command == "LINES" and parts.size() == 1) {
            print_the_lines_where_stop_is_located(route);

        } else if (command == "LINE" and parts.size() > 1) {
            print_stops_in_line(parts, route);

        } else if (command == "STOPS") {
            print_all_stops(route);

        } else if (command == "STOP" and parts.size() > 1) {
            print_the_lines_where_stop_is_located(parts, route);

        } else if (command == "DISTANCE" and parts.size() > 3) {
            print_distance_between_stops(parts, route);

        } else if (command == "ADDLINE" and parts.size() > 1) {
            add_line(parts, route);

        } else if (command == "ADDSTOP" and parts.size() > 3) {
            add_stop(parts, route);

        } else if (command == "REMOVE" and parts.size() > 1) {
            remove_stop(parts, route);

        } else if (command == "QUIT") {
            return EXIT_SUCCESS;

        } else {
            std::cout << "Error: Invalid input." << std::endl;
        }
    }
}

int main()
{
    print_rasse();

    Route route;

    if (not checks_and_saves_file(route)) {
        return EXIT_FAILURE;
    }
    processes_user_input(route);

    return EXIT_SUCCESS;
}
