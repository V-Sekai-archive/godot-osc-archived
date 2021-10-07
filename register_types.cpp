#include "register_types.h"

#include "core/object/class_db.h"

#include "oscmsg.h"

void register_osc_types() {
	ClassDB::register_class<oscmsg>();
	// ClassDB::register_class<oscsender>();
	// ClassDB::register_class<oscreceiver>();
}

void unregister_osc_types() {
}