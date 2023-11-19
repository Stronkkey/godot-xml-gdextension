#include "trail_component.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void TrailComponent::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_clean_points", "enabled"), &TrailComponent::set_clean_points);
  ClassDB::bind_method(D_METHOD("get_clean_points"), &TrailComponent::get_clean_points);

  ClassDB::bind_method(D_METHOD("get_process_callback"), &TrailComponent::get_process_callback);
  ClassDB::bind_method(D_METHOD("set_process_callback", "new_process_callback"), &TrailComponent::set_process_callback);

  ClassDB::bind_method(D_METHOD("get_follow_node"), &TrailComponent::get_follow_node);
  ClassDB::bind_method(D_METHOD("set_follow_node", "new_node"), &TrailComponent::set_follow_node);

  ClassDB::bind_method(D_METHOD("is_optimized"), &TrailComponent::is_optimized);
  ClassDB::bind_method(D_METHOD("set_optimized", "optimized"), &TrailComponent::set_optimized);

  ClassDB::bind_method(D_METHOD("get_max_points"), &TrailComponent::get_max_points);
  ClassDB::bind_method(D_METHOD("set_max_points"), &TrailComponent::set_max_points);

  ClassDB::add_property("TrailComponent", PropertyInfo(Variant::BOOL, "clean_points"), "set_clean_points", "get_clean_points");
  ClassDB::add_property("TrailComponent", PropertyInfo(Variant::OBJECT, "follow_node_path", PROPERTY_HINT_NODE_TYPE, "Node2D"), "set_follow_node", "get_follow_node");
  ClassDB::add_property("TrailComponent", PropertyInfo(Variant::INT, "process_callback", PROPERTY_HINT_ENUM, "Physics,Idle,None"), "set_process_callback", "get_process_callback");
  ClassDB::add_property("TrailComponent", PropertyInfo(Variant::BOOL, "optimized"), "set_optimized", "is_optimized");
  ClassDB::add_property("TrailComponent", PropertyInfo(Variant::INT, "max_points"), "set_max_points", "get_max_points");
}

TrailComponent::TrailComponent() {
  clean_points = true;
  optimize = true;
  max_points = -1;
  process_callback = TRAIL_PROCESS_IDLE;
  follow_node = this;
  previous_velocity = Vector2();
  previous_position = Vector2();
}

TrailComponent::~TrailComponent() {}

void TrailComponent::process_line() {
  if (!follow_node)
    return;

  Vector2 velocity = follow_node->get_position() - previous_position;
  if (previous_velocity.is_equal_approx(velocity) && optimize)
    set_point_position(get_point_count() - 1, follow_node->get_position());
  else if (optimize)
    add_point(follow_node->get_position());

  if (!optimize) {
    add_point(follow_node->get_position());
    if (get_point_count() == max_points)
      remove_point(1);
  }

  previous_velocity = velocity;
  previous_position = follow_node->get_position();
}

void TrailComponent::ready(){
  if (clean_points) {}
    clear_points();
  add_point(Vector2(0, 0));
}

void TrailComponent::process(double delta) {
  set_global_position(Vector2());
  if (process_callback == TRAIL_PROCESS_IDLE)
    process_line();
}

void TrailComponent::physics_process(double delta) {
  if (process_callback == TRAIL_PROCESS_PHYSICS)
    process_line();
}

void TrailComponent::_notification(int p_what) {
  switch (p_what) {

    case NOTIFICATION_PROCESS:
      process(get_process_delta_time());

    case NOTIFICATION_PHYSICS_PROCESS:
      physics_process(get_physics_process_delta_time());

  }
}

void TrailComponent::set_clean_points(const bool enabled) {
  clean_points = enabled;
}

bool TrailComponent::get_clean_points() const {
  return clean_points;
}

void TrailComponent::set_process_callback(const int new_process_callback) {
  switch (new_process_callback) {
    case 1:
      process_callback = TRAIL_PROCESS_IDLE;
    case 0:
      process_callback = TRAIL_PROCESS_PHYSICS;
    default:
      process_callback = TRAIL_PROCESS_NONE;
  }
}

int TrailComponent::get_process_callback() const {
  return process_callback;
}

void TrailComponent::set_follow_node(Node2D *new_node) {
  if (new_node != nullptr)
    follow_node = new_node;
  follow_node = this;
}

Node2D *TrailComponent::get_follow_node() {
  if (follow_node != nullptr)
    return follow_node;
  return this;
}

void TrailComponent::set_optimized(const bool optimized) {
  optimize = optimized;
}

bool TrailComponent::is_optimized() const {
  return optimize;
}

void TrailComponent::set_max_points(const int new_max_points) {
  max_points = new_max_points;
  if (get_points().size() > new_max_points && max_points < 0) {
    PackedVector2Array points = get_points();
    points.resize(Math::clamp(new_max_points, 0, new_max_points));
    set_points(points);
  }
}

int TrailComponent::get_max_points() const {
  return max_points;
}
