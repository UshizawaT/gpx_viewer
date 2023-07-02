#ifndef PARSEGPX_HPP
#define PARSEGPX_HPP

#include <boost/lexical_cast.hpp>

#include "ParseXML.hpp"

class ParseGPX : public ParseXML {
 private:
  DOMElement *_singleTrk;
  DOMElement *_singleTrkSeg;
  DOMNodeList *_trkPoints;
  int _nPoints;

  bool getTrkNode();
  bool getTrkSegNode();
  bool getInfoFromEachPoint();
  bool getTrkPoints();
  bool showElevInfo(DOMElement *parent);
  bool getLatitudeLongitude(const DOMNode *node, double *lat, double *lng);
  DOMNode *getElevation(DOMElement *parent);

 public:
  ParseGPX(const std::string &filename);
  int getPointNumber();
  bool getPointInfo(const int &index, double *lat, double *lng, double *ele);
  ~ParseGPX();
};

ParseGPX::ParseGPX(const std::string &filename) : ParseXML(filename) {
  if (!getTrkNode()) exit(1);
  if (!getTrkSegNode()) exit(1);
  // if (!showChildNode(_singleTrkSeg)) exit(1);
  // if (!getInfoFromEachPoint()) exit(1);
  if (!getTrkPoints()) exit(1);
}

inline bool ParseGPX::getTrkNode() {
  DOMNodeList *_trkList_ = _domRoot->getChildNodes();
  if (_trkList_->getLength() <= 0) {
    std::cerr << "List length is wrong!" << std::endl;
    return false;
  }
  _singleTrk = (DOMElement *)_trkList_->item(1);
  return true;
}

inline bool ParseGPX::getTrkSegNode() {
  char *_tagname_ = "trkseg";
  DOMNodeList *_trkList_ =
      _singleTrk->getElementsByTagName(XMLString::transcode(_tagname_));
  if (_trkList_->getLength() <= 0) {
    std::cerr << "There's no trkseg node!" << std::endl;
    return false;
  }
  if (_trkList_->getLength() == 1) {
    _singleTrkSeg = (DOMElement *)_trkList_->item(0);
    return true;
  }
  return false;
}

inline bool ParseGPX::getInfoFromEachPoint() {
  _trkPoints =
      _singleTrkSeg->getElementsByTagName(XMLString::transcode("trkpt"));
  _nPoints = _trkPoints->getLength();
  if (_nPoints <= 0) {
    std::cerr << "There's no points!" << std::endl;
    return false;
  }
  for (int i = 0; i < _nPoints; ++i) {
    if (!showAttribute((DOMElement *)_trkPoints->item(i))) continue;
    if (!showElevInfo((DOMElement *)_trkPoints->item(i))) {
      std::cerr << "There's no elevation!" << std::endl;
      continue;
    }
  }

  return true;
}

inline bool ParseGPX::getTrkPoints() {
  _trkPoints =
      _singleTrkSeg->getElementsByTagName(XMLString::transcode("trkpt"));
  _nPoints = _trkPoints->getLength();
  if (_nPoints <= 0) {
    std::cerr << "There's no points!" << std::endl;
    return false;
  }
  return true;
}

inline bool ParseGPX::showElevInfo(DOMElement *parent) {
  DOMNodeList *_elevNodes_ =
      parent->getElementsByTagName(XMLString::transcode("ele"));
  if (_elevNodes_->getLength() != 1) return false;
  DOMNode *_elev_ = _elevNodes_->item(0);
  char *_tagName_, *_tagTxt_;
  _tagName_ = XMLString::transcode(_elev_->getNodeName());
  _tagTxt_ = XMLString::transcode(_elev_->getTextContent());
  std::cout << "node " << _tagName_ << ": " << _tagTxt_ << std::endl;
  XMLString::release(&_tagName_);
  XMLString::release(&_tagTxt_);
  return true;
}

inline DOMNode *ParseGPX::getElevation(DOMElement *parent) {
  return (parent->getElementsByTagName(XMLString::transcode("ele")))->item(0);
}

inline bool ParseGPX::getLatitudeLongitude(const DOMNode *node, double *lat,
                                           double *lng) {
  DOMNamedNodeMap *_domMap_ = node->getAttributes();
  int _nAtt_ = _domMap_->getLength();
  if (_nAtt_ <= 0) return false;
  DOMAttr *_tmpAtt_;
  char *_attname_, *_attvalue_;
  for (int i = 0; i < _nAtt_; ++i) {
    _tmpAtt_ = static_cast<DOMAttr *>(_domMap_->item(i));
    _attname_ = XMLString::transcode(_tmpAtt_->getName());
    _attvalue_ = XMLString::transcode(_tmpAtt_->getValue());
    if (strstr(_attname_, "lat") != NULL) {
      *lat = boost::lexical_cast<double>(_attvalue_);
    } else if (strstr(_attname_, "lon") != NULL) {
      *lng = boost::lexical_cast<double>(_attvalue_);
    }
    XMLString::release(&_attname_);
    XMLString::release(&_attvalue_);
  }
  std::cout << "lat: " << *lat << ", lng: " << *lng << std::endl;
  return true;
}

inline bool ParseGPX::getPointInfo(const int &index, double *lat, double *lng,
                                   double *ele) {
  if (_nPoints <= 0) return false;
  DOMNode *_trkPoint_ = _trkPoints->item(index);
  if (!getLatitudeLongitude(_trkPoint_, lat, lng)) return false;
  DOMNode *_eleNode_ = getElevation((DOMElement *)_trkPoint_);
  char *_elevChar_;
  _elevChar_ = XMLString::transcode(_eleNode_->getTextContent());
  *ele = boost::lexical_cast<double>(_elevChar_);
  XMLString::release(&_elevChar_);
  return true;
}

inline int ParseGPX::getPointNumber() { return _nPoints; }

ParseGPX::~ParseGPX() {}

#endif  // !PARSEGPX_HPP