#include "data_object.h"
#include "clips_types.h"

#include "clips.h"

#include <QtGlobal>
#include <QString>

#include <string>

using namespace qclips;

namespace {
	Type intToClipsType(int type)
	{
		switch (type)
		{
		case STRING: return Type::String;

		case SYMBOL: return Type::Symbol;

		case FLOAT: return Type::Float;

		case INTEGER: return Type::Integer;

		default: return Type::Unknow;
		}
	}

	Type typeOfClipsObject(DATA_OBJECT& clipsObject)
	{
		return intToClipsType(GetType(clipsObject));
	}

	QString stringFromClipsObject(DATA_OBJECT& object)
	{
		const char* string = DOToString(object);

		return QString::fromUtf8(string);
	}
}



class DataObject::Impl
{
private:
	QString _string;
	Type _type;
	qreal _float = 0;
	long long _int = 0;

public:
	Impl(DATA_OBJECT& object)
		: _type(typeOfClipsObject(object))
	{
		switch (_type)
		{
		case Type::String:
		{
			_string = stringFromClipsObject(object);
			_float = _string.toDouble();
			_int = _float;
		}

		case Type::Symbol:
		{
			_string = stringFromClipsObject(object);
			_float = _string.toDouble();
			_int = _float;
		}
		case Type::Float:
		{
			_float = DOToDouble(object);
			_int = _float;
		}
		case qclips::Type::Integer:
		{
			_int = DOToLong(object);
			_float = _int;
		}
		case qclips::Type::Unknow:
			break;
		default:
			break;
		}
	}

	Type type() const
	{
		return _type;
	}

	long long integer() const
	{
		return _int;
	}

	qreal floating() const
	{
		return _float;
	}

	QString string() const
	{
		switch (_type)
		{
		case Type::Symbol: return _string;

		case Type::Float: return QString::number(_float);

		case Type::Integer: return QString::number(_int);

		default: return _string;
		}
	}
};

DataObject::DataObject() : _impl(nullptr)
{
}

DataObject::DataObject(const DataObject & other)
{
	_impl = new Impl(*other._impl);
}

DataObject::DataObject(DataObject && other)
{
	_impl = other._impl;

	other._impl = nullptr;
}

DataObject::~DataObject()
{
	delete _impl;
}

Type DataObject::type() const
{
	return _impl->type();
}

QString DataObject::string() const
{
	return _impl->string();
}

double DataObject::floating() const
{
	return _impl->floating();
}

long long DataObject::integer() const
{
	return _impl->integer();
}

bool DataObject::isNull() const
{
	return this->_impl == nullptr;
}

DataObject& DataObject::operator=(const DataObject & other)
{
	if (this == &other)
	{
		return *this;
	}

	delete _impl;

	_impl = new Impl(*other._impl);

	return *this;
}

DataObject & DataObject::operator=(DataObject && other)
{
	delete _impl;

	_impl = other._impl;

	other._impl = nullptr;

	return *this;
}

DataObject::operator bool() const
{
	return _impl != nullptr;
}

void DataObject::init(void *object)
{
	delete _impl;

	_impl = new Impl(*(DATA_OBJECT*)object);
}
