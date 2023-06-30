#ifndef GEOCALC_HPP
#define GEOCALC_HPP

#include <bitset>
#include <iostream>

#include "GlobalVariables.hpp"

struct GeoCoordinate {
  double latitude;
  double longitude;
  double elevation;
};

struct GeoRadCoordinate {
  double latitude;
  double longitude;
  double elevation;
};

class GeoCalc {
 private:
  GeoRadCoordinate _startPoint, _endPoint;
  ///
  // constants
  ///
  static const double _semimajorAxis;
  static const double _flattening;  // WGS84
  static const double _semiminorAxis;
  static const double _sqSecEcc;
  ///
  // variables
  ///
  double _dLat, _dLng, _absDlng, _coAbsDlng, _midLat, _halfLat;
  double _convStLat, _convEndLat, _convMidLat, _convHalfLat, _prodPolProj,
      _prodEqProj, _discZone, _theta, _Gamma, _sigma, _jFunc, _zeta, _kFunc;
  int _zoneIndex;
  double _distance;
  std::bitset<2> _fSetValue;  // |Calc ok.|set coordinate|

  bool initVariables();
  bool initLngParam();
  bool initLatParam();
  bool setThetaZero(double *theta0);
  bool calcTheta();
  bool calcDistance();

 public:
  GeoCalc();
  GeoCalc(const GeoCoordinate &start, const GeoCoordinate &end);
  void setCoordinate(const GeoCoordinate &start, const GeoCoordinate &end);
  bool calcAll();
  double getDistance();
  ~GeoCalc();
};

GeoCalc::GeoCalc() { _fSetValue = std::bitset<2>(0b0); }

GeoCalc::GeoCalc(const GeoCoordinate &start, const GeoCoordinate &end) {
  _fSetValue = std::bitset<2>(0b0);
  setCoordinate(start, end);
  if (!calcAll()) exit(1);
}

inline void GeoCalc::setCoordinate(const GeoCoordinate &start,
                                   const GeoCoordinate &end) {
  _startPoint.elevation = start.elevation;
  _endPoint.elevation = end.elevation;
  _startPoint.latitude = start.latitude * TORAD;
  _startPoint.longitude = start.longitude * TORAD;
  _endPoint.latitude = end.latitude * TORAD;
  _endPoint.longitude = end.longitude * TORAD;
  _fSetValue.set(0);
}

inline bool GeoCalc::calcAll() {
  if (!initVariables()) {
    std::cerr << "Failed initialize variables!" << std::endl;
    return false;
  }
  if (!calcTheta()) {
    std::cerr << "Failed calculate theta!" << std::endl;
    return false;
  }
  if (!calcDistance()) {
    std::cerr << "Failed calculate distance!" << std::endl;
    return false;
  }
  _fSetValue.set(1);
  return true;
}

inline double GeoCalc::getDistance() {
  if (!_fSetValue[1]) return -1;
  return _distance;
}

GeoCalc::~GeoCalc() {}

#endif  //! GEOCALC_HPP