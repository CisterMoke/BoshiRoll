template<class T>
void ButtonSetValue<T>::execute()
{
	this->ref_value = new_value;
}

template<class T>
void ButtonIncrementValue<T>::execute()
{
	this->ref_value += step;
	bool lim_check = step < 0 ? this->ref_value < limit : this->ref_value > limit;
	if (limited && lim_check) { this->ref_value = limit; }
}

template<class T>
void ButtonScrollValue<T>::execute()
{
	index = (index + 1) % options.size();
	this->ref_value = *(options.begin() + index);
}