#include "Engine.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

Engine::Engine() {
	VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	m_Window.create(desktopMode, "Particle Engine");
}

void Engine::run() {
    // Construct a local Clock object to track time per frame
    Clock clock;
    bool unit_tests_done = false;

    // Construct a local Particle for unit tests


    // Game loop
    while (m_Window.isOpen())
    {
        if (!unit_tests_done) {
            unit_tests_done = true;
            cout << "Starting Particle unit tests..." << endl;
            Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
            p.unitTests();
            cout << "Unit tests complete.  Starting engine..." << endl;
        }

        // Restart the clock
        Time deltaTime = clock.restart();

        // Convert the clock time to seconds
        float dt = deltaTime.asSeconds();

        // Process input
        input();

        // Update the game state
        update(dt);

        // Draw the game
        draw();
    }
}

void Engine::input() {
    Event event;

    while (m_Window.pollEvent(event)) {
        switch (event.type) {
        case Event::Closed:
            m_Window.close();
            break;

        case Event::KeyPressed:
            if (event.key.code == Keyboard::Escape)
                m_Window.close();
            break;
        case Event::MouseButtonPressed:
            if (event.mouseButton.button == Mouse::Left) {
                
                // Create 5 particles

                for (int i = 0; i < 5; ++i) {
                    // Random number of points between 25 and 50
                    int numPoints = rand() % 26 + 25;

                    // Construct a Particle with random number of points and mouse click position
                    Particle particle(m_Window, numPoints, { event.mouseButton.x, event.mouseButton.y });

                    // Add the particle to the vector
                    m_particles.push_back(particle);
                }
            }

            break;

        default:
            break;
        }
    }
}

void Engine::update(float dtAsSeconds) {
    // Iterator-based for-loop to update particles
    for (auto it = m_particles.begin(); it != m_particles.end();) {
        // Check if the particle's time to live has not expired
        if (it->getTTL() > 0.0) {
            // Update the particle
            it->update(dtAsSeconds);

            // Increment the iterator
            it++;
        } else {
            // Erase the particle whose time to live has expired
            it = m_particles.erase(it);
        }
    }
}

void Engine::draw() {
    // Clear the window
    this->m_Window.clear();

    // Loop through each particle and draw it
    for (const auto& particle : this->m_particles) {
        m_Window.draw(particle);
    }

    // Display the window
    this->m_Window.display();
}
