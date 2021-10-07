/*
 * See the LICENSE file at the root directory for license information
 */

/*
 * File:   oscmsg.cpp
 * Author: frankiezafe
 *
 * Created on December 10, 2018, 5:52 PM
 */

#include "oscmsg.h"

void oscmsg::_register_methods() {
    register_method("buffer_size", &oscmsg::buffer_size);
    register_method("address", &oscmsg::address);
    register_method("add", &oscmsg::add);
    register_method("close", &oscmsg::close);
    register_method("data", &oscmsg::data);
    register_method("size", &oscmsg::size);
    register_method("ready", &oscmsg::ready);
    register_method("closed", &oscmsg::closed);
    register_method("is_oscmsg", &oscmsg::is_oscmsg);
}

oscmsg::oscmsg() :
_buffer_size(1024),
_buffer(0), _packet(0),
_packet_closed(false) {

    std::cout << this << std::endl;
    std::cout << this->owner << std::endl;

}

oscmsg::~oscmsg() {
}

void oscmsg::reset() {

    if (_packet) {
        delete _packet;
        _packet = 0;
        _packet_closed = false;
    }
    if (_buffer) {
        delete[] _buffer;
        _buffer = 0;
    }

}

void oscmsg::buffer_size(int size) {

    if (size <= 0) {
        print_line("oscmsg::buffer_size, wrong buffer_size!");
        return;
    }

    reset();

    _buffer_size = size;

}

void oscmsg::address(String address) {

    reset();

    std::wstring ws = address.unicode_str();
    std::string std_address(ws.begin(), ws.end());

    _buffer = new char[_buffer_size];
    _packet = new osc::OutboundPacketStream(_buffer, _buffer_size);
    (*_packet) << osc::BeginBundleImmediate
            << osc::BeginMessage(std_address.c_str());

}

void oscmsg::add(Variant var) {

    if (!_packet) {
        print_line("oscmsg::add, message is not ready! "
                "call 'address' first!");
        return;
    }

    switch (var.get_type()) {
        case Variant::Type::BOOL:
        {
            bool b = var;
            (*_packet) << b;
        }
            break;
        case Variant::Type::STRING:
        case Variant::Type::NODE_PATH:
        case Variant::Type::_RID:
        case Variant::Type::OBJECT:
        {
            String s = var;
            (*_packet) << s.alloc_c_string();
        }
            break;
        case Variant::Type::INT:
        {
            int i = var;
            (*_packet) << i;
        }
            break;
        case Variant::Type::REAL:
        {
            float f = var;
            (*_packet) << f;
        }
            break;
        case Variant::Type::VECTOR2:
        {
            Vector2 v = var;
            (*_packet) << float( v.x);
            (*_packet) << float( v.y);
        }
            break;
        case Variant::Type::VECTOR3:
        {
            Vector3 v = var;
            (*_packet) << float( v.x);
            (*_packet) << float( v.y);
            (*_packet) << float( v.z);
        }
            break;
        case Variant::Type::QUAT:
        {
            Quat q = var;
            (*_packet) << float( q.x);
            (*_packet) << float( q.y);
            (*_packet) << float( q.z);
            (*_packet) << float( q.w);
        }
            break;
        case Variant::Type::COLOR:
        {
            Color c = var;
            (*_packet) << float( c.r);
            (*_packet) << float( c.g);
            (*_packet) << float( c.b);
            (*_packet) << float( c.a);
        }
            break;
        default:
            print_line("oscsender::add, unable to add "
                    "this kind of data!");
            break;

    }

}

void oscmsg::close() {

    if (!_packet_closed) {

        (*_packet) << osc::EndMessage << osc::EndBundle;

        _array.resize(0);
        for ( std::size_t i = 0; i < _buffer_size; ++i ) {
            _array.push_back( (unsigned char) _packet->Data()[i] );
        }

        _packet_closed = true;

    }

}
