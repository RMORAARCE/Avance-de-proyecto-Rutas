#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <string>
#include <fstream>

// Clase para representar un punto turístico
class PuntoTuristico {
public:
    std::string nombre;
    sf::Vector2f posicion;

    PuntoTuristico(const std::string& nombre, sf::Vector2f posicion)
        : nombre(nombre), posicion(posicion) {}
};


class RutaTuristica {
public:
    std::string nombre;
    std::list<PuntoTuristico> puntos;

    RutaTuristica(const std::string& nombre) : nombre(nombre) {}

    void agregarPunto(const PuntoTuristico& punto) {
        puntos.push_back(punto);
    }

    void eliminarUltimoPunto() {
        if (!puntos.empty()) {
            puntos.pop_back();
        }
    }
};


class GestorRutas {
private:
    sf::RenderWindow window;
    sf::Texture mapaTexture;
    sf::Sprite mapaSprite;
    std::list<RutaTuristica> rutas;
    RutaTuristica* rutaActual = nullptr;

public:
    GestorRutas() : window(sf::VideoMode(800, 600), "Gestor de Rutas Turísticas") {
        if (!mapaTexture.loadFromFile("mapa.png")) {
            std::cerr << "Error al cargar la imagen del mapa.\n";
            exit(1);
        }
        mapaSprite.setTexture(mapaTexture);
    }

    void iniciar() {
        while (window.isOpen()) {
            procesarEventos();
            renderizar();
        }
    }

    void crearNuevaRuta(const std::string& nombre) {
        rutas.emplace_back(nombre);
        rutaActual = &rutas.back();
    }

    void agregarPuntoARutaActual(const std::string& nombre, sf::Vector2f posicion) {
        if (rutaActual) {
            rutaActual->agregarPunto(PuntoTuristico(nombre, posicion));
        }
    }

    void guardarRutasEnArchivo() {
        std::ofstream archivo("rutas_turisticas.txt");
        for (const auto& ruta : rutas) {
            archivo << "Ruta: " << ruta.nombre << "\n";
            for (const auto& punto : ruta.puntos) {
                archivo << "Punto: " << punto.nombre << " (" << punto.posicion.x << ", " << punto.posicion.y << ")\n";
            }
        }
        archivo.close();
    }

private:
    void procesarEventos() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f posicion(event.mouseButton.x, event.mouseButton.y);
                    std::string nombre;
                    std::cout << "Ingrese el nombre del punto: ";
                    std::cin >> nombre;
                    agregarPuntoARutaActual(nombre, posicion);
                }
            }
        }
    }

    void renderizar() {
        window.clear();
        window.draw(mapaSprite);

        // Dibujar todas las rutas
        for (const auto& ruta : rutas) {
            sf::CircleShape circulo(5);
            circulo.setFillColor(sf::Color::Red);

            sf::VertexArray lineas(sf::LinesStrip, ruta.puntos.size());

            int i = 0;
            for (const auto& punto : ruta.puntos) {
                circulo.setPosition(punto.posicion);
                window.draw(circulo);

                lineas[i].position = punto.posicion;
                lineas[i].color = sf::Color::Blue;
                ++i;
            }
            window.draw(lineas);
        }

        window.display();
    }
};

int main() {
    GestorRutas gestor;
    std::string nombreRuta;
    std::cout << "Ingrese el nombre de la nueva ruta: ";
    std::cin >> nombreRuta;
    gestor.crearNuevaRuta(nombreRuta);

    gestor.iniciar();
    gestor.guardarRutasEnArchivo();

    return 0;
}
