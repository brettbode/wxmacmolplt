/*
 *  (c) 2003-2005 Iowa State University
 *      see the LICENSE file in the top level directory
 */

//	XML.cpp
//
//	Custom XML classes with a DOM-like interface. The implementation can use
//	either custom parsing functions or act as a wrapper for Xercesc
//

#include <vector>
#include <string>
#include <sstream>
#include <wx/gdicmn.h>
#include "XML.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cctype>
#ifdef UseXERCESC
#include <xercesc/util/PlatformUtils.hpp>
#if (XERCES_VERSION_MAJOR > 1)
#include <xercesc/dom/DOM.hpp>
#else
#include <xercesc/parsers/DOMParser.hpp>
#endif
#endif

#ifdef _MSC_VER
#define strcasecmp strcmp
#endif

using std::cerr;
using std::endl;

//using BambooLib::logger;

bool XMLSetup(void) {
	bool result = false;
	//Nothing todo for custom parser
#ifdef UseXERCESC
	try {
		XMLPlatformUtils::Initialize();
	}
	catch(const XMLException& toCatch) {
		cerr << "Error during Xerces-c Initilization.\n"
			<< " Exception message:"
			<< XMLString::transcode(toCatch.getMessage()) << endl;
		result = true;
	}
#endif

	return result;
}

void XMLShutdown(void) {
	//Nothing todo for custom parser
#ifdef UseXERCESC
	XMLPlatformUtils::Terminate();
#endif
}

//simple memory initializer
void XMLDocument::clear(void) {
	root = NULL;
	Buffer = NULL;
	uri = NULL;
	bufferLength = 0;
	useCustom = true;

#ifdef UseXERCESC
	doc = NULL;
	parser = NULL;
	lSrc = NULL;
#endif
}

XMLDocument::XMLDocument(void) {
	clear();
}


XMLDocument::XMLDocument(const char * rootName,  bool custom, const char * uriReq) {
	clear();
	
	useCustom = custom;
	
	if (uriReq != NULL) {
		int n = strlen(uriReq);
		uri = new char[n+1];
		strncpy(uri, uriReq, n);
	} else {	//add a default uri
		int n = strlen("http://sss.scl.ameslab.gov/");
		uri = new char[n+1];
		strncpy(uri, "http://sss.scl.ameslab.gov/", n);
	}

#ifdef UseXERCESC
	if (!custom) {
#if (XERCES_VERSION_MAJOR > 1)
			XMLCh * be = XMLString::transcode(rootName);
			XMLCh * xuri = XMLString::transcode(uri);
			static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
		DOMImplementation * di = DOMImplementationRegistry::getDOMImplementation(gLS);
		doc = di->createDocument(xuri, be, NULL);

#if (XERCES_VERSION_MINOR > 1)
		XMLString::release(&be);
		XMLString::release(&xuri);
#else
		delete be;
		delete xuri;
#endif
		
		root = new XMLElement(doc->getDocumentElement(), doc);
#else
		DOM_DOMImplementation impl;
		doc = impl.createDocument("http://sss.scl.ameslab.gov/", rootName, DOM_DocumentType());

		DOM_Element t = doc.getDocumentElement();
		root = new XMLElement(t, doc);
#endif
	} else
#endif
	{
		root = new XMLElement(rootName);
	}
}

XMLDocument::XMLDocument(const char * MemBuffer, const long & bufLength, bool custom) {
	clear();
	useCustom = custom;
	Buffer = MemBuffer;
	bufferLength = bufLength;
}

XMLDocument::~XMLDocument(void) {
#ifdef UseXERCESC
	if (parser!=NULL) delete parser;
#if (XERCES_VERSION_MAJOR > 1)
	else if(doc != NULL)
	{
		delete doc;
	}
#endif
	if (lSrc != NULL) delete lSrc;
	
#endif
	if(root != NULL) delete root;
	if (uri) delete [] uri;
	if (Buffer) delete [] Buffer;
}

bool XMLDocument::parse(void) {
	bool errorsOccurred = false;

#ifdef UseXERCESC
	if (!useCustom) {
		try {
#if (XERCES_VERSION_MAJOR > 1)
			parser = new XercesDOMParser;
#else
			parser = new DOMParser;
#endif
			lSrc = new MemBufInputSource((XMLByte *) Buffer, bufferLength,
				"ServerRequest", false);
			parser->parse(*lSrc);
			errorsOccurred = (parser->getErrorCount() > 0)?true:false;  
		}
		catch(const XMLException& e) {
			cerr << "An error occured during parsing\n   Message: "
				<< e.getMessage() << endl;
			errorsOccurred = true;
		}
#if (XERCES_VERSION_MAJOR > 1)
		catch (const DOMException& e)
#else
		catch (const DOM_DOMException& e)
#endif
		{
			cerr << "A DOM error occured during parsing\n   DOMException code: "
				<< e.code << endl;
			errorsOccurred = true;
		}
		catch (...)
		{
			cerr << "An error occured during parsing\n " << endl;
			errorsOccurred = true;
		}

		if(!errorsOccurred) {
#if (XERCES_VERSION_MAJOR > 1)
			doc = parser->getDocument();
			DOMElement * xroot = doc->getDocumentElement();
#else
			doc = parser->getDocument();
			DOM_Element xroot = doc.getDocumentElement();
#endif
			if (xroot != NULL) {
				root = new XMLElement(xroot, doc);
			}
		}
	} else
#endif
	{	//simple XML parser
		int p=0, i;
		while (Buffer[p] != '<') {
			if (Buffer[p] == '\0') return false;	//parsing failed
			p++; //advance to the first element token
		}
		if (Buffer[p+1] == '?') {	//parse for uri declaration
			p++;
			while (std::isspace(Buffer[p])) p++;
			if ((!strncmp("xml", &(Buffer[p]), 3))&&(std::isspace(Buffer[p+3]))) { //xml header declaration
			}
			while ((Buffer[p]!='\0')&& !((Buffer[p]=='?')&&(Buffer[p+1]=='>'))) p++;
			while (Buffer[p] != '<') {	//Advance to the start of the root element
				if (Buffer[p] == '\0') return false;	//parsing failed
				p++; //advance to the first element token
			}
		}
		try {
			i=0;
			root = new XMLElement(&(Buffer[p]), i, 0);
			//Basic parsing is finished. Now process Namespace information
			//I think I need something like two vectors, one for tags, one for namespace ids
			std::vector<std::string> tagNames;
			std::vector<int> tagIds;
			root->ParseNamespaceTags(tagNames, tagIds);
			//Finally clear off namespace tags
			root->StripNamespaceTags();
		}
		catch (...) {
			if (root!=NULL) {
				delete root;
				root = NULL;
			}
			errorsOccurred = true;
		}
	}

	if (errorsOccurred) {
//		logger.logEvent("XMLDocument: Error parsing message:", QLOG_ERR);
//		if (Buffer)
//			logger.logEvent(Buffer, QLOG_ERR);
	}
	return (!errorsOccurred);
}

XMLElement * XMLDocument::getDocumentRoot(void) const {
	return root;
}

std::ostream & operator << (std::ostream & target, const XMLDocument & toWrite) {
	if (toWrite.getDocumentRoot() != NULL) {
		target << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
#ifdef UseXERCESC
		if (!toWrite.Custom()) {
#if (XERCES_VERSION_MAJOR > 1)
			DOMDocument * d;
#else
			DOM_Document d;
#endif
			d = toWrite.getDOMDoc();
			target << d;
		} else
#endif
		{
			target << *(toWrite.getDocumentRoot());
		}
	}
	return target;
}

XMLAttribute::XMLAttribute(void) {
	name = value = NULL;
	next = NULL;
	namespaceID = 0;
}


XMLAttribute::XMLAttribute(const char * n, const char * v) {
	name = value = NULL;
	next = NULL;
	namespaceID = 0;
	if (n != NULL) {
		int i = strlen(n);
		name = new char[i+1];
		strcpy(name, n);
		if (v != NULL) {
			i = strlen(v);
			value = new char [i+1];
			strcpy(value, v);
		}
	}
}

XMLAttribute::~XMLAttribute(void) {
	if (name) delete [] name;
	if (value) delete [] value;
}

void XMLAttribute::setNextAttribute(XMLAttribute * n) {
	if ((n != NULL)&&(next == NULL)) next = n;
}

void XMLAttribute::setName(char * n) {
	if (n) {
		if (name) {
			delete [] name;
			name = NULL;
		}
		int i = strlen(n);
		name = new char [i+1];
		strcpy(name, n);
	}
}

void XMLAttribute::setValue(char * v) {
	if (v) {
		if (value) {
			delete [] value;
			value = NULL;
		}
		int i = strlen(v);
		value = new char [i+1];
		strcpy(value, v);
	}
}

bool XMLAttribute::isName(const char * target) const {
	bool result = false;
	
	if (strcmp(name, target) == 0) result = true;
	
	return result;
}

std::ostream & operator << (std::ostream & target, const XMLAttribute & toWrite) {
	target << toWrite.getName() << "=\""
		<< toWrite.getValue() << "\"";
	return target;
}

void XMLElement::Initialize(void) {
	name = value = NULL;
	first = last = NULL;
	firstChild = lastChild = nextChild = NULL;
	namespaceID = 0;
	
#ifdef UseXERCESC
	xElement = NULL;
	doc = NULL;
#endif
}

XMLElement::XMLElement(void) {
	Initialize();
}
XMLElement::XMLElement(const char * Buffer, int & bytesConsumed, int recursionDepth) {
	Initialize();
		//Upon entry Buffer should point to the opening <
	int p=1, i;
	while (std::isspace(Buffer[p])) p++;
	i = p;
	while ((!std::isspace(Buffer[i]))&&(Buffer[i]!='>')&&(Buffer[i]!='/')) {
		if (Buffer[i]=='\0') throw XMLParseError();
		i++;
	}
	if ((i-p)<=0) throw XMLParseError();
	name = new char[(i-p)+1];
	strncpy(name, &(Buffer[p]), (i-p));
	name[(i-p)] = '\0';
	p = i;
	bool done=false;
	while (!done) {
		while (std::isspace(Buffer[p])) p++;
		if (Buffer[p]=='\0') {
			bytesConsumed = p;
			return;
		}
		if (Buffer[p] == '>') {//done with attributes, move on to children
			p++;
			break;
		}
		if ((Buffer[p] =='/')&&(Buffer[p+1]=='>')) {//close element, no children
			bytesConsumed = p+2;
			return;
		}
		i = p;
		while (Buffer[i]!='=') {
			if (Buffer[i]=='\0') throw XMLParseError();
			if ((Buffer[i]=='>')||(Buffer[i]=='/')) throw XMLParseError(); //illegal in this context
			i++;
		}
		int j = i+1;
		while (std::isspace(Buffer[j])) j++;
		if (Buffer[j]!='\"') throw XMLParseError();
		j++;
		int k = j+1;
		while (Buffer[k]!='\"') {	//find end of attribute value
			if (Buffer[k]=='\0') throw XMLParseError();
			k++;
		}
		if ((i-p)>0) {
			char * tname = new char[(i-p)+1];
			strncpy(tname, &(Buffer[p]), (i-p));
			tname[(i-p)] = '\0';
			char * tval = new char[(k-j)+1];
			strncpy(tval, &(Buffer[j]), (k-j));
			tval[(k-j)] = '\0';
			addAttribute(tname, tval);
			delete [] tname;
			delete [] tval;
		}
		p = k+1;
	}
	//Have reached the element content (text node or child elements)
	int cstart = p;
	while (std::isspace(Buffer[p])) p++;
	if ((Buffer[p]=='<')&&(Buffer[p+1]!='/')) {	//one or more child nodes
		done = false;
		while (!done) {
			int b=0;
			XMLElement * child = new XMLElement(&(Buffer[p]), b, recursionDepth+1);
			appendChild(*child);
			p+=b;
			while (std::isspace(Buffer[p])) p++;
			if ((Buffer[p]=='<')&&(Buffer[p+1]=='/')) done=true;
		}
	} else {	//text node
		int textEnd = p;
		while ((Buffer[textEnd]!='<')&&(Buffer[textEnd]!='\0')) textEnd++;
		if (Buffer[textEnd]=='\0') throw XMLParseError();
		value = new char[(textEnd-cstart)+1];
		//copy string over and de-hash several special chars
		int c, nc=0;
		for (c=0; c<(textEnd-cstart); c++) {
			if (Buffer[cstart+c]!='&') {
				value[nc] = Buffer[cstart+c];
			} else {
				if (!strncmp("&amp;", &(Buffer[cstart+c]), 5)) {
					value[nc] = '&';
					c += 4;
				} else if (!strncmp("&lt;", &(Buffer[cstart+c]), 4)) {
					value[nc] = '<';
					c += 3;
				} else if (!strncmp("&gt;", &(Buffer[cstart+c]), 4)) {
					value[nc] = '>';
					c += 3;
				} else if (!strncmp("&#xD;", &(Buffer[cstart+c]), 5)) {
					value[nc] = '\13';
					c += 4;
				} else {	//some other hash or an illegal char
					value[nc] = '&';
				}
			}
			nc++;
		}
		value[nc] = '\0';
		p = textEnd;
	}
	//should be at the beginning of the closing element
	if ((Buffer[p]=='<')&&(Buffer[p+1]!='/')) throw XMLParseError();
	p+=2;
	while (std::isspace(Buffer[p])) p++;
	i=p;
	while ((Buffer[i]!='>')&&!std::isspace(Buffer[i])) {
		if (Buffer[i] == '\0') throw XMLParseError();
		i++;
	}
	if (strncmp(name, &(Buffer[p]), (i-p))) {	//a closing tag doesn't match the opening!
		throw XMLParseError();
	}
	p += strlen(name);
	while (std::isspace(Buffer[p])) p++;
	if (Buffer[p]=='>') p++;	//This should always be true!
	
	bytesConsumed = p;
}

XMLElement::XMLElement(const char * n, const char * v) {
	Initialize();
	
	if (n != NULL) {
		int i = strlen(n) + 1;
		name = new char[i];
		strcpy(name, n);
		if (v != NULL) {
			i = strlen(v) + 1;
			value = new char[i];
			strcpy(value, v);
		}
	}
}
#ifdef UseXERCESC
#if (XERCES_VERSION_MAJOR > 1)
XMLElement::XMLElement(DOMElement * target, DOMDocument * d) {
	Initialize();

	xElement = target;
	doc = d;
	const XMLCh * xName = xElement->getTagName();
	if (xName != NULL) {
		name = XMLString::transcode(xName);
	}

	DOMNamedNodeMap * attributes = xElement->getAttributes();
	int attrCount = attributes->getLength();
	for (int i = 0; i < attrCount; i++) {
		DOMNode * attribute = attributes->item(i);
	    const XMLCh * dsname = attribute->getNodeName();
	    char * sname = XMLString::transcode(dsname);
		const XMLCh * dsvalue =  attribute->getNodeValue();
		char * svalue = XMLString::transcode(dsvalue);
		XMLAttribute * a = new XMLAttribute(sname, svalue);
		if (sname) delete [] sname;
		if (svalue) delete [] svalue;
		if (first != NULL) {
			last->setNextAttribute(a);
			last = a;
		} else {
			first = last = a;
		}
	}

	DOMNodeList * elist = xElement->getChildNodes();
	for (int i=0; i<elist->getLength(); i++) {
		DOMNode * node = elist->item(i);
		switch (node->getNodeType()) {
			case DOMNode::TEXT_NODE:
				{
					const XMLCh * xValue = node->getNodeValue();
					if (xValue != NULL) {
						value = XMLString::transcode(xValue);
					}
				}
			break;
			case DOMNode::ELEMENT_NODE :
				XMLElement * xchild = new XMLElement((DOMElement *) node, doc);
				if (firstChild != NULL) {
					lastChild->setNextChild(*xchild);
					lastChild = xchild;
				} else {
					firstChild = lastChild = xchild;
				}
			break;
		}
	}
}
XMLElement::XMLElement(DOMDocument * d, const char * elementName, const char * elementValue) {
	Initialize();
	
	doc = d;

		XMLCh * c = XMLString::transcode(elementName);
	xElement = doc->createElement(c);
#if (XERCES_VERSION_MINOR > 1)
	XMLString::release(&c);
#else
	delete c;
#endif
	
	if (elementValue != NULL) {
		c = XMLString::transcode(elementValue);
		DOMText * ltext = doc->createTextNode(c);
		xElement->appendChild(ltext);
#if (XERCES_VERSION_MINOR > 1)
		XMLString::release(&c);
#else
		delete c;
#endif
	}
}
#else
XMLElement::XMLElement(DOM_Element & target, DOM_Document d) {
	Initialize();

	xElement = target;
	doc = d;
	DOMString xName = xElement.getTagName();
	if (xName != NULL) {
		name = xName.transcode();
	}

	DOM_NamedNodeMap attributes = xElement.getAttributes();
	int attrCount = attributes.getLength();
	for (int i = 0; i < attrCount; i++) {
		DOM_Node  attribute = attributes.item(i);
	    DOMString dsname = attribute.getNodeName();
	    char * sname = dsname.transcode();
		DOMString dsvalue =  attribute.getNodeValue();
		char * svalue = dsvalue.transcode();
		XMLAttribute * a = new XMLAttribute(sname, svalue);
		if (sname) delete [] sname;
		if (svalue) delete [] svalue;
		if (first != NULL) {
			last->setNextAttribute(a);
			last = a;
		} else {
			first = last = a;
		}
	}
	
	DOM_NodeList elist = xElement.getChildNodes();
	for (int i=0; i<elist.getLength(); i++) {
		DOM_Node node = elist.item(i);
		switch (node.getNodeType()) {
			case DOM_Node::TEXT_NODE:
				{
					DOMString xValue = node.getNodeValue();
					if (xValue != NULL) {
						value = xValue.transcode();
					}
				}
			break;
			case DOM_Node::ELEMENT_NODE:
				XMLElement * xchild = new XMLElement((DOM_Element &) node, doc);
				if (firstChild != NULL) {
					lastChild->setNextChild(*xchild);
					lastChild = xchild;
				} else {
					firstChild = lastChild = xchild;
				}
			break;
		}
	}
}
XMLElement::XMLElement(DOM_Document d, const char * elementName, const char * elementValue) {
	Initialize();

	doc = d;

	xElement = doc.createElement(elementName);
	if (elementValue != NULL) {
		DOM_Text ltext = doc.createTextNode(elementValue);
		xElement.appendChild(ltext);
	}
}
#endif
#endif

XMLElement::~XMLElement(void) {
	if (name) delete [] name;
	if (value) delete [] value;
	XMLAttribute * a = first;
	while (a != NULL) {
		XMLAttribute * b = a->getNextAttribute();
		delete a;
		a = b;
	}
	while (firstChild != NULL) {
		XMLElement * e = firstChild->getNextChild();
		delete firstChild;
		firstChild = e;
	}
}

XMLElement * XMLElement::addChildElement(const char * elementName,
		const char * elementValue) {
	XMLElement * result = NULL;
#ifdef UseXERCESC
	if (doc != NULL) {
		result = new XMLElement(doc, elementName, elementValue);
	} else
#endif
	{
		result = new XMLElement(elementName, elementValue);
	}
	appendChild(*result);
	return result;
}
XMLElement * XMLElement::addBoolChildElement(const char * name, bool value) {
	XMLElement * result=NULL;
	if (value) result = addChildElement(name, trueXML);
	else result = addChildElement(name, falseXML);
	return result;
}
XMLElement * XMLElement::addChildElementToFront(const char * elementName, const char * elementValue)
{
	XMLElement * result = NULL;
#ifdef UseXERCESC
	if (doc != NULL) 
	{
		result = new XMLElement(doc, elementName, elementValue);
	} 
	else
#endif
	{
		result = new XMLElement(elementName, elementValue);
	}
	prependChild(*result);
	return result;
}
const char * XMLElement::getAttributeValue(const char * target) const {
	const char * result = NULL;

	XMLAttribute * a = first;
	while (a != NULL) {
		if (a->isName(target)) {
			result = a->getValue();
			break;
		}
		a = a->getNextAttribute();
	}
	
	return result;
}

bool XMLElement::getAttributeValue(const char * target, long & result) const {
	bool success = false;
	const char * val = getAttributeValue(target);
	if (val) {
		if (strlen(val)>0) {
			long temp;
			char * end;
			temp = strtol(val, &end, 0);
			if (end[0] == '\0') { 
				result = temp;
				success = true;
			}
	//		sscanf(val, "%ld", &temp);
		}
	}
	return success;
}

bool XMLElement::getAttributeValue(const char * target, float & result) const {
	bool success = false;
	const char * val = getAttributeValue(target);
	if (val) {
		if (strlen(val)>0) {
			float temp;
			char * end;
			temp = strtod(val, &end);
			if (end[0] == '\0') { 
				result = temp;
				success = true;
			}
			//		sscanf(val, "%f", &temp);
		}
	}
	return success;
}

bool XMLElement::getAttributeValue(const char * target, bool & result) const {
	bool success = false;
	const char * val = getAttributeValue(target);
	if (val) {
		if (strlen(val)>0) {
			if (!strcmp(val, trueXML)) {
				result = true;
				success = true;
			} else if (!strcmp(val, falseXML)) {
				result = false;
				success = true;
			}
		}
	}
	return success;
}

bool XMLElement::isName(const char * target) const {
	bool result = false;
	
	if (strcmp(name, target) == 0) result = true;
	
	return result;
}

bool XMLElement::getBoolValue(bool & b) const {
	bool result = false;
	if (value) {
		if (!strcasecmp(value, trueXML)) {
			result = true;
			b = true;
		} else if (!strcasecmp(value, falseXML)) {
			result = true;
			b = false;
		}
	}
	return result;
}
bool XMLElement::getDoubleValue(double & x) const {
	bool result = false;
	
	if (value) {
		char * test;
		double temp = strtod(value, &test);
		if ((test != value)&&(test[0]=='\0')) {
			x = temp;
			result = true;
		}
	}
	return result;
}
bool XMLElement::getLongValue(long & i) const {
	bool result = false;
	
	if (value) {
		char * test;
		long temp = strtol(value, &test, 0);
		if ((test != value)&&(test[0]=='\0')) {
			i = temp;
			result = true;
		}
	}
	return result;
}
#define kMaxCopyBuffer	63
long XMLElement::getFloatArray(const long & count, float * array) const {
	long readsofar=0;
	if ((array != NULL)&&(value!=NULL)) {
		long pos=0;
		int nchar;
		char line[kMaxCopyBuffer+1];
	// It appears that internally sscanf internally calls strlen. For very large arrays this results in
	// scanning the text string many times over a progressively smaller range, but still takes significant time.
	// Simply copying the next value in the space delimited string over to a buffer and parsing that buffer results
	// a major improvement in performance.
		for (; readsofar<count; readsofar++) {
			nchar = 0;
			int itemCount = 0;
			while ((value[pos] == ' ')&&(value[pos]!='\0')) pos++;
			while ((value[pos] != ' ')&&(value[pos]!='\0')&&(nchar<kMaxCopyBuffer)) {
				line[nchar] = value[pos];
				nchar++;
				pos++;
			}
			line[nchar] = '\0';
			if (nchar < kMaxCopyBuffer) {
				itemCount = sscanf(line, "%f",
								   &(array[readsofar]));
			} else {
		// The following should be equivelent but is slow for very large strings
		// Switch to this code if the copy didn't fit in the buffer for some reason
				pos -= nchar;
				itemCount = sscanf(&(value[pos]), "%f%n",
							   &(array[readsofar]), &nchar);
				pos += nchar;
			}
			if (itemCount != 1) {	//Just break and return what we have read in so far
				break;
			}
		}
	}
	return readsofar;
}
long XMLElement::getFloatArray(const long & count, std::vector<float> & array) const {
	long readsofar=0;
	if (value!=NULL) {
		long pos=0;
		int nchar;
		float temp;
		char line[kMaxCopyBuffer+1];
		for (; readsofar<count; readsofar++) {
			nchar = 0;
			int itemCount = 0;
			while ((value[pos] == ' ')&&(value[pos]!='\0')) pos++;
			while ((value[pos] != ' ')&&(value[pos]!='\0')&&(nchar<kMaxCopyBuffer)) {
				line[nchar] = value[pos];
				nchar++;
				pos++;
			}
			line[nchar] = '\0';
			if (nchar < kMaxCopyBuffer) {
				itemCount = sscanf(line, "%f",
								   &(array[readsofar]));
			} else {
				// The following should be equivelent but is slow for very large strings
				// Switch to this code if the copy didn't fit in the buffer for some reason
				pos -= nchar;
				itemCount = sscanf(&(value[pos]), "%f%n",
								   &temp, &nchar);
				pos += nchar;
			}
			if (itemCount != 1) {	//Just break and return what we have read in so far
				break;
			}
			array.push_back(temp);
		}
	}
	return readsofar;
}
long XMLElement::getLongArray(const long & count, long * array) const {
	long readsofar=0;
	if ((array != NULL)&&(value!=NULL)) {
		long pos=0;
		int nchar;
		char line[kMaxCopyBuffer+1];
		for (; readsofar<count; readsofar++) {
			nchar = 0;
			int itemCount = 0;
			while ((value[pos] == ' ')&&(value[pos]!='\0')) pos++;
			while ((value[pos] != ' ')&&(value[pos]!='\0')&&(nchar<kMaxCopyBuffer)) {
				line[nchar] = value[pos];
				nchar++;
				pos++;
			}
			line[nchar] = '\0';
			if (nchar < kMaxCopyBuffer) {
				itemCount = sscanf(line, "%ld", &(array[readsofar]));
			} else {
				// The following should be equivelent but is slow for very large strings
				// Switch to this code if the copy didn't fit in the buffer for some reason
				pos -= nchar;
				itemCount = sscanf(&(value[pos]), "%ld%n",
								   &(array[readsofar]), &nchar);
				pos += nchar;
			}
			if (itemCount != 1) {	//Just break and return what we have read in so far
				break;
			}
		}
	}
	return readsofar;
}

long XMLElement::getLongArray(const long & numExpected, std::vector<long> & array) {
	long readsofar=0;
	long temp;
	if (value!=NULL) {
		long pos=0;
		int nchar;
		char line[kMaxCopyBuffer+1];
		for (; readsofar<numExpected; readsofar++) {
			nchar = 0;
			int itemCount = 0;
			while ((value[pos] == ' ')&&(value[pos]!='\0')) pos++;
			while ((value[pos] != ' ')&&(value[pos]!='\0')&&(nchar<kMaxCopyBuffer)) {
				line[nchar] = value[pos];
				nchar++;
				pos++;
			}
			line[nchar] = '\0';
			if (nchar < kMaxCopyBuffer) {
				itemCount = sscanf(line, "%ld", &temp);
			} else {
				// The following should be equivelent but is slow for very large strings
				// Switch to this code if the copy didn't fit in the buffer for some reason
				pos -= nchar;
				itemCount = sscanf(&(value[pos]), "%ld%n",
								   &(array[readsofar]), &nchar);
				pos += nchar;
			}
			if (itemCount == 1) {
				array.push_back(temp);
			} else {	//Just break and return what we have read in so far
				break;
			}
		}
	}
	return readsofar;
}
void XMLElement::addAttribute(const char * n, const long & v) {
	std::ostringstream bf;
	bf << v;
	addAttribute(n, bf.str().c_str());
}
void XMLElement::addFloatAttribute(const char * n, const float & v) {
	std::ostringstream bf;
	bf << v;
	addAttribute(n, bf.str().c_str());
}
void XMLElement::addBoolAttribute(const char * n, bool value) {
	if (value)
		addAttribute(n, trueXML);
	else
		addAttribute(n, falseXML);
}
#define WXRECT_X_XML	"x"
#define WXRECT_Y_XML	"y"
#define WXRECT_WIDTH_XML	"width"
#define WXRECT_HEIGHT_XML	"height"
void XMLElement::addwxRectAttribute(const wxRect & v) {
	//Add the components of the wxRect as attributes if they are a non-default value
	if (v.x != -1) {
		std::ostringstream bf;
		bf << v.x;
		addAttribute(WXRECT_X_XML, bf.str().c_str());
	}
	if (v.y != -1) {
		std::ostringstream bf;
		bf << v.y;
		addAttribute(WXRECT_Y_XML, bf.str().c_str());
	}
	if (v.width != -1) {
		std::ostringstream bf;
		bf << v.width;
		addAttribute(WXRECT_WIDTH_XML, bf.str().c_str());
	}
	if (v.height != -1) {
		std::ostringstream bf;
		bf << v.height;
		addAttribute(WXRECT_HEIGHT_XML, bf.str().c_str());
	}
}
void XMLElement::getwxRectAttribute(wxRect & v) const {
	long temp;
	if (getAttributeValue(WXRECT_X_XML, temp)) {
		v.x = temp;
	}
	if (getAttributeValue(WXRECT_Y_XML, temp)) {
		v.y = temp;
	}
	if (getAttributeValue(WXRECT_WIDTH_XML, temp)) {
		if (temp > 0) v.width = temp;
	}
	if (getAttributeValue(WXRECT_HEIGHT_XML, temp)) {
		if (temp > 0) v.height = temp;
	}
}
void XMLElement::addAttribute(const char * n, const char * v) {
#ifdef UseXERCESC
	if (xElement != NULL) {
#if (XERCES_VERSION_MAJOR > 1)
		XMLCh * xname = XMLString::transcode(n);
		XMLCh * xvalue = XMLString::transcode(v);
		xElement->setAttribute(xname, xvalue);
#if (XERCES_VERSION_MINOR > 1)
		XMLString::release(&xname);
		XMLString::release(&xvalue);
#else
		delete xname;
		delete xvalue;
#endif
#else
		xElement.setAttribute(n, v);
#endif
	} else
#endif
	{
		XMLAttribute * a = new XMLAttribute(n, v);
		if (first == NULL) {
			first = last = a;
		} else {
			last->setNextAttribute(a);
			last = a;
		}
	}
}
void XMLElement::prependChild(XMLElement & child) {
	
	if (firstChild == NULL) 
	{
		firstChild = lastChild = &child;
	}
	else
	{
		child.setNextChild(*firstChild);
		firstChild = &child;
	}
	#ifdef UseXERCESC
	if (xElement != NULL) {	//prepend the element in the DOM tree
	XMLElement * oldFirst = child.getNextChild();
	#if (XERCES_VERSION_MAJOR > 1)
			DOMElement * c = child.getDOMElement();
			if(oldFirst != NULL)
			{
				DOMElement * oldFirstDOM = oldFirst->getDOMElement();
				xElement->insertBefore(c,oldFirstDOM);
			}
			else
			{
				xElement->appendChild(c);
			}
	#else
			const DOM_Element c = child.getDOMElement();
			if(oldFirst != NULL)
			{
				DOM_Element  oldFirstDOM = oldFirst->getDOMElement();
				xElement.insertBefore(c,oldFirstDOM);
			}
			else
			{
				xElement.appendChild(c);
			}
	#endif
	}
	#endif
}

	
void XMLElement::appendChild(XMLElement & child) {
	if (firstChild == NULL) {
		firstChild = lastChild = &child;
	} else {
		lastChild->setNextChild(child);
		lastChild = &child;
	}
#ifdef UseXERCESC
	if (xElement != NULL) {	//append the element in the DOM tree
#if (XERCES_VERSION_MAJOR > 1)
		DOMElement * c = child.getDOMElement();
		xElement->appendChild(c);
#else
		const DOM_Element c = child.getDOMElement();
		xElement.appendChild(c);
#endif
	}
#endif
}

XMLElementList * XMLElement::getChildren(void) {
	XMLElementList * result=NULL;
	int count = 0;
	XMLElement * f = firstChild;
	while (f != NULL) {
		count++;
		f = f->getNextChild();
	}
	XMLElement ** p = new XMLElement*[count];
	if (count>0) {
		f = firstChild;
		for (int i=0; i<count; i++) {
			p[i] = f;
			f = f->getNextChild();
		}
	}
	result = new XMLElementList(p, count);
	return result;
}

XMLElementList * XMLElement::getElementsByName(const char * name) const {
	XMLElementList * result=NULL;
	
	XMLElement * le = firstChild;
	int count = 0;
	while (le != NULL) {
		count++;
		le = le->getNextChild();
	}
	XMLElement ** list = new XMLElement*[count];
	if (count > 0) {
		le = firstChild;
		count = 0;
		while (le != NULL) {
			if (le->isName(name)) {
				list[count] = le;
				count++;
			}
			le = le->getNextChild();
		}
	}
	result = new XMLElementList(list, count);
	return result;
}

int XMLElement::getElementCount(const char * name) const {
	XMLElement * le = firstChild;
	int count = 0;
	le = firstChild;
	count = 0;
	while (le != NULL) {
		if (le->isName(name)) {
			count++;
		}
		le = le->getNextChild();
	}
	return count;
}

void XMLElement::ParseNamespaceTags(std::vector<std::string> & tagNames, std::vector<int> & tagIds) {
//	int numLocalTags = 0;	//keep track of ids defined in this element as
							//they must be removed before exiting
		//Search for namespace definition attributes first
}

void XMLElement::StripNamespaceTags(void) {
	::StripNamespaceTag(name);
	XMLAttribute * a = first;
	while (a != NULL) {
		a->StripNameSpaceTag();
		a = a->getNextAttribute();
	}
	XMLElement * le = firstChild;
	while (le != NULL) {
		le->StripNamespaceTags();
		le = le->getNextChild();
	}
}

std::ostream & operator << (std::ostream & target, const XMLElement & toWrite) {
#ifdef UseXERCESC
	if (toWrite.getDOMElement() != NULL) {
		target << toWrite.getDOMElement();
		return target;
	}
#endif
	target << "<" << toWrite.getName();
	XMLAttribute * a = toWrite.getFirstAttribute();
	while (a != NULL) {
		target << " " << *a;
		a = a->getNextAttribute();
	}
	if (toWrite.hasChildren()) {
		target << ">";
		//put in a line feed to make it a little more readable
		target << std::endl;

		XMLElement * e = toWrite.getFirstChild();
		while (e != NULL) {
			target << *e;
			e = e->getNextChild();
		}
		target << "</" << toWrite.getName() << ">";
//	} else if (toWrite.getValue() != NULL) {
	} else {	//canonicalization requires no short empty tags
//**** need to filter and escape special characters
//		target << ">" << toWrite.getValue()
//			<< "<" << toWrite.getValue() << "/>";
		target << ">";
		if (toWrite.getValue() != NULL) {
			const char * value = toWrite.getValue();
			int len = strlen(value);
			for (int i=0; i<len; i++) {
				switch (value[i]) {
					case '&':
						target << "&amp;";
					break;
					case '<':
						target << "&lt;";
					break;
					case '>':
						target << "&gt;";
					break;
					case 13: //0xD line feeds
						target << "&#xD;";
					break;
					default:
						target << value[i];
				}
			}
//			target << toWrite.getValue();
		}
		target << "</" << toWrite.getName() << ">";
//	} else{
//		target << "/>";
	}
		//put in a line feed to make it a little more readable
	target << std::endl;
	return target;
}

XMLElementList::XMLElementList(XMLElement ** list, const int & count) {
	first = list;
	numElements = count;
}

XMLElementList::~XMLElementList(void) {
	delete [] first;
}

XMLElement * XMLElementList::item(const int & index) const {
	XMLElement * result = NULL;
	if (index < numElements) {
		result = first[index];
	}
	return result;
}

//Call to strip off a leading namespace qualifier
//The change is made in place
void StripNamespaceTag(char * name) {
	int i=0;
	while (name[i]!=':') {
		if (name[i] == '\0') return;	//no namespace tag
		i++;
	}
	i++;
	int taglength = i;
	while (name[i]!='\0') {
		name[i-taglength] = name[i];
		i++;
	}
	name[i-taglength] = '\0';
}

