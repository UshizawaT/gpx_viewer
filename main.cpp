#include "GeoCalc.hpp"
#include "ParseXML.hpp"

int main(int argc, char const *argv[]) {
  ParseXML *_xmlp = new ParseXML("data/20221104_odaigahara.gpx");
  GeoCoordinate _p1, _p2;
  _p1.latitude = 35.384367;
  _p1.longitude = 134.533648;
  _p1.elevation = 872.9;
  _p2.latitude = 35.384346;
  _p2.longitude = 134.533623;
  _p2.elevation = 871.7;
  // GeoCalc *_geoc = new GeoCalc(_p1, _p2);
  GeoCalc *_geoc = new GeoCalc();
  _geoc->setCoordinate(_p1, _p2);
  _geoc->calcAll();
  std::cout << "distance: " << _geoc->getDistance() << std::endl;
  return 0;
}
