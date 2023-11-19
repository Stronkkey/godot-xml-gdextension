#ifndef XML_HELPER_LIB
#define XML_HELPER_LIB

#include "xml_document.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/xml_parser.hpp>

namespace godot {

class XMLReader : public RefCounted {
  GDCLASS(XMLReader, RefCounted);

private:
  static void attach_node_data(XMLNode *node, Ref<XMLParser> parser);
  static void parser_read(Ref<XMLParser> parser, TypedArray<XMLNode> &queue, XMLDocument *document);
  static Dictionary get_attributes(Ref<XMLParser> parser);
  static Ref<XMLDocument> parse(const PackedByteArray &xml_buffer);

  static XMLNode *create_node(TypedArray<XMLNode> &queue, Ref<XMLParser> parser);
  static XMLNode *create_node_element(Ref<XMLParser> parser);
  static XMLNode *create_node_element_end(Ref<XMLParser> parser);

  static PackedByteArray cleanup_double_blankets(const PackedByteArray &xml_buffer);

protected:
  static void _bind_methods();

public:
  XMLReader();
  ~XMLReader();

  static Ref<XMLDocument> parse_file(const String &path);
  static Ref<XMLDocument> parse_string(const String &str);
  static Ref<XMLDocument> parse_buffer(const PackedByteArray &buffer);
};
}

#endif // !XML_HELPER_LIB
