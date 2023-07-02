#include "GeoCalc.hpp"

const double GeoCalc::_semimajorAxis = 6378137;  // Unit: m
// const double GeoCalc::_flattening = 1 / 298.257222101; // GRS80
const double GeoCalc::_flattening = 1 / 298.257223563;  // WGS84
const double GeoCalc::_semiminorAxis =
    GeoCalc::_semimajorAxis * (1 - GeoCalc::_flattening);
const double GeoCalc::_sqSecEcc = GeoCalc::_flattening *
                                  (2 - GeoCalc::_flattening) /
                                  pow((1 - GeoCalc::_flattening), 2);

bool GeoCalc::initVariables() {
  if (!initLngParam()) return false;
  if (!initLatParam()) return false;
  _prodEqProj = cos(_convStLat) * cos(_convEndLat);
  _prodPolProj = sin(_convStLat) * sin(_convEndLat);
  _discZone = _prodEqProj * cos(_absDlng) + _prodPolProj;
  return true;
}

bool GeoCalc::initLngParam() {
  if (!_fSetValue[0]) return false;
  double _tmpDlng_ = 0.0;
  _tmpDlng_ = _endPoint.longitude - _startPoint.longitude;
  _dLng = _tmpDlng_;
  if (_tmpDlng_ > PI) {
    _dLng -= TWOPI;
  }
  if (_tmpDlng_ < -PI) {
    _dLng += TWOPI;
  }
  _absDlng = abs(_dLng);
  _coAbsDlng = PI - _absDlng;
  return true;
}

bool GeoCalc::initLatParam() {
  if (!_fSetValue[0]) return false;
  if (_startPoint.latitude == HALFPI) return false;
  if (_endPoint.latitude == HALFPI) return false;
  // ccw(+)
  _halfLat = (_endPoint.latitude - _startPoint.latitude) / 2.;
  _midLat = (_endPoint.latitude + _startPoint.latitude) / 2.;
  _convStLat = atan((1 - _flattening) * tan(_startPoint.latitude));
  _convEndLat = atan((1 - _flattening) * tan(_endPoint.latitude));
  // cw(-)
  if (_dLng < 0) {
    _halfLat = (_startPoint.latitude - _endPoint.latitude) / 2.;
    _convStLat = atan((1 - _flattening) * tan(_endPoint.latitude));
    _convEndLat = atan((1 - _flattening) * tan(_startPoint.latitude));
  }
  _convMidLat = (_convEndLat + _convStLat) / 2.0;
  _convHalfLat = (_convEndLat - _convStLat) / 2.0;
  return true;
}

bool GeoCalc::setThetaZero(double *theta0) {
  const double _rad3deg_ = 3 * TORAD;
  const double _divZone23_ = -cos(_rad3deg_ * cos(_convStLat));
  ///
  // zone 1
  ///
  if (_discZone >= 0) {
    *theta0 = _absDlng * (1 + _flattening * _prodEqProj);
    _zoneIndex = 1;
    return true;
  }
  ///
  // zone 2
  ///
  if (_discZone < 0 && _discZone >= _divZone23_) {
    *theta0 = _coAbsDlng;
    _zoneIndex = 2;
    return true;
  }
  ///
  // zone 3
  ///
  if (_discZone < _divZone23_) {
    _zoneIndex = 3;
    std::cerr << "There's no calculation on zone 3!" << std::endl;
    return false;  // temporary
  }
  return false;
}

bool GeoCalc::calcTheta() {
  double _theta0_ = 0.0;
  double _tmptheta_ = 0.0, _gFunc_, _hFunc_, _discF_, _gamma_, _zetaprime_,
         _dFunc_, _eFunc_;
  if (!setThetaZero(&_theta0_)) return false;
  // std::cout << "Zone: " << _zoneIndex << std::endl;
  const double _stopF_ = 1e-15;
  _tmptheta_ = _theta0_;
  while (true) {
    if (_zoneIndex == 1) {
      _gFunc_ = sqrt(pow(sin(_convHalfLat), 2) * pow(cos(_tmptheta_ / 2.), 2) +
                     pow(cos(_convMidLat), 2) * pow(sin(_tmptheta_ / 2.), 2));
      _hFunc_ = sqrt(pow(cos(_convHalfLat), 2) * pow(cos(_tmptheta_ / 2.), 2) +
                     pow(sin(_convMidLat), 2) * pow(sin(_tmptheta_ / 2.), 2));
    } else if (_zoneIndex > 1) {
      _gFunc_ = sqrt(pow(sin(_convHalfLat), 2) * pow(sin(_tmptheta_ / 2.), 2) +
                     pow(cos(_convMidLat), 2) * pow(cos(_tmptheta_ / 2.), 2));
      _hFunc_ = sqrt(pow(cos(_convHalfLat), 2) * pow(sin(_tmptheta_ / 2.), 2) +
                     pow(sin(_convMidLat), 2) * pow(cos(_tmptheta_ / 2.), 2));
    }
    _sigma = 2 * atan(_gFunc_ / _hFunc_);
    _jFunc = 2 * _gFunc_ * _hFunc_;
    _kFunc = pow(_hFunc_, 2) - pow(_gFunc_, 2);
    _gamma_ = _prodEqProj * sin(_tmptheta_) / _jFunc;
    _Gamma = 1 - pow(_gamma_, 2);
    _zeta = _Gamma * _kFunc - 2 * _prodPolProj;
    _zetaprime_ = _zeta + _prodPolProj;
    _dFunc_ = 1. / 4. * _flattening * (1 + _flattening) -
              3 / 16. * pow(_flattening, 2) * _Gamma;
    _eFunc_ = (1 - _dFunc_ * _Gamma) * _flattening * _gamma_ *
              (_sigma + _dFunc_ * _jFunc *
                            (_zeta + _dFunc_ * _kFunc *
                                         (2 * pow(_zeta, 2) - pow(_Gamma, 2))));
    if (_zoneIndex == 1) {
      _discF_ = _tmptheta_ - _absDlng - _eFunc_;
    } else if (_zoneIndex > 1) {
      _discF_ = _tmptheta_ - _coAbsDlng + _eFunc_;
    }
    if (abs(_discF_) < _stopF_) break;
    _gFunc_ =
        _flattening * pow(_gamma_, 2) * (1 - 2 * _dFunc_ * _Gamma) +
        _flattening * _zetaprime_ * (_sigma / _jFunc) *
            (1 - _dFunc_ * _Gamma + 1 / 2 * _flattening * pow(_gamma_, 2)) +
        1 / 4 * pow(_flattening, 2) * _zeta * _zetaprime_;
    _tmptheta_ -= _discF_ / (1 - _gFunc_);
  }
  _theta = _tmptheta_;
  // std::cout << "Theta: " << _theta << std::endl;
  return true;
}

bool GeoCalc::calcDistance() {
  if (_zoneIndex < 0 || _zoneIndex > 2) return false;
  double _n0_ = _sqSecEcc * _Gamma / pow(sqrt(1 + _sqSecEcc * _Gamma) + 1, 2);
  double _aFunc_ = (1 + _n0_) * (1 + 5 / 4 * pow(_n0_, 2));
  double _bFunc_ = _sqSecEcc * (1 - 3 * pow(_n0_, 2) / 8) /
                   pow(sqrt(1 + _sqSecEcc * _Gamma) + 1, 2);
  _distance =
      (1 - _flattening) * _semimajorAxis * _aFunc_ *
      (_sigma -
       _bFunc_ * _jFunc *
           (_zeta - 1 / 4 * _bFunc_ *
                        (_kFunc * (pow(_Gamma, 2) - 2 * pow(_zeta, 2)) -
                         1 / 6 * _bFunc_ * _zeta * (1 - 4 * pow(_kFunc, 2)) *
                             (3 * pow(_Gamma, 2) - 4 * pow(_zeta, 2)))));
  return true;
}