/*
 *
 *  Copyright (C) 2003-2005, OFFIS
 *
 *  This software and supporting documentation were developed by
 *
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *
 *  Module: dcmsr
 *
 *  Author: Joerg Riesmeier
 *
 *  Purpose:
 *    classes: DSRXMLDocument
 *
 *  Last Update:      $Author$
 *  Update Date:      $Date$
 *  CVS/RCS Revision: $Revision$
 *  Status:           $State$
 *
 *  CVS/RCS Log at end of file
 *
 */


#ifndef DSRXMLD_H
#define DSRXMLD_H

#include "dcmtk/config/osconfig.h"   /* make sure OS specific configuration is included first */

#include "dcmtk/ofstd/ofstream.h"
#include "dcmtk/dcmsr/dsrtypes.h"
#include "dcmtk/dcmsr/dsrxmlc.h"


/*--------------------*
 *  type definitions  *
 *--------------------*/

#ifndef WITH_LIBXML
// define types if 'libxml' absent
typedef void (*xmlDocPtr);
typedef void (*xmlCharEncodingHandlerPtr);
typedef char xmlChar;
#endif


/*---------------------*
 *  class declaration  *
 *---------------------*/

/** Class for XML documents.
 *  This class encapsulates the access to the 'libxml' routines.  Since the
 *  public interface of this class does not use any 'libxml' specific data
 *  structures it should be possible to replace the XML library with little
 *  effort (if required).
 */
class DSRXMLDocument
  : protected DSRTypes
{

  public:

  // --- constructors and destructor ---

    /** default constructor
     */
    DSRXMLDocument();

    /** destructor
     */
    virtual ~DSRXMLDocument();


  // --- misc routines ---

    /** clear all internal member variables
     */
    void clear();

    /** check whether the current internal state is valid
     ** @return OFTrue if valid, OFFalse otherwise
     */
    OFBool valid() const;

    /** get the current log stream.
     *  See setLogStream() for details.
     *  @return pointer to the log stream (might be NULL = no messages)
     */
    OFConsole *getLogStream() const;

    /** set the log stream.
     *  The log stream is used to report warning and error messages.  Unfortunately, the
     *  stream cannot be used for 'libxml' messages.  Therefore, the error output of 'libxml'
     *  is disabled by default (see read() for details).
     ** @param  stream  pointer to the log stream (might be NULL = no messages)
     */
    void setLogStream(OFConsole *stream);


  // --- input and output ---

    /** read XML document from file.
     *  In order to enable the optional Schema validation the flag XF_validateSchema has to be set.
     *  XF_enableLibxmlErrorOutput enables the debug output from 'libxml'.
     ** @param  filename  name of the file from which the XML document is read ("-" for stdin)
     *  @param  flags     optional flag used to customize the reading process (see DSRTypes::XF_xxx)
     ** @return status, EC_Normal if successful, an error code otherwise
     */
    OFCondition read(const OFString &filename,
                     const size_t flags = 0);


  // --- character encoding ---

    /** check whether the currently set character encoding handler is valid.
     *  If no encoding handler is set this is equivalent to an invalid handler.
     ** @return OFTrue if handler is valid, OFFalse otherwise
     */
    OFBool encodingHandlerValid() const;

    /** set the specified character encoding handler.
     *  NB: 'libxml' relies on GNU 'libiconv' for most character sets.
     ** @param  charset  XML name of the character set (e.g. "ISO-8859-1" for ISO Latin-1)
     ** @return status, EC_Normal if successful, an error code otherwise
     */
    OFCondition setEncodingHandler(const char *charset);


  // --- navigation and checking ---

    /** get root node of the document
     ** @return cursor pointing to the root node if successful, invalid cursor otherwise
     */
    DSRXMLCursor getRootNode() const;

    /** get a particular named node of the document.
     *  Please note that the search process is limited to the current node level, i.e. no
     *  deep search is performed.
     ** @param  cursor    cursor pointing to the node where to start from
     *  @param  name      name of the node (XML element) to be searched for
     *  @param  required  flag specifying whether the node is required or not.  If the node
     *                    is required to be present an error message is reported to the
     *                    log stream in case it is not found.
     ** @return cursor pointing to the named node if successful, invalid cursor otherwise
     */
    DSRXMLCursor getNamedNode(const DSRXMLCursor &cursor,
                              const char *name,
                              const OFBool required = OFTrue) const;

    /** check whether particular node matches a given name
     ** @param  cursor  cursor pointing to the particular node
     *  @param  name    name of the node (XML element) to be checked
     ** @return OFTrue if name matches, OFFalse otherwise
     */
    OFBool matchNode(const DSRXMLCursor &cursor,
                     const char *name) const;

    /** check whether particular node matches a given name and report an error if not
     ** @param  cursor  cursor pointing to the particular node
     *  @param  name    name of the node (XML element) to be checked
     ** @return OFTrue if name matches, OFFalse otherwise
     */
    OFCondition checkNode(const DSRXMLCursor &cursor,
                          const char *name) const;


  // --- get attributes and node content ---

    /** check whether particular node has a specific attribute
     ** @param  cursor  cursor pointing to the particular node
     *  @param  name    name of the XML attribute to be checked
     ** @return OFTrue if attribute is present, OFFalse otherwise
     */
    OFBool hasAttribute(const DSRXMLCursor &cursor,
                        const char *name) const;

    /** get string value from particular XML attribute.
     *  The result variable 'stringValue' is automatically cleared at the beginning.
     ** @param  cursor       cursor pointing to the particular node
     *  @param  stringValue  reference to string object in which the value should be stored
     *  @param  name         name of the XML attribute to be retrieved
     *  @param  encoding     use encoding handler if OFTrue, ignore character set otherwise
     *  @param  required     flag specifying whether the attribute is required or not.  If the
     *                       attribute is required to be present an error message is reported
     *                       to the log stream in case it is not found.
     ** @return reference to string object (might be empty)
     */
    OFString &getStringFromAttribute(const DSRXMLCursor &cursor,
                                     OFString &stringValue,
                                     const char *name,
                                     const OFBool encoding = OFFalse,
                                     const OFBool required = OFTrue) const;

    /** get element value from particular XML attribute
     ** @param  cursor    cursor pointing to the particular node
     *  @param  delem     DICOM element in which the attribute value is stored
     *  @param  name      name of the XML attribute to be retrieved
     *  @param  encoding  use encoding handler if OFTrue, ignore character set otherwise
     *  @param  required  flag specifying whether the attribute is required or not.  If the
     *                    attribute is required to be present an error message is reported
     *                    to the log stream in case it is not found.
     ** @return status, EC_Normal if successful, an error code otherwise
     */
    OFCondition getElementFromAttribute(const DSRXMLCursor &cursor,
                                        DcmElement &delem,
                                        const char *name,
                                        const OFBool encoding = OFFalse,
                                        const OFBool required = OFTrue) const;

    /** get string value from particular XML element
     ** @param  cursor       cursor pointing to the particular node
     *  @param  stringValue  reference to string object in which the value should be stored
     *  @param  name         name of the XML element to be retrieved
     *  @param  encoding     use encoding handler if OFTrue, ignore character set otherwise
     *  @param  clearString  flag specifying whether to clear the 'stringValue' at first or not
     ** @return reference to string object (might be empty)
     */
    OFString &getStringFromNodeContent(const DSRXMLCursor &cursor,
                                       OFString &stringValue,
                                       const char *name = NULL,
                                       const OFBool encoding = OFFalse,
                                       const OFBool clearString = OFTrue) const;

    /** get element value from particular XML element
     ** @param  cursor    cursor pointing to the particular node
     *  @param  delem     DICOM element in which the element value is stored
     *  @param  name      name of the XML element to be retrieved
     *  @param  encoding  use encoding handler if OFTrue, ignore character set otherwise
     ** @return status, EC_Normal if successful, an error code otherwise
     */
    OFCondition getElementFromNodeContent(const DSRXMLCursor &cursor,
                                          DcmElement &delem,
                                          const char *name = NULL,
                                          const OFBool encoding = OFFalse) const;

    /** get value type from particular node.
     *  The value type is either stored as the element name or in the attribute "valType".
     *  Additionally, by-reference relationships are also supported (either by attribute
     *  "ref" being present or element named "reference").
     ** @param  cursor  cursor pointing to the particular node
     ** @return value type (incl. by-reference) if successful, VT_invalid/unknown otherwise
     */
    E_ValueType getValueTypeFromNode(const DSRXMLCursor &cursor) const;

    /** get relationship type from particular node.
     *  The relationship type is either stored in the element "relationship" or in the
     *  attribute "relType".
     ** @param  cursor  cursor pointing to the particular node
     ** @return relationship type if successful, RT_invalid/unknown otherwise
     */
    E_RelationshipType getRelationshipTypeFromNode(const DSRXMLCursor &cursor) const;


  // --- error/warning messages ---

    /** print warning message for unexpected node
     ** @param  cursor  cursor pointing to the unexpected node
     */
    void printUnexpectedNodeWarning(const DSRXMLCursor &cursor) const;

    /** print general node error message
     ** @param  cursor  cursor pointing to the unexpected node
     *  @param  result  status used to print details on the error (no message if EC_Normal)
     */
    void printGeneralNodeError(const DSRXMLCursor &cursor,
                               const OFCondition &result) const;

  protected:

    /** convert given string from 'libxml' format (UTF8) to current character set
     ** @param  fromString  character string to be converted
     *  @param  toString    reference to string object in which the result should be stored
     ** @return OFTrue if successful, OFFalse otherwise (e.g. no handler selected)
     */
    OFBool convertUtf8ToCharset(const xmlChar *fromString,
                                OFString &toString) const;

    /** print error message for missing attribute
     ** @param  cursor  cursor pointing to the relevant node
     *  @param  name    name of the XML attribute
     */
    void printMissingAttributeError(const DSRXMLCursor &cursor,
                                    const char *name) const;

  // --- static function ---

    /** get the full path (incl. all predecessors) to the current node.
     *  Returns "<invalid>" in case of an invalid 'cursor'.
     ** @param  cursor       cursor pointing to the relevant node
     *  @param  stringValue  reference to string object in which the result should be stored
     *  @param  omitCurrent  flag indicating whether to omit the current node or not
     */
    static OFString &getFullNodePath(const DSRXMLCursor &cursor,
                                     OFString &stringValue,
                                     const OFBool omitCurrent = OFFalse);


  private:

    /// pointer to the internal representation of the XML document (libxml)
    xmlDocPtr Document;
    /// pointer to the currently selected character encoding handler (libxml)
    xmlCharEncodingHandlerPtr EncodingHandler;

    /// output stream for error messages, NULL for no messages
    OFConsole *LogStream;

// --- declaration copy constructor and assignment operator

    DSRXMLDocument(const DSRXMLDocument &);
    DSRXMLDocument &operator=(const DSRXMLDocument &);
};


#endif


/*
 *  CVS/RCS Log:
 *  $Log$
 *  Revision 1.1  2007/04/24 09:53:28  braindead
 *  - updated DCMTK to version 3.5.4
 *  - merged Gianluca's WIN32 changes
 *
 *  Revision 1.1.1.1  2006/07/19 09:16:43  pipelka
 *  - imported dcmtk354 sources
 *
 *
 *  Revision 1.5  2005/12/08 16:05:36  meichel
 *  Changed include path schema for all DCMTK header files
 *
 *  Revision 1.4  2004/01/05 14:35:44  joergr
 *  Renamed XML attribute "ref_id" to "ref".
 *
 *  Revision 1.3  2003/12/01 15:46:18  joergr
 *  Changed XML encoding of by-reference relationships if flag
 *  XF_valueTypeAsAttribute is set.
 *
 *  Revision 1.2  2003/08/07 17:31:00  joergr
 *  Removed libxml dependency from header files. Simplifies linking (MSVC).
 *
 *  Revision 1.1  2003/08/07 12:16:37  joergr
 *  Added interface classes hiding the access to libxml (document and cursor
 *  class).
 *
 *
 */
