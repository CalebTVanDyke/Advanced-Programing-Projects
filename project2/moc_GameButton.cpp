/****************************************************************************
** Meta object code from reading C++ file 'GameButton.h'
**
** Created: Wed Nov 27 17:10:43 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GameButton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GameButton[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      36,   12,   11,   11, 0x0a,
      66,   12,   11,   11, 0x0a,
      96,   12,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GameButton[] = {
    "GameButton\0\0grid,gameBoard,cellsize\0"
    "play(QGridLayout*,board*,int)\0"
    "step(QGridLayout*,board*,int)\0"
    "restart(QGridLayout*,board*,int)\0"
};

void GameButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GameButton *_t = static_cast<GameButton *>(_o);
        switch (_id) {
        case 0: _t->play((*reinterpret_cast< QGridLayout*(*)>(_a[1])),(*reinterpret_cast< board*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->step((*reinterpret_cast< QGridLayout*(*)>(_a[1])),(*reinterpret_cast< board*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->restart((*reinterpret_cast< QGridLayout*(*)>(_a[1])),(*reinterpret_cast< board*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GameButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GameButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_GameButton,
      qt_meta_data_GameButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GameButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GameButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GameButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GameButton))
        return static_cast<void*>(const_cast< GameButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int GameButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
