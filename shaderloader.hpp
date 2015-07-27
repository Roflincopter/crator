#pragma once

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <string>

boost::format format(boost::format format_string){
	return format_string;
}

template<class Hd, class... Tail>
boost::format format(boost::format format_string, Hd&& t, Tail&&... subs){
	return format(format_string % std::forward<Hd>(t), std::forward<Tail>(subs)...);
}

template <typename... Subs>
std::string load_shader(boost::filesystem::path file, Subs&&... subs){
	if(!boost::filesystem::exists(file)) {
		throw std::runtime_error("file did not exist: " +  boost::filesystem::canonical(file).string());
	}
	
	std::ifstream t(file.string());
	std::string contents(
		(std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>()
	);
	
	return str(format(boost::format(contents), std::forward<Subs>(subs)...));
	
}

