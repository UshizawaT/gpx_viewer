#ifndef PARSEXML_HPP
#define PARSEXML_HPP

#include <iostream>
#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

using namespace xercesc;

class ParseXML {
 protected:
  XercesDOMParser *_domParser;
  ErrorHandler *_errHandler;
  DOMDocument *_domDoc;
  DOMElement *_domRoot;

  bool initXMLPlatform();
  bool loadXML2Doc(const std::string &filename);
  bool showChildNode(const DOMElement *parent);
  bool showAttribute(const DOMElement *parent);

 public:
  ParseXML(const std::string &filename);
  ~ParseXML();
};

ParseXML::ParseXML(const std::string &filename) {
  if (!initXMLPlatform()) exit(1);
  if (!loadXML2Doc(filename)) exit(1);
  _domRoot = _domDoc->getDocumentElement();
}

inline bool ParseXML::initXMLPlatform() {
  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException &e) {
    char *_message_ = XMLString::transcode(e.getMessage());
    std::cerr << "Error: Xecers-C can't be initialized!" << std::endl;
    std::cerr << _message_ << std::endl;
    XMLString::release(&_message_);
    return false;
  }
  std::cout << "xml platform initialized!" << std::endl;
  _domParser = new XercesDOMParser();
  _domParser->setValidationScheme(XercesDOMParser::Val_Always);
  _domParser->setDoNamespaces(true);
  _errHandler = new HandlerBase();
  _domParser->setErrorHandler(_errHandler);
  return true;
}

inline bool ParseXML::loadXML2Doc(const std::string &filename) {
  try {
    _domParser->parse(filename.c_str());
    _domDoc = _domParser->getDocument();
    return true;
  } catch (const XMLException &e) {
    char *_message_ = XMLString::transcode(e.getMessage());
    std::cerr << "Error: " << _message_ << std::endl;
    XMLString::release(&_message_);
    return false;
  } catch (const DOMException &e) {
    char *_message_ = XMLString::transcode(e.getMessage());
    std::cerr << "Error: " << _message_ << std::endl;
    XMLString::release(&_message_);
    return false;
  } catch (...) {
    std::cerr << "Error: Unexpected exception!" << std::endl;
    return false;
  }
  return true;
}

inline bool ParseXML::showChildNode(const DOMElement *parent) {
  DOMNodeList *_nodelist_ = parent->getChildNodes();
  int _nNode_ = _nodelist_->getLength();
  if (_nNode_ <= 0) return false;
  std::cout << "N Node: " << _nNode_ << std::endl;
  DOMElement *_tmpEle_;
  char *_nodeInfo_;
  for (int i = 0; i < _nNode_; ++i) {
    std::cout << "node index: " << i << std::endl;
    _tmpEle_ = (DOMElement *)_nodelist_->item(i);
    if (((DOMNode *)_tmpEle_)->getNodeType() == DOMElement::ELEMENT_NODE) {
      _nodeInfo_ = XMLString::transcode(((DOMNode *)_tmpEle_)->getNodeName());
      std::cout << "node: " << _nodeInfo_ << std::endl;
    } else if (((DOMNode *)_tmpEle_)->getNodeType() == DOMElement::TEXT_NODE) {
      _nodeInfo_ = XMLString::transcode(_tmpEle_->getTextContent());
      std::cout << "txt: " << _nodeInfo_ << std::endl;
    }
    XMLString::release(&_nodeInfo_);
  }
  return true;
}

inline bool ParseXML::showAttribute(const DOMElement *parent) {
  DOMNamedNodeMap *_domMap_ = parent->getAttributes();
  int _nAtt_ = _domMap_->getLength();
  if (_nAtt_ <= 0) return false;
  DOMAttr *_tmpAtt_;
  char *_attname_, *_attvalue_;
  for (int i = 0; i < _nAtt_; ++i) {
    _tmpAtt_ = static_cast<DOMAttr *>(_domMap_->item(i));
    _attname_ = XMLString::transcode(_tmpAtt_->getName());
    _attvalue_ = XMLString::transcode(_tmpAtt_->getValue());
    std::cout << "Att. " << _attname_ << ": " << _attvalue_ << std::endl;
    XMLString::release(&_attname_);
    XMLString::release(&_attvalue_);
  }
  return true;
}

ParseXML::~ParseXML() {
  delete _domParser;
  delete _errHandler;
}

#endif  // !PARSEXML_HPP