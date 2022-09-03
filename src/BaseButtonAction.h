#pragma once

class BaseButtonAction
{
protected:
public:
	virtual ~BaseButtonAction() {};
	virtual void execute() = 0;
};