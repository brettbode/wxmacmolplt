/*
 *  (c) 2003 Iowa State University
 *      see the LICENSE file in the top level directory
 */

// XML.hpp
//
//	Custom XML building and parsing classes that can be used either to wrapper
//	other XML libraries or as a standalone parsing library
//
// Select type of XML parser (custom, Xerces v1 or 2)
// define "USEXERCESC" to activate the use of Xerces internally
// The code will automatically detect the use of Xerces v1.x or v2.x

#ifndef __XML__
#define __XML__

//#define UseXERCESC 1
#include <iostream>

#ifdef UseXERCESC
#include <xercesc/util/XercesDefs.hpp>
#if (XERCES_VERSION_MAJOR > 1)
//xercesc namespace was introduced in 2.2
#if ((XERCES_VERSION_MAJOR == 2) && (XERCES_VERSION_MINOR > 1))
XERCES_CPP_NAMESPACE_USE
#endif

#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#else
#include <xercesc/dom/DOM.hpp>
#endif
#include <xercesc/framework/MemBufInputSource.hpp>
#endif

#define trueXML	"true"
#define falseXML "false"

//Global library setup and teardown routines

bool XMLSetup(void);
void XMLShutdown(void);

//Exception thrown in the custom XML parsing routines
class XMLParseError {
private:
//	char *	ErrorMessage;
public:
	XMLParseError(void) {};
//	XMLParseError(char * foo) {ErrorMessage = foo;};
};

//Call to strip off a leading namespace qualifier
//The change is made in place
void StripNamespaceTag(char * name);

class XMLAttribute {
	private:
		char * name;
		char * value;
		int		namespaceID;
		
		XMLAttribute * next;
	public:
		XMLAttribute(void);
		XMLAttribute(const char * name, const char * value);
		~XMLAttribute(void);
		
		const char * getName(void) const {return name;};
		const char * getValue(void) const {return value;};
		void setName(char * name);
		void setValue(char * value);
		bool isName(const char * name) const;
		XMLAttribute * getNextAttribute(void) const {return next;};
		void setNextAttribute(XMLAttribute * n);
		void StripNameSpaceTag(void) {::StripNamespaceTag(name);};
};

std::ostream & operator << (std::ostream & target, const XMLAttribute & toWrite);
class XMLElement;
class wxRect;

class XMLDocument {
private:
	XMLElement *	root;
	const char *	Buffer;
	char *			uri;
	long			bufferLength;
	bool			useCustom;
	
	void clear(void);
#ifdef UseXERCESC
	MemBufInputSource *	lSrc;
	friend std::ostream & operator << (std::ostream & target, const XMLDocument & toWrite);

	friend std::ostream& operator<<(std::ostream& target, DOMNode * toWrite);
	
	DOMDocument * 			doc;
	XercesDOMParser	*	parser;
	DOMDocument * getDOMDoc() const { return doc;};
#endif
public:
	XMLDocument(void);
	XMLDocument(const char * rootName, bool custom, const char * uri=NULL);
	//the XMLDocument assumes ownership of the MemBuffer and will delete it.
	XMLDocument(const char * MemBuffer, const long & bufferLength, bool custom);
	~XMLDocument(void);
	
	XMLElement * getDocumentRoot(void) const;
	bool Custom(void) const {return useCustom;};
	
	bool parse(void);
};

std::ostream & operator << (std::ostream & target, const XMLDocument & toWrite);

class XMLElementList;

class XMLElement {
	private:
		char * 			name;
		char * 			value;
		XMLAttribute *	first;
		XMLAttribute *	last;
		XMLElement *	firstChild;
		XMLElement *	lastChild;
		XMLElement *	nextChild;
		int				namespaceID;

#ifdef UseXERCESC
		friend XMLDocument::XMLDocument(const char * rootName,  bool custom, const char * uriReq);
		friend std::ostream & operator << (std::ostream & target, const XMLElement & toWrite);

		DOMElement * 	xElement;
		DOMDocument *	doc;	//reference to create child elements

		XMLElement(DOMElement * target, DOMDocument * d);
		XMLElement(DOMDocument * d, const char * name, const char * value=NULL);
		DOMElement * getDOMElement(void) const {return xElement;};
#endif
		void Initialize(void);
		friend bool XMLDocument::parse(void);
		XMLElement(const char * buffer, int & bytesConsumed, int recursionDepth);
	public:
		XMLElement(void);
		XMLElement(const char * name, const char * value=NULL);
		~XMLElement(void);
		
		const char * getName(void) const {return name;};
		const char * getValue(void) const {return value;};
		bool getBoolValue(bool & b) const;
		bool getDoubleValue(double & x) const;
		bool getLongValue(long & i) const;
		long getFloatArray(const long & count, float * array) const;
		long getFloatArray(const long & count, std::vector<float> & array) const;
		long getLongArray(const long & count, long * array) const;
		/** Read an array of long values into a vector.
		 * The array is assumed to be space separated.
		 * The return value is the number of items read in and stored in the vector.
		 * All items are appended to the vector.
		 * @param numExpected How many values are expected, at most this many will be read.
		 * @param array The vector to which the values will be appended.
		 */
		long getLongArray(const long & numExpected, std::vector<long> & array);
		const char * getAttributeValue(const char * name) const;
		bool getAttributeValue(const char * name, long & newvalue) const;
		bool getAttributeValue(const char * name, float & newvalue) const;
		bool getAttributeValue(const char * name, bool & result) const;
		void getwxRectAttribute(wxRect & v) const;
		XMLElementList * getChildren(void);
		XMLElementList * getElementsByName(const char * name) const;
		bool isName(const char * name) const;
		inline XMLElement * getNextChild(void) const {return nextChild;};
		inline XMLElement * getFirstChild(void) const {return firstChild;};
		XMLAttribute * getFirstAttribute(void) const {return first;};
		bool hasChildren(void) const {return ((firstChild != NULL)?true:false);};
		void addAttribute(const char * name, const char * value);
		void addAttribute(const char * name, const long & value);
		void addFloatAttribute(const char * name, const float & value);
		void addBoolAttribute(const char * name, bool value);
		void addwxRectAttribute(const wxRect & v);
		void appendChild(XMLElement & child);
		void prependChild(XMLElement & child);
		void setNextChild(XMLElement & n) {nextChild = &n;};
		XMLElement * addChildElement(const char * name, const char * value=NULL);
		XMLElement * addChildElementToFront(const char * name, const char * value=NULL);
		/**
		 * Add a child element from a bool value
		 * @param name The name of the new element
		 * @param value The bool value to write as the text content of the element
		 */
		XMLElement * addBoolChildElement(const char * name, bool value);
		/** Create an array element of long values from the provided vector.
		 * Produces an element of the form:
		 * \<name title="title" size="count">1 2 3 4\</name>
		 * @param array The vector containing the values.
		 * @param name The name of the new element
		 * @param title The title attribute for the element
		 * @param count How many values should be output, if ommitted the entire vector.
		 */
		XMLElement * AddLongArray(const std::vector<long> & array, const char * name, const char * title, long count=-1);
		int getElementCount(const char * elementName) const;
		void ParseNamespaceTags(std::vector<std::string> & tagNames, std::vector<int> & tagIds);
		void StripNamespaceTags(void);
};

std::ostream & operator << (std::ostream & target, const XMLElement & toWrite);

class XMLElementList {
	private:
		XMLElement **	first;
		int				numElements;

	public:
		XMLElementList(XMLElement ** list, const int & count);
		~XMLElementList(void);
		
		XMLElement * item(const int & index) const;
		int length(void) const {return numElements;};
};

#endif
