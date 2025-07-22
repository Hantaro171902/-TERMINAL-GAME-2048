#ifndef POINT_2D_H
#define POINT_2D_H

#include <tuple>

class point2D_t {
public:
  using point_datatype_t = std::tuple<int, int>;

  enum class PointCoord { COORD_X, COORD_Y };

  point2D_t() = default;
  point2D_t(int x, int y) : point_vector(std::make_tuple(x, y)) {}
  explicit point2D_t(const point_datatype_t pt) : point_vector{pt} {}

  template<PointCoord dimension>
  int get() const {
    return std::get<static_cast<int>(dimension)>(point_vector);
  }

  template<PointCoord dimension>
  void set(int value) {
    std::get<static_cast<int>(dimension)>(point_vector) = value;
  }

  point_datatype_t get() const { return point_vector; }
  void set(point_datatype_t value) { point_vector = value; }
  void set(int x, int y) { point_vector = std::make_tuple(x, y); }

  point2D_t& operator+=(const point2D_t& pt) {
    point_vector = std::make_tuple(
      get<PointCoord::COORD_X>() + pt.get<PointCoord::COORD_X>(),
      get<PointCoord::COORD_Y>() + pt.get<PointCoord::COORD_Y>()
    );
    return *this;
  }

  point2D_t& operator-=(const point2D_t& pt) {
    point_vector = std::make_tuple(
      get<PointCoord::COORD_X>() - pt.get<PointCoord::COORD_X>(),
      get<PointCoord::COORD_Y>() - pt.get<PointCoord::COORD_Y>()
    );
    return *this;
  }

private:
  point_datatype_t point_vector{};
};

inline point2D_t operator+(point2D_t l, const point2D_t& r) {
  l += r;
  return l;
}

inline point2D_t operator-(point2D_t l, const point2D_t& r) {
  l -= r;
  return l;
}

#endif
