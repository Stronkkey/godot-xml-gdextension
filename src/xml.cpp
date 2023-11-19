#include "xml.hpp"
#include "godot_cpp/classes/image.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void XMLReader::_bind_methods() {
  ClassDB::bind_static_method("XMLReader", D_METHOD("parse_buffer", "buffer"), &XMLReader::parse_buffer);
  ClassDB::bind_static_method("XMLReader", D_METHOD("parse_file", "file_path"), &XMLReader::parse_file);
  ClassDB::bind_static_method("XMLReader", D_METHOD("parse_string", "string"), &XMLReader::parse_string);
}

XMLReader::XMLReader() {
}

XMLReader::~XMLReader() {
}

PackedByteArray XMLReader::cleanup_double_blankets(const PackedByteArray &xml_buffer) {
  PackedByteArray clean_buffer = {};
  PackedByteArray unsafe_escapes;
  unsafe_escapes.append(9);
  unsafe_escapes.append(10);
  unsafe_escapes.append(13);

  for (uint8_t byte: xml_buffer) {
    if (!unsafe_escapes.has(byte)) {
      clean_buffer.append(byte);
    }
  }

  return clean_buffer;
}

XMLNode *XMLReader::create_node_element(Ref<XMLParser> parser) {
  XMLNode *new_node = memnew(XMLNode);

  new_node->set_name(parser->get_node_name());
  new_node->set_attributes(get_attributes(parser));
  new_node->set_standalone(parser->is_empty());

  return new_node;
}

XMLNode *XMLReader::create_node_element_end(Ref<XMLParser> parser) {
  XMLNode *new_node = memnew(XMLNode);

  new_node->set_name(parser->get_node_name());

  return new_node;
}

void XMLReader::attach_node_data(XMLNode *node, Ref<XMLParser> parser) {
  if (node->get_content().is_empty())
    node->set_content(parser->get_node_data().strip_edges().lstrip(" ").rstrip(" "));
}

Dictionary XMLReader::get_attributes(Ref<XMLParser> parser) {
  Dictionary attributes;
  int32_t attribute_count = parser->get_attribute_count();

  for (int i = 0; i < attribute_count; i++) {
    attributes[parser->get_attribute_name(i)] = parser->get_attribute_value(i);
  }

  return attributes;
}

XMLNode *XMLReader::create_node(TypedArray<XMLNode> &queue, Ref<XMLParser> parser) {
  XMLParser::NodeType node_type = parser->get_node_type();

  switch (node_type) {
    case XMLParser::NODE_ELEMENT:
      return create_node_element(parser);
      break;
    case XMLParser::NODE_ELEMENT_END:
      return create_node_element_end(parser);
      break;
    case XMLParser::NODE_TEXT:
      {
      XMLNode *back_node = Object::cast_to<XMLNode>(queue.back());

      if (!queue.is_empty())
        attach_node_data(back_node, parser);
      break;
      }
    default:
      break;
  }

  return memnew(XMLNode);
}

void XMLReader::parser_read(Ref<XMLParser> parser, TypedArray<XMLNode> &queue, XMLDocument *document) {
  XMLNode *node = create_node(queue, parser);

  if (node->is_empty()) {
    memdelete(node);
    return;
  }

  if (queue.is_empty()) {
    document->set_root(node);
    queue.append(node);
    return;
  }

  XMLParser::NodeType node_type = parser->get_node_type();
  XMLNode *last_node = Object::cast_to<XMLNode>(queue.back());

  if (node->get_standalone()) {
    last_node->get_children().append(node);
  } else if (node_type == XMLParser::NODE_ELEMENT_END) {
    queue.pop_back();
  } else {
    last_node->get_children().append(node);
    queue.append(node);
  }
}

Ref<XMLDocument> XMLReader::parse(const PackedByteArray &buffer) {
  PackedByteArray new_buffer = cleanup_double_blankets(buffer);

  XMLDocument *document = memnew(XMLDocument);
  Ref<XMLParser> parser = Ref<XMLParser>(memnew(XMLParser));
  TypedArray<XMLNode> queue;

  Error error_message = parser->open_buffer(new_buffer);

  if (error_message != OK) {
    ERR_PRINT("Couldn't read buffer.");
  }

  while (parser->read() == OK) {
    parser_read(parser, queue, document);
  }

  if (!queue.is_empty()) {
    queue.reverse();
    TypedArray<String> names;

    for (int i = 0; i < queue.size(); i++) {
      Ref<XMLNode> node = queue[i];
      if (node.is_valid())
        names.append(node->get_name());
    }
    queue.clear();
  }

  return Ref<XMLDocument>(document);
}

Ref<XMLDocument> XMLReader::parse_buffer(const PackedByteArray &buffer) {
  return parse(buffer);
}


Ref<XMLDocument> XMLReader::parse_file(const String &path) {
  Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);

  if (file == nullptr) {
    ERR_PRINT(vformat("Couldn't open file \"%s\". File invalid or not readable.", path));
    return Ref<XMLDocument>(memnew(XMLDocument));
  }

  return parse(file->get_as_text().to_utf8_buffer());
}


Ref<XMLDocument> XMLReader::parse_string(const String &str) {
  return parse(str.to_utf8_buffer());
}
