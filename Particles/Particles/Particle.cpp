#include "Particle.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

using namespace std;
using namespace sf;


bool Particle::almostEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }


    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_A(2, numPoints), m_ttl(TTL), m_numPoints(numPoints)
{
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initialize m_radiansPerSec to a random angular velocity in the range [0:PI]
    m_radiansPerSec = static_cast<float>(rand()) / RAND_MAX * static_cast<float>(M_PI);

    // Set up the Cartesian plane
    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(static_cast<float>(target.getSize().x), -1.0f * static_cast<float>(target.getSize().y));

    // Store the location of the center of this particle on the Cartesian plane
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    // Assign random pixel velocities
    m_vx = (rand() % 2 != 0 ? -1 : 1) * (rand() % 401 + 100); // Random velocity between -100 and 500
    m_vy = (rand() % 2 != 0 ? -1 : 1) * (rand() % 401 + 100); // Random velocity between -100 and 500

    // Assign colors
    m_color1 = Color::White;
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256); // Random color

    // Generate vertices
    float theta = static_cast<float>(rand()) / RAND_MAX * static_cast<float>(M_PI / 2); // Angle between [0: PI/2]
    float dTheta = 2 * M_PI / (numPoints - 1); // Amount to rotate per vertex

    for (int j = 0; j < numPoints; ++j)
    {
        // Generate a random radius
        float r = static_cast<float>(rand() % 61 + 20); // Random radius between 20 and 80

        // Calculate Cartesian coordinates of the vertex
        float dx = r * cos(theta);
        float dy = r * sin(theta);

        // Assign the Cartesian coordinate of the newly generated vertex to m_A
        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;

        // Increment theta for the next iteration
        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const {
    // Construct a VertexArray
    VertexArray lines(TriangleFan, m_numPoints + 1);

    // Map centerCoordinate from Cartesian to pixel coordinates
    Vector2f center = target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);

    // Assign the center position and color
    lines[0].position = center;
    lines[0].color = m_color1;

    // Loop to assign positions and colors for each vertex
    for (int j = 1; j <= m_numPoints; ++j) {
        // Map vertex position from Cartesian to pixel coordinates
        Vector2f vertexPosition = target.mapCoordsToPixel(Vector2f(m_A(0, j - 1), m_A(1, j - 1)), m_cartesianPlane);

        // Assign vertex position and color
        lines[j].position = vertexPosition;
        lines[j].color = m_color2;
    }

    // Draw the VertexArray
    target.draw(lines);
}

void Particle::update(float dt) {
    // Subtract dt from m_ttl
    m_ttl -= dt;

    // Rotate the particle
    rotate(dt * m_radiansPerSec);

    // Scale the particle
    scale(SCALE);

    // Calculate translation distances
    float dx = m_vx * dt;
    float dy = m_vy * dt - 0.5f * G * dt * dt; // Apply gravity to vertical velocity

    // Update vertical velocity with gravity
    m_vy += G * dt;

    // Translate the particle
    translate(dx, dy);
}

void Particle::translate(double xShift, double yShift) {
    // Construct a TranslationMatrix
    Matrices::TranslationMatrix T(xShift, yShift, m_numPoints);

    // Add it to m_A
    m_A = T + m_A;

    // Update the particle's center coordinate
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}

void Particle::rotate(double theta) {
    // Store the value of m_centerCoordinate in a temporary vector
    Vector2f temp(m_centerCoordinate);

    // Shift the particle's center back to the origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    // Construct a RotationMatrix
    Matrices::RotationMatrix R(theta);

    // Multiply it by m_A
    m_A = R * m_A;

    // Shift the particle back to its original center
    translate(temp.x, temp.y);
}

void Particle::scale(double c) {
    // Store the value of m_centerCoordinate in a temporary vector
    Vector2f temp(m_centerCoordinate);

    // Shift the particle's center back to the origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    // Construct a ScalingMatrix
    Matrices::ScalingMatrix S(c);

    // Multiply it by m_A
    m_A = S * m_A;

    // Shift the particle back to its original center
    translate(temp.x, temp.y);
}

bool Particle::almostEqual(double a, double b, double eps) {
    return fabs(a - b) < eps;
}

// Function to run unit tests
void Particle::unitTests() {
    // Test almostEqual function
    cout << "Testing almostEqual function..." << endl;
    bool testResult = true;

    if (!almostEqual(1.0, 1.0)) {
        cout << "Test 1 failed!" << endl;
        testResult = false;
    }

    if (almostEqual(1.0, 2.0)) {
        cout << "Test 2 failed!" << endl;
        testResult = false;
    }

    if (!almostEqual(0.0, 0.00001)) {
        cout << "Test 3 failed!" << endl;
        testResult = false;
    }
    if (almostEqual(0.0, 0.1)) {
        cout << "Test 4 failed!" << endl;
        testResult = false;
    }

    if (testResult) {
        cout << "All tests passed!" << endl;
    }
}