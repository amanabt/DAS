#ifndef __STL_ENUM__
#define __STL_ENUM__

namespace std
{

template <typename enum_type, typename object_type>
inline enum_type valid_enum (const object_type& o)
{
	return static_cast<enum_type> (
		(o < static_cast<object_type> (enum_type::__END__)) ? o : 0);
}

};

#endif
