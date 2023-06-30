#ifndef PARSEXML_HPP
#define PARSEXML_HPP

#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

using namespace xercesc;

class ParseXML {
 private:
  XercesDOMParser *_domParser;
  ErrorHandler *_errHandler;
  DOMDocument *_domDoc;

  bool initXMLPlatform();
  bool loadXML2Doc(const std::string &filename);
  bool showChildNode(const DOMElement &parent);

 public:
  ParseXML(const std::string &filename);
  ~ParseXML();
};

ParseXML::ParseXML(const std::string &filename) {
  if (!initXMLPlatform()) exit(1);
  if (!loadXML2Doc(filename)) exit(1);
  if (!showChildNode(_domDoc->getDocumentElement())) exit(1);
}

inline bool ParseXML::initXMLPlatform() {
  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException &e) {
    std::string _message_ = XMLString::transcode(exp.getMessage());
    std::cerr << "Error: Xecers-C can't be initialized!" << std::endl;
    std::cerr << _message_ << std::endl;
    XMLString::release(&_message_);
    return false;
  }
  _domParser = new XercesDOMParser();
  _domParser->setValidationScheme(XercesDOMParser::Val_Always);
  _domParser->setDoNamespaces(true);
  _errHandler = new HandlerBase();
  _domParser->setErrorHandler(_errHandler);
  return true;
}

inline bool ParseXML::loadXML2Doc(const std::string &filename) {
  try {
    _domParser->parse(filename);
    _domDoc = _domParser->getDocument();
  } catch (const XMLException &e) {
    std::string _message_ = XMLString::transcode(exp.getMessage());
    std::cerr << "Error: " << _message_ << std::endl;
    XMLString::release(&_message_);
    return false;
  } catch (const DOMException &e) {
    std::string _message_ = XMLString::transcode(exp.getMessage());
    std::cerr << "Error: " << _message_ << std::endl;
    XMLString::release(&_message_);
    return false;
  } catch (...) {
    std::cerr << "Error: Unexpected exception!" << std::endl;
    return false;
  }
  return true;
}

inline bool ParseXML::showChildNode(const DOMElement &parent) {
  DOMNodeList *_nodelist_ = parent.getChildNodes();
  int _nNode_ = _nodelist_->getLength();
  if (_nNode_ <= 0) return false;
  DOMElement *_tmpEle_;
  std::string _nodeName_ = "", _nodeTxt_ = "";
  for (int i = 0; i < _nNode_; ++i) {
    _tmpEle_ = (DOMElement *)_nodelist_->item(i);
    if ((DOMNode *)_tmpEle_->getNodeType() == DOMElement::ELEMENT_NODE) {
      _nodeName_ = XMLString::transcode((DOMNode *)_tmpEle_->getNodeName());
      std::cout << "node: " << _nodeName_ << std::endl;
      XMLString::release(&_nodeName_);
    } else if ((DOMNode *)_tmpEle_->getNodeType() == DOMElement::TEXT_NODE) {
      _nodeTxt_ = XMLString::transcode(_tmpEle_->getTextContent());
      std::cout << "txt: " << _nodeTxt_ << std::endl;
      XMLString::release(&_nodeTxt_);
    }
  }
  return true;
}

ParseXML::~ParseXML() {
  delete _domParser;
  delete _errHandler;
}

#endif  // !PARSEXML_HPP