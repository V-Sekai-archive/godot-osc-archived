#include "register_types.h"

#include "core/object/class_db.h"

#include "osc.h"

void register_osc_types() {
	ClassDB::register_class<OSC>();
}

void unregister_osc_types() {
}