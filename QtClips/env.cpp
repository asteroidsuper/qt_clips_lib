#include "env.h"

#include "data_object.h"

extern "C" {

#include "clips.h"

}

#include <QString>
#include <QByteArray>

using namespace qclips;

namespace {
	static const char* OPEN_STRING_SOURCE_LOGICAL_NAME = u8"open_string_source_logical_name";
}

class Env::Impl
{
	void* _env;

public:
	Impl()
	{
		_env = CreateEnvironment();
	}

	~Impl()
	{
		DestroyEnvironment(_env);
	}

	DataObject eval(const QString& code)
	{
		DATA_OBJECT output;

		QByteArray utf8_code = code.toUtf8();

		auto result = EnvEval(_env, utf8_code.data(), &output);

		if (result)
		{
			return wrap(output);
		}

		return DataObject();
	}

	bool load(const QString& fileName)
	{
		QByteArray utf8_fileName = fileName.toUtf8();

		return EnvLoad(_env, utf8_fileName.constData()) > 0;
	}

	bool loadFromString(const QString& str)
	{
		QByteArray utf8_str = str.toUtf8();

		OpenStringSource(_env, OPEN_STRING_SOURCE_LOGICAL_NAME, utf8_str.constData(), 0);
		int result = LoadConstructsFromLogicalName(_env, OPEN_STRING_SOURCE_LOGICAL_NAME);
		CloseStringSource(_env, OPEN_STRING_SOURCE_LOGICAL_NAME);

		return result > 0;
	}

	void reset()
	{
		EnvReset(_env);
	}

	long long run(long long int num)
	{
		return EnvRun(_env, num);
	}

	DataObject getGlobalVarValue(const QString& name)
	{
		DATA_OBJECT output;

		QByteArray utf8_name = name.toUtf8();

		auto found = EnvGetDefglobalValue(_env, utf8_name.constData(), &output);

		if (found)
		{
			return wrap(output);
		}

		return DataObject();
	}

	void clear()
	{
		EnvClear(_env);
	}

private:
	DataObject wrap(DATA_OBJECT& object)
	{
		DataObject result;

		result.init(&object);

		return result;
	}
};

Env::Env() : _impl(new Impl)
{
}

Env::~Env()
{
	delete _impl;
}

DataObject Env::getGlobalVarValue(const QString& name)
{
	return _impl->getGlobalVarValue(name);
}

DataObject Env::eval(const QString& code)
{
	return _impl->eval(code);
}

bool Env::loadFromString(const QString & str)
{
	return _impl->loadFromString(str);
}

bool Env::load(const QString & fileName)
{
	return _impl->load(fileName);
}

long long Env::run(long long limit)
{
	return _impl->run(limit);
}

void Env::reset()
{
	_impl->reset();
}

void Env::clear()
{
	_impl->clear();
}
