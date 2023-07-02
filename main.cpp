#include "GeoCalc.hpp"
#include "ParseGPX.hpp"

bool judgeSamePoint(GeoCoordinate p1, GeoCoordinate p2) {
  if (p1.latitude != p2.latitude) return false;
  if (p1.longitude != p2.longitude) return false;
  return true;
}

void calcDistance(const double &horizontal, const double &ele1,
                  const double &ele2, double *output) {
  *output = sqrt(pow(horizontal, 2) + pow(ele2 - ele1, 2));
}

int main(int argc, char const *argv[]) {
  // ParseXML *_xmlp = new ParseXML("../data/20230604_hachibuse.gpx");
  ParseGPX *_gpxp = new ParseGPX("../data/20230604_hachibuse.gpx");
  GeoCoordinate _p1, _p2;
  GeoCalc *_geoc = new GeoCalc();
  double _lat, _lng, _ele, _dHor, _dAll, _totald = 0.0;
  /*
  _p1.latitude = 35.384367;
  _p1.longitude = 134.533648;
  _p1.elevation = 872.9;
  _p2.latitude = 35.384346;
  _p2.longitude = 134.533623;
  _p2.elevation = 871.7;*/
  // GeoCalc *_geoc = new GeoCalc(_p1, _p2);
  int _nP = _gpxp->getPointNumber();
  for (int i = 0; i < _nP; ++i) {
    std::cout << "index: " << i << std::endl;
    _gpxp->getPointInfo(i, &_lat, &_lng, &_ele);
    _p2.latitude = _lat;
    _p2.longitude = _lng;
    _p2.elevation = _ele;
    if (i != 0 && !judgeSamePoint(_p1, _p2)) {
      _geoc->setCoordinate(_p1, _p2);
      _geoc->calcAll();
      _dHor = _geoc->getDistance();
      calcDistance(_dHor, _p1.elevation, _p2.elevation, &_dAll);
      std::cout << "distance: " << _dAll << std::endl;
      _totald += _dAll;
    } else if (judgeSamePoint(_p1, _p2)) {
      calcDistance(0.0, _p1.elevation, _p2.elevation, &_dAll);
      std::cout << "distance: " << _dAll << std::endl;
      _totald += _dAll;
    }
    std::cout << "total distance: " << _totald << std::endl;
    _p1 = _p2;
  }
  return 0;
}
