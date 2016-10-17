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

#include <string>

#include <error.hpp>
#include <format.hpp>

#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>

#include <parsers/where.hpp>
#include <parsers/where/node.hpp>
#include <parsers/where/engine.hpp>
#include <parsers/filter/modern_filter.hpp>
#include <parsers/where/filter_handler_impl.hpp>

namespace check_cpu_filter {

	struct filter_obj {
		std::string time;
		std::string core;

		filter_obj(std::string time, std::string core) : time(time), core(core) {}

		long long get_total() const {
			return 0;
		}
		long long get_idle() const {
			return 0;
		}
		long long get_kernel() const {
			return 0;
		}
		std::string get_time() const {
			return time;
		}
		std::string get_core_s() const {
			return core;
		}
		std::string get_core_id() const {
			return boost::replace_all_copy(core, " ", "_");
		}
		long long get_core_i() const {
			return 0;
		}
	};
	typedef parsers::where::filter_handler_impl<boost::shared_ptr<filter_obj> > native_context;
	
	struct filter_obj_handler : public native_context {
		filter_obj_handler();
	};


	typedef modern_filter::modern_filters<filter_obj, filter_obj_handler> filter;
}

namespace check_mem_filter {

	struct filter_obj {
		std::string type;
		unsigned long long free;
		unsigned long long total;

		filter_obj(std::string type, unsigned long long free, unsigned long long total) : type(type), free(free), total(total) {}
		filter_obj(const filter_obj &other) : type(other.type), free(other.free), total(other.total) {}

		long long get_total() const {
			return total;
		}
		long long get_used() const {
			return total-free;
		}
		long long get_free() const {
			return free;
		}
		std::string get_type() const {
			return type;
		}

		std::string get_total_human() const {
			return format::format_byte_units(get_total());
		}
		std::string get_used_human() const {
			return format::format_byte_units(get_used());
		}
		std::string get_free_human() const {
			return format::format_byte_units(get_free());
		}
	};

	typedef parsers::where::filter_handler_impl<boost::shared_ptr<filter_obj> > native_context;
	struct filter_obj_handler : public native_context {
		filter_obj_handler();
	};
	typedef modern_filter::modern_filters<filter_obj, filter_obj_handler> filter;
}
/*
namespace check_proc_filter {
	typedef process_helper::process_info filter_obj;

	typedef parsers::where::filter_handler_impl<boost::shared_ptr<filter_obj> > native_context;
	struct filter_obj_handler : public native_context {
		filter_obj_handler();
	};
	typedef modern_filter::modern_filters<filter_obj, filter_obj_handler> filter;
}
*/
namespace os_version_filter {

	struct filter_obj {
		std::string kernel_name;
		std::string nodename;
		std::string kernel_version;
		std::string kernel_release;
		std::string machine;
		std::string processor;
		std::string os;

		filter_obj() {}

		std::string get_kernel_name() const {
			return kernel_name;
		}
		std::string get_nodename() const {
			return nodename;
		}
		std::string get_kernel_version() const {
			return kernel_version;
		}
		std::string get_kernel_release() const {
			return kernel_release;
		}
		std::string get_machine() const {
			return machine;
		}
		std::string get_processor() const {
			return processor;
		}
		std::string get_os() const {
			return os;
		}
	};
	typedef parsers::where::filter_handler_impl<boost::shared_ptr<filter_obj> > native_context;

	struct filter_obj_handler : public native_context {
		filter_obj_handler();
	};


	typedef modern_filter::modern_filters<filter_obj, filter_obj_handler> filter;
}

namespace check_uptime_filter {

	struct filter_obj {
		long long uptime;
		long long now;
		boost::posix_time::ptime boot;

		filter_obj(long long uptime, long long now, boost::posix_time::ptime boot) : uptime(uptime), now(now), boot(boot) {}

		long long get_uptime() const {
			return uptime;
		}
		long long get_boot() const {
			return now-uptime;
		}
		std::string get_boot_s() const {
			return format::format_date(boot);
		}
		std::string get_uptime_s() const {
			return format::itos_as_time(get_uptime()*1000);
		}
	};

	typedef parsers::where::filter_handler_impl<boost::shared_ptr<filter_obj> > native_context;
	struct filter_obj_handler : public native_context {
		filter_obj_handler();
	};
	typedef modern_filter::modern_filters<filter_obj, filter_obj_handler> filter;
}
