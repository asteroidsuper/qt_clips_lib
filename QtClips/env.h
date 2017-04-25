#pragma once

class QString;

namespace qclips
{
	class DataObject;

	class Env
	{
	public:
		Env();
		Env(Env&) = delete;
		~Env();

		DataObject getGlobalVarValue(const QString& name);

		DataObject eval(const QString& code);

		bool loadFromString(const QString& str);
		bool load(const QString& fileName);

		long long run(long long limit);
		void reset();
		void clear();

		Env& operator = (Env&) = delete;

	private:
		class Impl;
		Impl* _impl;
	};
}