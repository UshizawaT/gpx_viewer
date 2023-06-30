#ifndef PARSEXML_HPP
#define PARSEXML_HPP

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

class ParseXML {
 private:
  xercesc::XercesDOMParser *_domParser;
  xercesc::ErrorHandler *_errHandler;
  xercesc::DOMDocument *_domDoc;

  bool initXMLPlatform();
  bool loadXML2Doc(const std::string &filename);

 public:
  ParseXML(const std::string &filename);
  ~ParseXML();
};

ParseXML::ParseXML(const std::string &filename) {
  if (!initXMLPlatform()) exit(1);
}

inline bool ParseXML::initXMLPlatform() {
  try {
    xercesc::XMLPlatformUtils::Initialize();
  } catch (const xercesc::XMLException &e) {
    std::string _message_ = XMLString::transcode(exp.getMessage());
    std::cerr << "Error: Xecers-C can't be initialized!" << std::endl;
    std::cerr << _message_ << std::endl;
    xercesc::XMLString::release(&_message_);
    return false;
  }
  _domParser = new xercesc::XercesDOMParser();
  _domParser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
  _domParser->setDoNamespaces(true);
  _errHandler = new xercesc::HandlerBase();
  _domParser->setErrorHandler(_errHandler);
  return true;
}

inline bool ParseXML::loadXML2Doc(const std::string &filename) {
  try {
    _domParser->parse(filename);
    _domDoc = _domParser->getDocument();
  } catch (const xercesc::XMLException &e) {
    std::string _message_ = XMLString::transcode(exp.getMessage());
    std::cerr << "Error: " << _message_ << std::endl;
    xercesc::XMLString::release(&_message_);
    return false;
  } catch (const xercesc::DOMException &e) {
    std::string _message_ = XMLString::transcode(exp.getMessage());
    std::cerr << "Error: " << _message_ << std::endl;
    xercesc::XMLString::release(&_message_);
    return false;
  } catch (...) {
    std::cerr << "Error: Unexpected exception!" << std::endl;
    return false;
  }
  return true;
}

ParseXML::~ParseXML() {
  delete _domParser;
  delete _errHandler;
}

#endif  // !PARSEXML_HPP