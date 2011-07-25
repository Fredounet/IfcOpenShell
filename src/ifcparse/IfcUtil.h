/********************************************************************************
 *                                                                              *
 * This file is part of IfcOpenShell.                                           *
 *                                                                              *
 * IfcOpenShell is free software: you can redistribute it and/or modify         *
 * it under the terms of the Lesser GNU General Public License as published by  *
 * the Free Software Foundation, either version 3.0 of the License, or          *
 * (at your option) any later version.                                          *
 *                                                                              *
 * IfcOpenShell is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * Lesser GNU General Public License for more details.                          *
 *                                                                              *
 * You should have received a copy of the Lesser GNU General Public License     *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                              *
 ********************************************************************************/

#ifndef IFCUTIL_H
#define IFCUTIL_H

#include <string>
#include <vector>

#include "../ifcparse/SharedPointer.h"
#include "../ifcparse/Ifc2x3enum.h"

class IfcAbstractEntity;
typedef SHARED_PTR<IfcAbstractEntity> IfcAbstractEntityPtr;

class IfcEntityList;
typedef SHARED_PTR<IfcEntityList> IfcEntities;

template <class F, class T>
inline SHARED_PTR<T> reinterpret_pointer_cast(SHARED_PTR<F> from) {
	SHARED_PTR<void> v = std::tr1::static_pointer_cast<void,F>(from);
	return std::tr1::static_pointer_cast<T,void>(v);
}

namespace IfcUtil {

class IfcBaseClass {
public:
    IfcAbstractEntityPtr entity;
    virtual bool is(Ifc2x3::Type::Enum v) = 0;
    virtual Ifc2x3::Type::Enum type() = 0;
};

typedef SHARED_PTR<IfcBaseClass> IfcSchemaEntity;

}

class IfcEntityList {
	std::vector<IfcUtil::IfcSchemaEntity> ls;
public:
	typedef std::vector<IfcUtil::IfcSchemaEntity>::const_iterator it;
	IfcEntities getInverse(Ifc2x3::Type::Enum c = Ifc2x3::Type::ALL);
	IfcEntities getInverse(Ifc2x3::Type::Enum c, int i, const std::string& a);
	void push(IfcUtil::IfcSchemaEntity l);
	void push(IfcEntities l);
	it begin();
	it end();
	IfcUtil::IfcSchemaEntity operator[] (int i);
	int Size() const;
};

template <class T>
class IfcTemplatedEntityList {
	std::vector<SHARED_PTR<T> > ls;
public:
	typedef typename std::vector<SHARED_PTR<T> >::const_iterator it;
	inline void push(SHARED_PTR<T> t) {if (t) ls.push_back(t);}
	inline void push(SHARED_PTR< IfcTemplatedEntityList<T> > t) { for ( typename T::it it = t->begin(); it != t->end(); ++it ) push(*it); }
	inline it begin() { return ls.begin(); }
	inline it end() { return ls.end(); }
	inline T operator[] (int i) { return ls[i]; }
	inline unsigned int Size() const { return (unsigned int) ls.size(); }
};

class Argument;
typedef SHARED_PTR<Argument> ArgumentPtr;

namespace IfcUtil {
	class IfcAbstractSelect : public IfcBaseClass {
	public:
		typedef SHARED_PTR< IfcTemplatedEntityList<IfcAbstractSelect> > list;
		typedef IfcTemplatedEntityList<IfcAbstractSelect>::it it;
		typedef SHARED_PTR<IfcAbstractSelect> ptr;
		virtual bool isSimpleType() = 0;
	};
	class IfcEntitySelect : public IfcAbstractSelect {
	public:
		typedef SHARED_PTR<IfcEntitySelect> ptr;
		IfcEntitySelect(SHARED_PTR<IfcUtil::IfcBaseClass> b);
		IfcEntitySelect(IfcAbstractEntityPtr e);
		bool is(Ifc2x3::Type::Enum v);
		Ifc2x3::Type::Enum type();
		bool isSimpleType();
	};
	class IfcArgumentSelect : public IfcAbstractSelect {
		Ifc2x3::Type::Enum _type;
		ArgumentPtr arg;
	public:
		typedef SHARED_PTR<IfcArgumentSelect> ptr;
		IfcArgumentSelect(Ifc2x3::Type::Enum t, ArgumentPtr a);
		ArgumentPtr wrappedValue();
		bool is(Ifc2x3::Type::Enum v);
		Ifc2x3::Type::Enum type();
		bool isSimpleType();
	};
}

class Argument {
protected:
public:
	virtual operator int() const = 0;
	virtual operator bool() const = 0;
	virtual operator float() const = 0;
	virtual operator std::string() const = 0;
	virtual operator std::vector<float>() const = 0;
	virtual operator std::vector<int>() const = 0;
	virtual operator std::vector<std::string>() const = 0;
	virtual operator IfcUtil::IfcSchemaEntity() const = 0;
	virtual operator SHARED_PTR<IfcUtil::IfcAbstractSelect>() const = 0;
	virtual operator IfcEntities() const = 0;
	virtual unsigned int Size() const = 0;
	virtual ArgumentPtr operator [] (unsigned int i) const = 0;
	virtual std::string toString() const = 0;
	virtual bool isNull() const = 0;
};

class IfcAbstractEntity {
public:
	virtual IfcEntities getInverse(Ifc2x3::Type::Enum c = Ifc2x3::Type::ALL) = 0;
	virtual IfcEntities getInverse(Ifc2x3::Type::Enum c, int i, const std::string& a) = 0;
	virtual std::string datatype() = 0;
	virtual ArgumentPtr getArgument (unsigned int i) = 0;
	virtual Ifc2x3::Type::Enum type() = 0;
	virtual bool is(Ifc2x3::Type::Enum v) = 0;
	virtual std::string toString() = 0;
	virtual unsigned int id() = 0;
};

#endif