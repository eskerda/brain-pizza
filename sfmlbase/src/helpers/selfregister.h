#pragma once

#include <set>
#include <cassert>

/**
 * Inheriting from this class gives you a static method getAll()
 * that returns a set with all the current instances of the class.
 */
template <typename T> class SelfRegister {

public:
	SelfRegister()
	{ 
		_getAll().insert((T)this); 
	}

	SelfRegister(const SelfRegister& other)
	{
		_getAll().insert((T)this); 
	}

	void operator=(const SelfRegister& other)
	{
		_getAll().insert((T)this); 
	}

	~SelfRegister()
	{
		assert( _getAll().find((T)this) != _getAll().end() );
		_getAll().erase((T)this);
	}

	static std::set<T>& getAll()
	{
		return _getAll();
	}

	static typename std::set<T>::iterator begin()
	{
		return _getAll().begin();
	}

	static typename std::set<T>::iterator end()
	{
		return _getAll().end();
	}

	static void destroyAll() {
		for(typename std::set<T>::iterator i=_getAll().begin();
			i!=_getAll().end();
			i=_getAll().begin())
		{
			delete *i;
		}
	}

private:
	static std::set<T>& _getAll()
	{
		static std::set<T> instancies;
		return instancies;
	}

};
