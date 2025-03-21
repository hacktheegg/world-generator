#ifndef GLMANAGER
#define GLMANAGER

#include <algorithm>
#include <math.h>

namespace window {

struct colour {
  float* r;
  float* g;
  float* b;
  float* a;

  // Default constructor
  colour() : r(nullptr), g(nullptr), b(nullptr), a(nullptr) {}

  // Constructor aith initial values
  colour(float r_val = 0.00f, float g_val = 0.00f,
           float b_val = 0.00f, float a_val = 0.00f) {
    r = new float(r_val);
    g = new float(g_val);
    b = new float(b_val);
    a = new float(a_val);
  }

  // Copg constructor
    colour(const colour& other) {
        r = new float(*other.r);
        g = new float(*other.g);
        b = new float(*other.b);
        a = new float(*other.a);
    }

    // Move constructor
    colour(colour&& other) noexcept {
        r = other.r; g = other.g; b = other.b; a = other.a;
        other.r = other.g = other.b = other.a = nullptr;
    }

    // Assignment operator
    colour& operator=(const colour& other) {
        if (this != &other) {
            delete r; delete g; delete b; delete a;
            r = new float(*other.r);
            g = new float(*other.g);
            b = new float(*other.b);
            a = new float(*other.a);
        }
        return *this;
    }

    // Destructor
    ~colour() {
        delete r; delete g; delete b; delete a;
    }
};

struct position {
  float* x;
  float* y;
  float* z;
  float* w;

  // Default constructor
  position() : x(nullptr), y(nullptr), z(nullptr), w(nullptr) {}

  // Constructor with initial values
  position(float x_val = 0.00f, float y_val = 0.00f,
           float z_val = 0.00f, float w_val = 0.00f) {
    x = new float(x_val);
    y = new float(y_val);
    z = new float(z_val);
    w = new float(w_val);
  }

  // Copy constructor
    position(const position& other) {
        x = new float(*other.x);
        y = new float(*other.y);
        z = new float(*other.z);
        w = new float(*other.w);
    }

    // Move constructor
    position(position&& other) noexcept {
        x = other.x; y = other.y; z = other.z; w = other.w;
        other.x = other.y = other.z = other.w = nullptr;
    }

    // Assignment operator
    position& operator=(const position& other) {
        if (this != &other) {
            delete x; delete y; delete z; delete w;
            x = new float(*other.x);
            y = new float(*other.y);
            z = new float(*other.z);
            w = new float(*other.w);
        }
        return *this;
    }

    // Destructor
    ~position() {
        delete x; delete y; delete z; delete w;
    }
};

class point {
public:
  // Default constructor
  point() : pos(nullptr), col(nullptr) {}

  // Constructor with initial values
  point(position pos1, colour colour1) {
    pos = new position(pos1);
    col = new colour(colour1);
  }

  // Copy constructor
  point(const point& other) {
    if (other.pos != nullptr) {
      pos = new position(*other.pos);
    } else {
      pos = nullptr;
    }

    if (other.col != nullptr) {
      col = new colour(*other.col);
    } else {
      col = nullptr;
    }
  }

  // Move constructor
  point(point&& other) noexcept {
    pos = other.pos;
    col = other.col;
    other.pos = nullptr;
    other.col = nullptr;
  }

  // Assignment operator
  point& operator=(const point& other) {
    if (this != &other) {
      delete pos;
      delete col;
      pos = new position(*other.pos);
      col = new colour(*other.col);
    }
    return *this;
  }

  // Destructor
  ~point() {
    delete pos;
    delete col;
  }

  float* getData() {
    float* data = new float[8];

    data[0] = *pos->x;
    data[1] = *pos->y;
    data[2] = *pos->z;
    data[3] = *pos->w;

    data[4] = *col->r;
    data[5] = *col->g;
    data[6] = *col->b;
    data[7] = *col->a;

    return data;
  }

  position* pos;
  colour* col;
private:
};

class polygon {
public:
  // Default constructor
  polygon() : points(nullptr), size(0) {}

  // Constructor with initial size
  polygon(unsigned int num_points) : size(num_points) {
    points = new point[num_points];
  }

  // Copy constructor
  polygon(const polygon& other) : size(other.size) {
    points = new point[size];
    for (unsigned int i = 0; i < size; i++) {
      points[i] = other.points[i];
    }
  }

  // Move constructor
  polygon(polygon&& other) noexcept : points(other.points), size(other.size) {
    other.points = nullptr;
    other.size = 0;
  }

  // Assignment operator
  polygon& operator=(const polygon& other) {
    if (this != &other) {
      delete[] points;
      size = other.size;
      points = new point[size];
      for (unsigned int i = 0; i < size; i++) {
        points[i] = other.points[i];
      }
    }
    return *this;
  }

  // Move assignment operator
  polygon& operator=(polygon&& other) noexcept {
    if (this != &other) {
      delete[] points;
      points = other.points;
      size = other.size;
      other.points = nullptr;
      other.size = 0;
    }
    return *this;
  }

  // Destructor
  ~polygon() {
    delete[] points;
  }

  // Add a point to the polygon
  void addPoint(const point& pt) {
    point* temp = new point[size + 1];
    for (unsigned int i = 0; i < size; i++) {
      temp[i] = points[i];
    }
    temp[size] = pt;
    delete[] points;
    points = temp;
    size++;
  }

  // Get all point data as a single array
  float* getAllData() {
    float* data = new float[size * 8];  // Each point has 8 floats

    for (unsigned int i = 0; i < size; i++) {
    float* point_data = points[i].getData();

    // Copy 8 floats from current point to main array
    std::copy(point_data, point_data + 8, data + (i * 8));

    delete[] point_data;  // Clean up temporary array
    }

    return data;
  }

  // Get size of polygon
  unsigned int getSize() const {
    return size;
  }

  point* points;
private:
  unsigned int size;

};

}

namespace shapes {

class create {
public:
  static void rectangle(window::polygon *poly, float x1, float y1, float x2, float y2, window::colour colour) {

    poly->addPoint( window::point( window::position( x1, y1, 0.00f, 1.00f ), colour ) );
    poly->addPoint( window::point( window::position( x1, y2, 0.00f, 1.00f ), colour ) );
    poly->addPoint( window::point( window::position( x2, y1, 0.00f, 1.00f ), colour ) );
    poly->addPoint( window::point( window::position( x1, y2, 0.00f, 1.00f ), colour ) );
    poly->addPoint( window::point( window::position( x2, y1, 0.00f, 1.00f ), colour ) );
    poly->addPoint( window::point( window::position( x2, y2, 0.00f, 1.00f ), colour ) );

  };

  static void circle(window::polygon *poly, float x, float y, float radius, float startAngle, float endAngle, float precisionInterval) {

    /*poly->addPoint(
      window::point(
        window::position( x, y, 0.00f, 1.00f ),
        window::colour( 0.00f, 0.00f, 0.00f, 1.00f )
      )
    );*/

    float j = 0;
    float jModulator = 0;
    for (float i = startAngle; i < endAngle; i+=precisionInterval) {
      j = i-(360.00f*jModulator);
      if (j > 360.00f) {
        jModulator++;
        j = i-(360.00f*jModulator);
      }
      poly->addPoint(
        window::point(
          window::position( x+(cos((j/180)*M_PI)*radius), y+(sin((j/180)*M_PI)*radius), 0.00f, 1.00f ),
          window::colour( (1.00f+(cos((j/180)*M_PI)*radius))/2.00f, (1.00f+(sin((j/180)*M_PI)*radius))/2.00f, 0.00f, 1.00f )
        )
      );
    }

  };

};

}

#endif // GLMANAGER
