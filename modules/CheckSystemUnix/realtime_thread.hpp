/*
 * Copyright 2004-2016 The NSClient++ Authors - https://nsclient.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>

#include "filter_config_object.hpp"

#include <nscapi/nscapi_settings_proxy.hpp>

#include <error.hpp>

/**
 * @ingroup NSClientCompat
 * PDH collector thread (gathers performance data and allows for clients to retrieve it)
 *
 * @version 1.0
 * first version
 *
 * @date 02-13-2005
 *
 * @author mickem
 *
 * @par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 */
template<class T>
struct rrd_buffer {
	typedef T value_type;
	typedef boost::circular_buffer<T> list_type;
	typedef typename list_type::const_iterator const_iterator;
	list_type seconds;
	list_type minutes;
	list_type hours;
	int second_counter;
	int minute_counter;

public:
	rrd_buffer() : second_counter(0), minute_counter(0) {
		seconds.resize(60);
		minutes.resize(60);
		hours.resize(24);
	}
	value_type get_average(long time) const {
		value_type ret;
		if (time <= 60) {
			for (const_iterator cit = seconds.end()-time; cit != seconds.end(); ++cit) {
				ret.add(*cit);
			}
			ret.normalize(time);
			return ret;
		}
		time /= 60;
		if (time <= 60) {
			for (const_iterator cit = minutes.end()-time; cit != minutes.end(); ++cit) {
				ret.add(*cit);
			}
			ret.normalize(time);
			return ret;
		}
		time/=60;
		if (time >= 24)
			throw nscp_exception("Size larger than buffer");
		for (const_iterator cit = hours.end()-time; cit != hours.end(); ++cit) {
			ret.add(*cit);
		}
		ret.normalize(time);
		return ret;
	}
	value_type calculate_avg(list_type &buffer) const {
		value_type ret;
		BOOST_FOREACH(const value_type &entry, buffer) {
			ret.add(entry);
		}
		ret.normalize(buffer.size());
		return ret;
	}

	void push(const value_type &value) {
		seconds.push_back(value);
		if (second_counter++ >= 59) {
			second_counter = 0;
			T avg = calculate_avg(seconds);
			minutes.push_back(avg);
			if (minute_counter++ >= 59) {
				minute_counter = 0;
				T avg = calculate_avg(minutes);
				hours.push_back(avg);
			}
		}
	}
};

class pdh_thread {
private:
//	typedef boost::unordered_map<std::string,PDH::pdh_instance> lookup_type;

	boost::shared_ptr<boost::thread> thread_;
	boost::shared_mutex mutex_;
//	HANDLE stop_event_;

//	std::list<PDH::pdh_object> configs_;
//	std::list<PDH::pdh_instance> counters_;
//	rrd_buffer<windows::system_info::cpu_load> cpu;
//	lookup_type lookups_;
public:

	std::string subsystem;
	std::string default_buffer_size;
	std::string filters_path_;

public:

//	void add_counter(const PDH::pdh_object &counter);
/*
	std::map<std::string,double> get_value(std::string counter);
	std::map<std::string,double> get_average(std::string counter, long seconds);
	std::map<std::string,long long> get_int_value(std::string counter);
	std::map<std::string,windows::system_info::load_entry> get_cpu_load(long seconds);
*/
	bool start();
	bool stop();

	void add_realtime_filter(boost::shared_ptr<nscapi::settings_proxy> proxy, std::string key, std::string query);

private:
	filters::filter_config_handler filters_;

	void thread_proc();

};
