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

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints), m_ttl(TTL), m_numPoints(numPoints) {
    // Initialize random seed
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> thetaDis(0.0f, static_cast<float>(M_PI / 2));
    uniform_real_distribution<float> velocityDis(100.0f, 500.0f); // Random velocity between -100 and 500
    uniform_real_distribution<float> radiusDis(20.0f, 80.0f);  // Random radius between 20 and 80

    // Initialize m_radiansPerSec to a random angular velocity in the range [0:PI]
    m_radiansPerSec = static_cast<float>(rand()) / RAND_MAX * static_cast<float>(M_PI);

    // Set up the Cartesian plane
    this->m_cartesianPlane.setCenter(0, 0);
    this->m_cartesianPlane.setSize(static_cast<float>(target.getSize().x), -1.0f * static_cast<float>(target.getSize().y));

    // Store the location of the center of this particle on the Cartesian plane
    this->m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    // Assign random pixel velocities
    this->m_vx = velocityDis(gen); 
    this->m_vy = velocityDis(gen); 

    if (rand() % 2 != 0) {
        this->m_vx *= -1;
        this->m_vy *= -1;
    }

    // Assign colors
    m_color1 = Color::White;
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256); // Random color

    // Generate vertices
    float theta = thetaDis(gen);
    float dTheta = 2 * M_PI / (numPoints - 1); // Amount to rotate per vertex

    for (int j = 0; j < numPoints; ++j)
    {
        // Generate a random radius
        float r = radiusDis(gen);

        // Calculate Cartesian coordinates of the vertex
        float dx = r * cos(theta);
        float dy = r * sin(theta);

        // Assign the Cartesian coordinate of the newly generated vertex to m_A
        this->m_A(0, j) = m_centerCoordinate.x + dx;
        this->m_A(1, j) = m_centerCoordinate.y + dy;

        // Increment theta for the next iteration
        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const {
    // Construct a VertexArray
    VertexArray lines(TriangleFan, m_numPoints + 1);

    // Map centerCoordinate from Cartesian to pixel coordinates
    Vector2f pixelCenter = Vector2f(target.mapCoordsToPixel(this->m_centerCoordinate, this->m_cartesianPlane));

    // Assign the center position and color
    lines[0].position = pixelCenter;
    lines[0].color = this->m_color1;

    // Loop to assign positions and colors for each vertex
    for (unsigned short j = 1; j <= m_numPoints; ++j) {

        lines[j].position = Vector2f(target.mapCoordsToPixel(Vector2f(m_A(0, j - 1), m_A(1, j - 1)), this->m_cartesianPlane));
        lines[j].color = m_color2;
    }

    // Draw the VertexArray
    target.draw(lines);
}

void Particle::update(float dt) {
    // Subtract dt from m_ttl
    this->m_ttl -= dt;

    // Rotate the particle
    rotate(dt * this->m_radiansPerSec);

    // Scale the particle
    scale(SCALE);

    // Calculate translation distances
    float dx = this->m_vx * dt;
    float dy = this->m_vy * dt - 0.5f * G * dt * dt; // Apply gravity to vertical velocity

    // Update vertical velocity with gravity
    m_vy += G * dt;

    // Translate the particle
    translate(dx, dy);
}

void Particle::translate(double xShift, double yShift) {
    // Construct a TranslationMatrix
    TranslationMatrix T(xShift, yShift, m_numPoints);

    // Add it to m_A
    this->m_A = T + m_A;

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
    RotationMatrix R(theta);

    // Multiply it by m_A
    this-> m_A = R * m_A;

    // Shift the particle back to its original center
    translate(temp.x, temp.y);
}

void Particle::scale(double c) {
    // Store the value of m_centerCoordinate in a temporary vector
    Vector2f temp(m_centerCoordinate);

    // Shift the particle's center back to the origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    // Construct a ScalingMatrix
    ScalingMatrix S(c);

    // Multiply it by m_A
    this->m_A = S * m_A;

    // Shift the particle back to its original center
    translate(temp.x, temp.y);
}
