#pragma once
#include <algorithm>
#include<cmath>
#include<string>
#include <vector>
#include "BaseButtonAction.h"

//https://stackoverflow.com/a/495056/13115502 <-- useful template stuff



template <class T>
class ButtonValue : public BaseButtonAction
{
protected:
	T &ref_value;
public:
	ButtonValue(T &ref_value) : ref_value(ref_value) {};
};

class ButtonBoolToggle : public ButtonValue<bool>
{
public:
	ButtonBoolToggle(bool &ref_value)
		: ButtonValue<bool>(ref_value) {};
	virtual void execute() { this->ref_value = !this->ref_value; };
	virtual ~ButtonBoolToggle() {};
};

template<class T>
class ButtonSetValue : public ButtonValue<T>
{
	T new_value;
public:
	ButtonSetValue(T &ref_value, T new_value)
		: ButtonValue<T>(ref_value), new_value(new_value) {};
	virtual void execute();
	virtual ~ButtonSetValue() {};
};

template<class T>
class ButtonIncrementValue : public ButtonValue<T>
{
	T step, limit;
	bool limited;
public:
	ButtonIncrementValue(T &ref_value, T step)
		: ButtonValue<T>(ref_value), step(step), limit(step), limited(false) {};
	ButtonIncrementValue(T &ref_value, T step, T limit)
		: ButtonValue<T>(ref_value), step(step), limit(limit), limited(true) {};
	virtual void execute();
	virtual ~ButtonIncrementValue() {};
};

template<class T>
class ButtonScrollValue :public ButtonValue<T>
{
	std::vector<T> options;
	int index;
public:
	ButtonScrollValue(T &ref_value, std::vector<T> options)
		: ButtonValue<T>(ref_value), options(options)
	{
		this->index = std::find(options.begin(), options.end(), ref_value) - options.begin();
	};
	virtual void execute();
	virtual ~ButtonScrollValue() {};
};

#include "ButtonActions.tpp"