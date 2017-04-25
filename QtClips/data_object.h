#pragma once

class QString;

namespace qclips
{
	enum class Type;

	class DataObject
	{
	public:
		DataObject();
		DataObject(const DataObject& other);
		DataObject(DataObject&& other);
		~DataObject();

		Type type() const;
		QString string() const;
		double floating() const;
		long long integer() const;

		bool isNull() const;

		DataObject& operator =(const DataObject& other);
		DataObject& operator =(DataObject&& other);

		operator bool() const;

	private:
		class Impl;
		Impl *_impl;

		void init(void*);

		friend class Env;
	};
}