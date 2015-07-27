#pragma once

#include <boost/iterator/zip_iterator.hpp>
#include <boost/range/iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/range/iterator_range_core.hpp>

template<class... Conts>
auto zip_range(Conts&... conts) {
	return boost::make_iterator_range(
		boost::make_zip_iterator(boost::make_tuple(conts.begin()...)),
		boost::make_zip_iterator(boost::make_tuple(conts.end()...))
	);
}

