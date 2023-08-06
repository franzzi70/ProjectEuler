#pragma once
class Field
{
public:
	Field()
	{
		value = 0;
	}

	bool IsEmpty()
	{
		return 0 == value;
	}
public:
	char value;
};

