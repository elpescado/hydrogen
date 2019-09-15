#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <cppunit/TestAssert.h>
#include <QString>
#include <cassert>
#include <utility>
#include <sstream>

class TestHelper {
	static TestHelper *	__instance;
	QString m_sDataDir;
	QString m_sTestDataDir;
	public:
	TestHelper();

	QString data_dir() const { return m_sDataDir; }
	QString test_file(const QString &file) { return m_sTestDataDir + "/" + file; }

	static void			create_instance();
	static TestHelper* get_instance() { assert(__instance); return __instance; }
};

#define H2TEST_FILE(name) TestHelper::get_instance()->test_file(name)


namespace CppUnit {
template<>
struct assertion_traits<std::pair<const float,float> >
{
	static bool equal(const std::pair<const float,float> &lhs, const std::pair<const float,float> &rhs)
	{
		return lhs == rhs;
	}

	static std::string toString(const std::pair<const float,float> &p)
	{
		std::stringstream o;
		o << "(" << p.first << "," << p.second << ")";
		return o.str();
	}
};
}

#endif
