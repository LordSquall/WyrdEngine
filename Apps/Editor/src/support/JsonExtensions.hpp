#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace nlohmann {

	/* Added support for unique and shared pointers as per https://github.com/nlohmann/json/issues/975 */
	template <typename T>
	struct adl_serializer<std::unique_ptr<T>> {
		static void to_json(json& j, const std::unique_ptr<T>& opt) {
			if (opt.get()) {
				j = *opt;
			}
			else {
				j = nullptr;
			}
		}

		static void from_json(const json& j, std::unique_ptr<T>& opt) {
			opt = std::make_unique(j);
		}
	};

	/* Added support for unique and shared pointers as per https://github.com/nlohmann/json/issues/975 */
	template <typename T>
	struct adl_serializer<std::shared_ptr<T>> {
		static void to_json(json& j, const std::shared_ptr<T>& opt) {
			if (opt.get()) {
				j = *opt;
			}
			else {
				j = nullptr;
			}
		}

		static void from_json(const json& j, std::shared_ptr<T>& opt) {
			opt = std::make_shared<T>(j);
		}
	};

}