#include <memory>
#include <algorithm>

#ifdef _WIN32
/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define TECODITY_PLATFORM_WINDOWS
#else
   /* Windows x86 */
//#error "x86 Builds are not supported!"
#endif
#endif

#ifdef TECODITY_DEBUG
#define TECODITY_ENABLE_ASSERTS
#define TRACE(msg) std::cout << msg << std::endl;
#else
#define TRACE(msg)
#endif

#ifdef TECODITY_ENABLE_ASSERTS
#if defined TECODITY_PLATFORM_WINDOWS
#define TECODITY_ASSERT(x, ...) { if(!(x)) {TECODITY_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#define TECODITY_CORE_ASSERT(x, ...) { if(!(x)) {TECODITY_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#endif
#else
#define TECODITY_ASSERT(x, ...)
#define TECODITY_CORE_ASSERT(x, ...)
#endif 

namespace Tecodity {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}